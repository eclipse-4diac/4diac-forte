/*******************************************************************************
 * Copyright (c) 2016, 2023 Johannes Messmer (admin@jomess.com), fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Jörg Walter - more efficient pin handling
 *******************************************************************************/

#include "pin.h"
#include <devlog.h>
#include <unistd.h>
#include <fcntl.h>

const char * const EmbrickPinHandler::scmFailedToExportPin = "Failed to export GPIO pin.";
const char * const EmbrickPinHandler::scmFailedToSetDirection = "Failed to set GPIO direction.";
const char * const EmbrickPinHandler::scmFailedToOpenFile = "Failed to open sysfs file.";
const char * const EmbrickPinHandler::scmFailedToWriteFile = "Failed to write sysfs file.";
const char * const EmbrickPinHandler::scmNotInitialised = "Failed to write to not initialised sysfs stream.";

EmbrickPinHandler::EmbrickPinHandler(unsigned int paPin) :
  mError(0), mPinNumber(paPin) {

  // Init pin
  init();
}

EmbrickPinHandler::~EmbrickPinHandler() {
  deInit();
}

static bool writeFile(const std::string &fn, const std::string &val) {
  int fd = open(fn.c_str(), O_WRONLY);
  if (fd < 0 || write(fd, val.data(), val.size()) != (ssize_t)val.size()) {
    DEVLOG_DEBUG("emBrick[PinHandler]: writing %s to %s failed: %s\n",
                 fn.c_str(), val.c_str(), strerror(errno));
    close(fd);
    return false;
  }
  close(fd);
  return true;
}

void EmbrickPinHandler::init() {
  std::string pinStr = std::to_string(mPinNumber);

  // Use pin as output
  if (!writeFile("/sys/class/gpio/gpio" + pinStr + "/direction", "out")) {
    if (!writeFile("/sys/class/gpio/export", std::to_string(mPinNumber))) {
      return fail(scmFailedToExportPin);
  }

    didExport = true;

    if (!writeFile("/sys/class/gpio/gpio" + pinStr + "/direction", "out"))
      return fail(scmFailedToSetDirection);
  }

  // Prepare pin stream for usage
  std::string fn = "/sys/class/gpio/gpio" + pinStr + "/value";
  mPinFd = open(fn.c_str(), O_WRONLY);
  if (mPinFd < 0) {
    return fail(scmFailedToOpenFile);
  }

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %i ready.\n", mPinNumber);
}

void EmbrickPinHandler::deInit() {
  std::string fileName;

  // Close pin stream
  if (mPinFd >= 0) {
    close(mPinFd);
    mPinFd = -1;
  }

  if (didExport) {
    if (!writeFile("/sys/class/gpio/unexport", std::to_string(mPinNumber))) {
      return fail(scmFailedToExportPin);
    }
  }

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %i stopped.\n", mPinNumber);
}

bool EmbrickPinHandler::set(bool paState) {
  if (mPinFd < 0) {
    fail(scmNotInitialised);
    return false;
  }

  lseek(mPinFd, 0, SEEK_SET);
  if (write(mPinFd, paState?"1":"0", 1) != 1) {
    fail(scmFailedToWriteFile);
    return false;
  }

  return true;
}

void EmbrickPinHandler::fail(const char* paReason) {
  mError = paReason;
  DEVLOG_ERROR("emBrick[PinHandler]: %s\n", paReason);
}
