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

// #include <sched.h>
#include <sys/mman.h>
#include "forte/timerha.h"
#include "forte/io/mapper/io_mapper.h"
#include "bus.h"
#include "../device/ec_device.h"

namespace forte::eclipse4diac::io::ethercat {

  namespace {
    const char *const scmECControllerRequestFailed = "Request controller failed.";
    const char *const scmECCreateDomainFailed = "Controller create domain failed.";
    const char *const scmECConfigDeviceFailed = "Controller config device failed.";
    const char *const scmECConfigDevicePdoFailed = "Controller config device PDOs failed.";
    const char *const scmECRegisterPdoEntryFailed = "Controller register PDO entries failed.";
    const char *const scmECControllerActivateFailed = "Controller activate failed.";
    const char *const scmECGetDomainProcessDataFailed = "Controller get domain process data failed.";

  }

  ECBusHandler::ECBusHandler(CDeviceExecution &paDeviceExecution) :
    IODeviceMultiController(paDeviceExecution),
    mECController(nullptr),
    mECDomain(nullptr),
    mECDomainPd(nullptr),
    mInitializedFlag(false),
    mLoopPreparedFlag(false),
    mEnableFlag(false),
    mIsShuttingDown(false){
  }

  void ECBusHandler::setConfig(struct IODeviceController::Config *paConfig) {
    if(isAlive()) {
      DEVLOG_ERROR("ethercat[BusHandler]: Cannot change configuration while running.\n");
      return;
    }

    this->mConfig = *static_cast<Config *>(paConfig);
  }

  void ECBusHandler::addDevice(ECBusDeviceHandler *device) {
    if(device != 0) {
      mDevices.push_back(device);
    }
  }

  ECBusDeviceHandler* ECBusHandler::getDevice(size_t paDeviceIndex) {
    for(ECBusDeviceHandler *handler : mDevices) {
      if(handler == nullptr) {
        continue;
      }
      if(handler->mDeviceIndex == paDeviceIndex) {
        return handler;
      }
    }
    return nullptr;
  }

  void ECBusHandler::addSlaveHandle(size_t paDeviceIndex, std::unique_ptr<forte::io::IOHandle> paHandle) {
    ECBusDeviceHandler *device = getDevice(paDeviceIndex);
    if(device == nullptr) {
      return;
    }

    device->addHandle(static_cast<ECDeviceHandle *>(paHandle.release()));
  }

  void ECBusHandler::dropSlaveHandles(size_t paDeviceIndex) {
    ECBusDeviceHandler *device = getDevice(paDeviceIndex);
    if(device == nullptr) {
      return;
    }

    device->dropHandles();
  }

  ECDeviceHandler *ECBusHandler::getParentDevice(ECBusDeviceHandler *paDevice) {
    if(paDevice == nullptr) {
      return nullptr;
    }
    if(paDevice->mDeviceType == ECBusDeviceHandler::DeviceType::ECModule) {
      return static_cast<ECDeviceHandler *>(getDevice(paDevice->mDeviceIndex / 100 - 1));
    }
    return static_cast<ECDeviceHandler *>(paDevice);
  }

  bool ECBusHandler::isSlaveAvailable(size_t paDeviceIndex) {
    return getDevice(paDeviceIndex) != nullptr;
  }

  bool ECBusHandler::checkSlaveType(size_t paDeviceIndex, int paDeviceType) {
    ECBusDeviceHandler *device = getDevice(paDeviceIndex);
    if(device == nullptr) {
      return false;
    }

    return device->mDeviceType == paDeviceType;
  }

  const char *ECBusHandler::init() {
    mInitializedFlag = false;

    mECController = ecrt_request_master(mConfig.mECControllerId);
    if(mECController == nullptr) {
      DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u).\n", scmECControllerRequestFailed, mConfig.mECControllerId);
      return scmECControllerRequestFailed;
    }

    mECDomain = ecrt_master_create_domain(mECController);
    if(mECDomain == nullptr) {
      DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u).\n", scmECCreateDomainFailed, mConfig.mECControllerId);
      return scmECCreateDomainFailed;
    }

    mInitializedFlag = true;

    return 0;
  }

  void ECBusHandler::deInit() {
    if (mIsShuttingDown) {
      return;
    }
    mIsShuttingDown = true;
    DEVLOG_INFO("ECBusHandler deInit!\n");
    if(mDevices.empty() && mECController == nullptr) {
      return;
    }

    if(mECController != nullptr) {
      ecrt_release_master(mECController);
      mECController = nullptr;
    }

    mECDomain = nullptr;
    mECDomainPd = nullptr;

    for(ECBusDeviceHandler *it : mDevices) {
      delete it;
    }

    mDevices.clear();
  }

  forte::io::IOHandle *ECBusHandler::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
    HandleDescriptor &desc = static_cast<HandleDescriptor &>(paHandleDescriptor);
    ECBusDeviceHandler *device = getDevice(desc.mSlaveIndex);
    // Remove stale mapper entry left by prior session before creating a new handle with same ID.
    forte::io::IOMapper::getInstance().deregisterHandle(desc.mId);
    if(device == nullptr) {
      return nullptr;
    }

    switch (desc.mByteLength){
      case 1:
        return new ECDeviceHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_BYTE, desc.mOffset, desc.mId, *device);
      case 2:
        return new ECDeviceHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_WORD, desc.mOffset, desc.mId, *device);
      case 4:
        return new ECDeviceHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_DWORD, desc.mOffset, desc.mId, *device);
      case 8:
        return new ECDeviceHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_LWORD, desc.mOffset, desc.mId, *device);
      default:
        DEVLOG_ERROR("ethercat[BusHandler]: Unsupported handle byte length %u at device %zu offset %u.\n",
                     static_cast<unsigned int>(desc.mByteLength),
                     desc.mSlaveIndex,
                     static_cast<unsigned int>(desc.mOffset));
        break;
    }

    return nullptr;
  }

  void ECBusHandler::prepareLoop() {
    mLoopPreparedFlag = false;

    for(size_t i = 0; i < mDevices.size(); ++i) {
      ECBusDeviceHandler *handler = mDevices[i];
      if(handler->mDeviceType == ECBusDeviceHandler::DeviceType::ECModule) {
        continue;
      }

      ECDeviceHandler *deviceHandler = static_cast<ECDeviceHandler *>(handler);
      ECDeviceModel &deviceModel = deviceHandler->mECDeviceModel;

      ec_slave_config_t *sc = ecrt_master_slave_config(
        mECController,
        deviceModel.mAlias,
        deviceModel.mPosition,
        deviceModel.mVendorId,
        deviceModel.mProductCode);
      
      if(sc == nullptr) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u),Device position:%u.\n",
          scmECConfigDeviceFailed,
          mConfig.mECControllerId,
          deviceModel.mPosition);
        return;
      }

      if(deviceModel.mSyncList.size() > 0) {
        ec_sync_info_t *syncInfo = deviceModel.getSyncs();
        if(ecrt_slave_config_pdos(sc, EC_END, syncInfo)) {
          DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u),Device position:%u.\n"
                       ,scmECConfigDevicePdoFailed
                       ,mConfig.mECControllerId
                       ,deviceModel.mPosition);
          return;
        }
      }

      if(deviceModel.mEntryRegList.size() > 0) {
        ec_pdo_entry_reg_t *reg = deviceModel.getDomainRegs();
        if(ecrt_domain_reg_pdo_entry_list(mECDomain, reg)) {
          DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u),Device position:%u.\n"
                       ,scmECRegisterPdoEntryFailed
                       ,mConfig.mECControllerId
                       ,deviceModel.mPosition);
          return;
        } else {
          DEVLOG_INFO("ethercat[BusHandler]: PDO entries registered. Offsets:\n");
          for(size_t j = 0; j < deviceModel.mEntryRegList.size(); j++) {
            if(deviceModel.mEntryRegList[j].mOffset) {
              DEVLOG_INFO("ethercat[BusHandler]: [%zu] 0x%04X:%u → offset=%u\n"
                          ,j
                          ,deviceModel.mEntryRegList[j].mIndex
                          ,deviceModel.mEntryRegList[j].mSubIndex
                          ,*deviceModel.mEntryRegList[j].mOffset);
            }
          }
        }
      }

      DEVLOG_INFO("ethercat[BusHandler]: Configured device - Alias: %u, Position: %u, VendorId: 0x%08X, ProductCode: 0x%08X\n"
                  ,deviceModel.mAlias, deviceModel.mPosition, deviceModel.mVendorId, deviceModel.mProductCode);
      
      const int activateRet = ecrt_master_activate(mECController);
      if(activateRet) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u).\n", scmECControllerActivateFailed, mConfig.mECControllerId);
        return;
      }

      mECDomainPd = ecrt_domain_data(mECDomain);
      if(mECDomainPd == nullptr) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Controller ID: %u).\n", scmECGetDomainProcessDataFailed, mConfig.mECControllerId);
        return;
      }

      ec_master_state_t ms;
      ecrt_master_state(mECController, &ms);
      DEVLOG_INFO("ethercat[BusHandler]: Initial state - Devices responding: %u, AL States: 0x%02x\n", ms.slaves_responding, ms.al_states);

      size_t domainSize = ecrt_domain_size(mECDomain);
      DEVLOG_INFO("ethercat[BusHandler]: Domain size: %zu bytes\n", domainSize);

      DEVLOG_INFO("ethercat[BusHandler]: Controller activated successfully.\n");

      mLoopPreparedFlag = true;
    }
  }

  void ECBusHandler::runLoop() {
    if(!mInitializedFlag){
      return;
    }

    while(isAlive()) {
      // Wait until Controller FB enabled.
      while(!mEnableFlag && isAlive()) {
        sleepThread(10);
      }

      if(!mLoopPreparedFlag) {
        DEVLOG_INFO("ethercat[BusHandler]: All devices initialized. prepareLoop...\n");
        prepareLoop();
        // Check if prepareLoop executed successfully.
        if(!mLoopPreparedFlag) {
          DEVLOG_INFO("ethercat[BusHandler]: PrepareLoop failed.\n");
          return;
        }

        //If executed here, then prepareLoop executed successfully.
        clock_gettime(CLOCK_MONOTONIC, &mWakeupTime);
        mWakeupTime.tv_nsec += static_cast<long>(mConfig.mUpdateInterval) * PERIOD_NS; // µs → ns (PERIOD_NS = 1000)
        while(mWakeupTime.tv_nsec >= NSEC_PER_SEC) {
          mWakeupTime.tv_nsec -= NSEC_PER_SEC;
          mWakeupTime.tv_sec++;
        }
        DEVLOG_INFO("ethercat[BusHandler]: PrepareLoop completed, starting cyclic communication...\n");
      }

      int ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &mWakeupTime, NULL);
      if(ret) {
        mError = "ethercat[BusHandler]: clock_nanosleep() failed";
        DEVLOG_ERROR("%s: %s.\n", mError,strerror(ret));
        break;
      }

      // Check if cycle is still enabled before executing
      if(!mEnableFlag) {
        // Update wakeup time even when paused to avoid time drif
        mWakeupTime.tv_nsec += static_cast<long>(mConfig.mUpdateInterval) * PERIOD_NS;
        while(mWakeupTime.tv_nsec >= NSEC_PER_SEC) {
          mWakeupTime.tv_nsec -= NSEC_PER_SEC;
          mWakeupTime.tv_sec++;
        }
        continue;
      }

      ecrt_master_receive(mECController);
      ecrt_domain_process(mECDomain);

      for(ECBusDeviceHandler *handler : mDevices) {
        // Update the device with domain data.
        handler->update(mECDomainPd);
      }

      // Queue domain data for sending.
      ecrt_domain_queue(mECDomain);
      // Send process data to devices.
      ecrt_master_send(mECController);

      // Calculate next wakeup time (same cycle as mUpdateInterval µs).
      mWakeupTime.tv_nsec += static_cast<long>(mConfig.mUpdateInterval) * PERIOD_NS;
      while(mWakeupTime.tv_nsec >= NSEC_PER_SEC) {
        mWakeupTime.tv_nsec -= NSEC_PER_SEC;
        mWakeupTime.tv_sec++;
      }
    }

  }

  void ECBusHandler::enableECCycle(bool paEnableFlag) {
    mEnableFlag = paEnableFlag;

    if(paEnableFlag && mLoopPreparedFlag) {
      clock_gettime(CLOCK_MONOTONIC, &mWakeupTime);
      mWakeupTime.tv_nsec += static_cast<long>(mConfig.mUpdateInterval) * PERIOD_NS;
      while(mWakeupTime.tv_nsec >= NSEC_PER_SEC) {
        mWakeupTime.tv_nsec -= NSEC_PER_SEC;
        mWakeupTime.tv_sec++;
      }
      DEVLOG_INFO("[ECBusHandler]: EtherCAT cycle resumed.\n");
    }
  }

}
