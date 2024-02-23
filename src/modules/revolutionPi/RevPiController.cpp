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

using namespace forte::core::io;

const char * const RevPiController::scmFailedToOpenControlFile =
    "Failed to open control file.";
const char * const RevPiController::scmFailedToResetControllerFile =
    "Failed to reset control file";
const char * const RevPiController::scmFailedToGetDeviceList =
    "Failed to get device list";

RevPiController::RevPiController(CDeviceExecution& paDeviceExecution) : IODeviceMultiController(paDeviceExecution),
    deviceCount(0) {
  config.updateInterval = 25;
}

void RevPiController::setConfig(struct IODeviceController::Config* paConfig) {
  Config newConfig = *static_cast<Config*>(paConfig);

  if (newConfig.updateInterval <= 0) {
    DEVLOG_WARNING(
        "[RevPiController] Configured UpdateInterval is set to an invalid value '%d'. Set to 25.\n",
        newConfig.updateInterval);
    newConfig.updateInterval = 25;
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

IOHandle* RevPiController::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
  HandleDescriptor &desc(static_cast<HandleDescriptor&>(paHandleDescriptor));

  return new RevPiHandle(this, desc.mType, desc.mDirection,
      static_cast<uint16_t>(deviceList[desc.mSlaveIndex + 1].i16uBaseOffset + desc.mOffset
          + (desc.mDirection == IOMapper::In ?
              deviceList[desc.mSlaveIndex + 1].i16uInputOffset :
              deviceList[desc.mSlaveIndex + 1].i16uOutputOffset)), desc.mPosition);
}

void RevPiController::deInit() {
  piControlReset();
  piControlClose();
}

void RevPiController::runLoop() {
  while (isAlive()) {
    mTimeoutSemaphore.timedWait(1000000000 / config.updateInterval);

    // Perform poll operation
    this->checkForInputChanges();

  }
}

void RevPiController::addSlaveHandle(int, IOHandle* paHandle) {
  CCriticalRegion criticalRegion(mHandleMutex);
  paHandle->isInput() ? mInputHandles.pushBack(paHandle) : mOutputHandles.pushBack(paHandle);
}

void RevPiController::dropSlaveHandles(int) {
  // Is handled by #dropHandles method
}

bool RevPiController::isSlaveAvailable(int paIndex) {
  return paIndex + 1 < deviceCount;
}

bool RevPiController::checkSlaveType(int paIndex, int paType) {
  return deviceList[paIndex + 1].i16uModuleType == paType;
}

bool RevPiController::isHandleValueEqual(IOHandle* paHandle) {
  return !static_cast<RevPiHandle*>(paHandle)->check();
}
