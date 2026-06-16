/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include <string>
#include <ecrt.h>
#include "../slave/slave.h"
#include <forte/io/device/io_controller_multi.h>

namespace forte::eclipse4diac::io::ethercat {

  class ECSlaveHandler;

  class ECBusHandler : public forte::io::IODeviceMultiController {

    public:
      explicit ECBusHandler(CDeviceExecution &paDeviceExecution);

      struct Config : IODeviceController::Config {
        unsigned int mECMasterId;
        unsigned int mUpdateInterval;
      };

      class HandleDescriptor : public IODeviceMultiController::HandleDescriptor {
        public:
          uint8_t mOffset;
          uint8_t mByteLength;

          HandleDescriptor(std::string const &paId,
                           forte::io::IOMapper::Direction paDirection,
                           size_t paSlaveIndex,
                           uint8_t paOffset,
                           uint8_t paByteLength) : 
              IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex),
              mOffset(paOffset),
              mByteLength(paByteLength){
          }
      };

      void setConfig(struct IODeviceController::Config *paConfig) override;

      void addSlave(ECSlaveHandler *slave);
      ECSlaveHandler *getSlave(size_t paSlaveIndex);

      void addSlaveHandle(size_t paSlaveIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;
      void dropSlaveHandles(size_t paSlaveIndex) override;

      void enableECCycle(bool paEnableFlag);
      bool isShuttingDown() const {
        return mIsShuttingDown;
      }

    protected:
      const char* init() override;
      void deInit() override;

      forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) override;

      void prepareLoop();
      void runLoop() override;

      // Config
      struct  Config mConfig;
      
      // Devices
      std::vector<ECSlaveHandler *> mDevices;
      
    private:
      bool isSlaveAvailable(size_t paSlaveIndex) override;
      bool checkSlaveType(size_t paSlaveIndex, int paSlaveType);
      
      ec_master_t *mECMaster;
      ec_domain_t *mECDomain;
      uint8_t *mECDomainPd;

      struct timespec mWakeupTime;
      static const long PERIOD_NS = 1000L;      // 1us in nanoseconds
      static const long NSEC_PER_SEC = 1000000000L;

      bool mInitializedFlag;
      bool mLoopPreparedFlag;
      bool mEnableFlag;
      bool mIsShuttingDown;
  };

}