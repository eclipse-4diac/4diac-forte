/*******************************************************************************
 * Copyright (c) 2023 OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jörg Walter - initial implementation
 *******************************************************************************/

#include <string>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <devlog.h>
#include <fcntl.h>
#include <unistd.h>

#include <io/mapper/io_handle_bit.h>
#include "gpiochip_controller.h"

const char *const GPIOChipController::scmFailedToAccessChip = "Failed to open GPIO chip device";
const char *const GPIOChipController::scmFailedToAcquireLine = "Failed to acquire GPIO line";
const char *const GPIOChipController::scmFailedToWatchLine = "Failed to monitor GPIO line for events";
const char *const GPIOChipController::scmFailedToWriteLine = "Failed to write GPIO line";
const char *const GPIOChipController::scmFailedToReadLine = "Failed to read GPIO line";
const char *const GPIOChipController::scmOK = "OK";

static inline void print_line_info(int fd, int line) {
  struct gpiochip_info info{};
  if (ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info) == -1) {
    DEVLOG_ERROR("[GPIOChipController] Could not get chip info\n");
    return;
  }
  DEVLOG_INFO("[GPIOChipController] GPIO Chip name: %s, label %s, %i lines\n", info.name, info.label, info.lines);

  struct gpioline_info line_info{};
  line_info.line_offset = line;
  if (ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &line_info) == -1) {
    DEVLOG_ERROR("[GPIOChipController] Could not get line info\n");
    return;
  }
  DEVLOG_INFO("[GPIOChipController] Line %d name: %s, consumer %s. flags %s %s %s %s %s\n",
                 line,
                 line_info.name,
                 line_info.consumer,
                 (line_info.flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT",
                 (line_info.flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGH",
                 (line_info.flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "",
                 (line_info.flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPEN_SOURCE" : "",
                 (line_info.flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");
}

const char* GPIOChipController::init() {
  auto devname = "/dev/gpiochip" + std::to_string(mConfig.mChipNumber);
  auto fd = open(devname.c_str(), O_RDONLY);
  if (fd < 0) {
    DEVLOG_ERROR("[GPIOChipController::init] Could not access GPIO chip %i: %s\n", mConfig.mChipNumber, strerror(errno));
    return scmFailedToAccessChip;
  }

  print_line_info(fd, mConfig.mLineNumber);

  static const unsigned mode[] = { GPIOHANDLE_REQUEST_INPUT, GPIOHANDLE_REQUEST_OUTPUT, GPIOHANDLE_REQUEST_OUTPUT|GPIOHANDLE_REQUEST_OPEN_DRAIN, GPIOHANDLE_REQUEST_OUTPUT|GPIOHANDLE_REQUEST_OPEN_SOURCE };
  static const unsigned pull[] = { 0, GPIOHANDLE_REQUEST_BIAS_DISABLE, GPIOHANDLE_REQUEST_BIAS_PULL_UP, GPIOHANDLE_REQUEST_BIAS_PULL_DOWN };
  static const unsigned active[] = { 0, GPIOHANDLE_REQUEST_ACTIVE_LOW };
  auto flags = mode[mConfig.mReadWriteMode] | pull[mConfig.mBiasMode] | active[mConfig.mActiveLow];

  if (mConfig.mReadWriteMode == Config::Input) {
    struct gpioevent_request erq{};
    erq.lineoffset = mConfig.mLineNumber;
    erq.handleflags = flags;
    erq.eventflags = GPIOEVENT_REQUEST_BOTH_EDGES;

    if (ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &erq) == -1) {
      close(fd);
      DEVLOG_ERROR("[GPIOChipController::init] Could not acquire input events from GPIO chip %i line %i flags 0x%x: %s\n", mConfig.mChipNumber, mConfig.mLineNumber, erq.handleflags, strerror(errno));
      return scmFailedToAcquireLine;
    }
    mFd = erq.fd;

  } else {
    struct gpiohandle_request hrq{};
    hrq.lines = 1;
    hrq.lineoffsets[0] = mConfig.mLineNumber;
    hrq.flags = flags;
    if (ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &hrq) == -1) {
      close(fd);
      DEVLOG_ERROR("[GPIOChipController::init] Could not acquire GPIO chip %i line %i flags 0x%x: %s\n", mConfig.mChipNumber, mConfig.mLineNumber, hrq.flags, strerror(errno));
      return scmFailedToAcquireLine;
    }
    mFd = hrq.fd;
  }
  close(fd);

  DEVLOG_INFO("[GPIOChipController::init] GPIO chip %i line %i flags 0x%x ready\n", mConfig.mChipNumber, mConfig.mLineNumber, flags);
  return nullptr;
}

void GPIOChipController::deInit() {
  if (mFd != -1) {
    close(mFd);
    mFd = -1;
  }

  DEVLOG_INFO("[GPIOChipController::deInit] GPIO Chip %i line %i shutdown\n", mConfig.mChipNumber, mConfig.mLineNumber);
}

void GPIOChipController::runLoop() {
  if (mConfig.mReadWriteMode != Config::Input) {
    // output does not need any loop logic, values are written directly from the handle
    while(isAlive() && !hasError()) {
      CThread::sleepThread(100);
    }
    return;
  }

  struct gpiohandle_data data{};
  if (ioctl(mFd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) == -1) {
    DEVLOG_ERROR("[GPIOChipController::runLoop] Could not read initial value of GPIO chip %i line %i: %s\n", mConfig.mChipNumber, mConfig.mLineNumber, strerror(errno));
    mError = scmFailedToReadLine;
    return;
  }
  mValue = data.values[0];
  DEVLOG_DEBUG("[GPIOChipController::runLoop] GPIO Chip %i line %i initial value %i\n", mConfig.mChipNumber, mConfig.mLineNumber, mValue);
  if (mValue) {
    checkForInputChanges();
  }

  while (isAlive() && !hasError()) {
    struct pollfd pfd{};
    pfd.fd = mFd;
    pfd.events = POLLIN;

    if (poll(&pfd, 1, 100) == -1) {
      DEVLOG_ERROR("[GPIOChipController::runLoop] Could not wait for input events from GPIO chip %i line %i: %s\n", mConfig.mChipNumber, mConfig.mLineNumber, strerror(errno));
      mError = scmFailedToWatchLine;
      return;
    } else if (!(pfd.revents & POLLIN)) {
      continue;
    }

    struct gpioevent_data event{};
    if (read(mFd, &event, sizeof(event)) != sizeof(event)) {
      DEVLOG_ERROR("[GPIOChipController::runLoop] Could not read event from GPIO chip %i line %i: %s\n", mConfig.mChipNumber, mConfig.mLineNumber, strerror(errno));
      mError = scmFailedToReadLine;
      return;
    }

    mValue = (event.id == GPIOEVENT_EVENT_RISING_EDGE);
    DEVLOG_DEBUG("[GPIOChipController::runLoop] GPIO Chip %i line %i value %i\n", mConfig.mChipNumber, mConfig.mLineNumber, mValue);
    checkForInputChanges();
  }
}

void GPIOChipController::handleChangeEvent(forte::core::io::IOHandle *) {
    struct gpiohandle_data data{};
    data.values[0] = mValue;
    if (ioctl(mFd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) == -1) {
      DEVLOG_ERROR("[GPIOChipController::runLoop] Could not write output data to GPIO chip %i line %i\n", mConfig.mChipNumber, mConfig.mLineNumber);
      mError = scmFailedToWriteLine;
    }
}

forte::core::io::IOHandle* GPIOChipController::initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) {
  return new forte::core::io::IOHandleBit(this, paHandleDescriptor->mDirection, 0, 0, &mValue);
}
