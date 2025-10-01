/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "RevPiController.h"
#include "RevPiHandle.h"

using namespace forte::io;

namespace forte::eclipse4diac::io::revpi {

  const char *const RevPiController::scmFailedToOpenControlFile = "Failed to open control file.";
  const char *const RevPiController::scmFailedToResetControllerFile = "Failed to reset control file";
  const char *const RevPiController::scmFailedToGetDeviceList = "Failed to get device list";

  RevPiController::RevPiController(CDeviceExecution &paDeviceExecution) :
      IODeviceMultiController(paDeviceExecution),
      mDeviceCount(0) {
    mConfig.updateInterval = 25;
  }

  void RevPiController::setConfig(struct IODeviceController::Config *paConfig) {
    Config newConfig = *static_cast<Config *>(paConfig);

    if (newConfig.updateInterval <= 0) {
      DEVLOG_WARNING("[RevPiController] Configured UpdateInterval is set to an invalid value '%d'. Set to 25.\n",
                     newConfig.updateInterval);
      newConfig.updateInterval = 25;
    }

    this->mConfig = newConfig;
  }

  const char *RevPiController::init() {
    int status;

    if ((status = piControlReset()) != 0) {
      DEVLOG_ERROR("[RevPiController] %s - %d\n", scmFailedToResetControllerFile, status);
      return scmFailedToResetControllerFile;
    }

    if ((status = piControlGetDeviceInfoList((SDeviceInfoStr *) &mDeviceList)) < 0) {
      DEVLOG_ERROR("[RevPiController] %s - %d\n", scmFailedToGetDeviceList, status);
      return scmFailedToGetDeviceList;
    }

    mDeviceCount = status;

    for (int i = 0; i < mDeviceCount; i++) {
      DEVLOG_INFO(
          "[RevPiController] Found device %d - Type: %d -  Active: %d - Outputs: %d - Inputs: %d - Configs: %d\n",
          mDeviceList[i].i8uAddress, mDeviceList[i].i16uModuleType, mDeviceList[i].i8uActive,
          mDeviceList[i].i16uBaseOffset + mDeviceList[i].i16uOutputOffset,
          mDeviceList[i].i16uBaseOffset + mDeviceList[i].i16uInputOffset, mDeviceList[i].i16uConfigLength);
    }

    return 0;
  }

  IOHandle *RevPiController::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
    HandleDescriptor &desc(static_cast<HandleDescriptor &>(paHandleDescriptor));

    return new RevPiHandle(
        this, desc.mType, desc.mDirection,
        static_cast<uint16_t>(mDeviceList[desc.mSlaveIndex + 1].i16uBaseOffset + desc.mOffset +
                              (desc.mDirection == IOMapper::In ? mDeviceList[desc.mSlaveIndex + 1].i16uInputOffset
                                                               : mDeviceList[desc.mSlaveIndex + 1].i16uOutputOffset)),
        desc.mPosition);
  }

  void RevPiController::deInit() {
    piControlReset();
    piControlClose();
  }

  void RevPiController::runLoop() {
    while (isAlive()) {
      mTimeoutSemaphore.timedWait(1000000000 / mConfig.updateInterval);

      // Perform poll operation
      this->checkForInputChanges();
    }
  }

  void RevPiController::addSlaveHandle(size_t, std::unique_ptr<IOHandle> paHandle) {
    forte::util::CCriticalRegion criticalRegion(mHandleMutex);
    paHandle->isInput() ? mInputHandles.push_back(std::move(paHandle)) : mOutputHandles.push_back(std::move(paHandle));
  }

  void RevPiController::dropSlaveHandles(size_t) {
    // Is handled by #dropHandles method
  }

  bool RevPiController::isSlaveAvailable(size_t paIndex) {
    return paIndex + 1 < mDeviceCount;
  }

  bool RevPiController::checkSlaveType(size_t paIndex, int paType) {
    return mDeviceList[paIndex + 1].i16uModuleType == paType;
  }

  bool RevPiController::isHandleValueEqual(IOHandle &paHandle) {
    return !static_cast<RevPiHandle &>(paHandle).check();
  }
} // namespace forte::eclipse4diac::io::revpi
