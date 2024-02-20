/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "RevPiController.h"
#include "RevPiHandle.h"

const char * const RevPiController::scmFailedToOpenControlFile =
    "Failed to open control file.";
const char * const RevPiController::scmFailedToResetControllerFile =
    "Failed to reset control file";
const char * const RevPiController::scmFailedToGetDeviceList =
    "Failed to get device list";

RevPiController::RevPiController(CDeviceExecution& paDeviceExecution) : forte::core::IO::IODeviceMultiController(paDeviceExecution),
    deviceCount(0) {
  config.UpdateInterval = 25;
}

void RevPiController::setConfig(struct forte::core::IO::IODeviceController::Config* config) {
  Config newConfig = *static_cast<Config*>(config);

  if (newConfig.UpdateInterval <= 0) {
    DEVLOG_WARNING(
        "[RevPiController] Configured UpdateInterval is set to an invalid value '%d'. Set to 25.\n",
        newConfig.UpdateInterval);
    newConfig.UpdateInterval = 25;
  }

  this->config = newConfig;
}

const char* RevPiController::init() {
  int status;

  if ((status = piControlReset()) != 0) {
    DEVLOG_ERROR("[RevPiController] %s - %d\n", scmFailedToResetControllerFile,
        status);
    return scmFailedToResetControllerFile;
  }

  if ((status = piControlGetDeviceInfoList((SDeviceInfoStr*) &deviceList))
      < 0) {
    DEVLOG_ERROR("[RevPiController] %s - %d\n", scmFailedToGetDeviceList,
        status);
    return scmFailedToGetDeviceList;
  }

  deviceCount = status;

  for (int i = 0; i < deviceCount; i++) {
    DEVLOG_INFO(
        "[RevPiController] Found device %d - Type: %d -  Active: %d - Outputs: %d - Inputs: %d - Configs: %d\n",
        deviceList[i].i8uAddress, deviceList[i].i16uModuleType,
        deviceList[i].i8uActive,
        deviceList[i].i16uBaseOffset + deviceList[i].i16uOutputOffset,
        deviceList[i].i16uBaseOffset + deviceList[i].i16uInputOffset,
        deviceList[i].i16uConfigLength);
  }

  return 0;
}

forte::core::IO::IOHandle* RevPiController::initHandle(
    forte::core::IO::IODeviceMultiController::HandleDescriptor *handleDescriptor) {
  HandleDescriptor desc = *static_cast<HandleDescriptor*>(handleDescriptor);

  return new RevPiHandle(this, desc.type, desc.direction,
      static_cast<uint16_t>(deviceList[desc.slaveIndex + 1].i16uBaseOffset + desc.offset
          + (desc.direction == forte::core::IO::IOMapper::In ?
              deviceList[desc.slaveIndex + 1].i16uInputOffset :
              deviceList[desc.slaveIndex + 1].i16uOutputOffset)), desc.position);
}

void RevPiController::deInit() {
  piControlReset();
  piControlClose();
}

void RevPiController::runLoop() {
  while (isAlive()) {
    mTimeoutSemaphore.timedWait(1000000000 / config.UpdateInterval);

    // Perform poll operation
    this->checkForInputChanges();

  }
}

void RevPiController::addSlaveHandle(int, forte::core::IO::IOHandle* handle) {
  handleMutex.lock();
  handle->is(forte::core::IO::IOMapper::In) ? inputHandles.pushBack(handle) : outputHandles.pushBack(handle);
  handleMutex.unlock();
}

void RevPiController::dropSlaveHandles(int) {
  // Is handled by #dropHandles method
}

bool RevPiController::isSlaveAvailable(int index) {
  return index + 1 < deviceCount;
}

bool RevPiController::checkSlaveType(int index, int type) {
  return deviceList[index + 1].i16uModuleType == type;
}

bool RevPiController::isHandleValueEqual(forte::core::IO::IOHandle* handle) {
  return !((RevPiHandle*) handle)->check();
}
