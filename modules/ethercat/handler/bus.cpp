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
#include <fmt/base.h>
#include "forte/timerha.h"
#include "forte/io/mapper/io_mapper.h"
#include "bus.h"
#include "../slave/ec_device.h"

namespace forte::eclipse4diac::io::ethercat {

  namespace {
    const char *const scmECMasterRequestFailed = "Request master failed.";
    const char *const scmECCreateDomainFailed = "Master create domain failed.";
    const char *const scmECConfigSlaveFailed = "Master config slave failed.";
    const char *const scmECConfigSlavePdoFailed = "Master config slave pdoes failed.";
    const char *const scmECRegisterPdoEntryFailed = "Master register pdo entries failed.";
    const char *const scmECMasterActivateFailed = "Master activate failed.";
    const char *const scmECGetDomainProcessDataFailed = "Master get domain process data failed.";

  }

  ECBusHandler::ECBusHandler(CDeviceExecution &paDeviceExecution) :
    IODeviceMultiController(paDeviceExecution),
    mECMaster(nullptr),
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

  void ECBusHandler::addSlave(ECSlaveHandler *slave) {
    if(slave != 0) {
      mDevices.push_back(slave);
    }
  }

  ECSlaveHandler* ECBusHandler::getSlave(size_t paSlaveIndex) {
    for(ECSlaveHandler *handler : mDevices) {
      if(handler == nullptr) {
        continue;
      }
      if(handler->mSlaveIndex == paSlaveIndex) {
        return handler;
      }
    }
    return nullptr;
  }

  void ECBusHandler::addSlaveHandle(size_t paSlaveIndex, std::unique_ptr<forte::io::IOHandle> paHandle) {
    ECSlaveHandler *slave = getSlave(paSlaveIndex);
    if(slave == nullptr) {
      return;
    }

    slave->addHandle((ECSlaveHandle*)paHandle.release());
  }

  void ECBusHandler::dropSlaveHandles(size_t paSlaveIndex) {
    ECSlaveHandler *slave = getSlave(paSlaveIndex);
    if(slave == nullptr) {
      return;
    }

    slave->dropHandles();
  }

  bool ECBusHandler::isSlaveAvailable(size_t paSlaveIndex) {
    return getSlave(paSlaveIndex) != nullptr;
  }

  bool ECBusHandler::checkSlaveType(size_t paSlaveIndex, int paSlaveType) {
    ECSlaveHandler *slave = getSlave(paSlaveIndex);
    if(slave == nullptr) {
      return false;
    }

    return slave->mSlaveType == paSlaveType;
  }

  const char *ECBusHandler::init() {
    mInitializedFlag = false;

    mECMaster = ecrt_request_master(mConfig.mECMasterId);
    if(mECMaster == nullptr) {
      DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u).\n", scmECMasterRequestFailed, mConfig.mECMasterId);
      return scmECMasterRequestFailed;
    }

    mECDomain = ecrt_master_create_domain(mECMaster);
    if(mECDomain == nullptr) {
      DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u).\n", scmECCreateDomainFailed, mConfig.mECMasterId);
      return scmECCreateDomainFailed;
    }

    mInitializedFlag = true;

    return 0;
  }

  void ECBusHandler::deInit() {
    DEVLOG_INFO("ECBusHandler deInit!\n");
    mIsShuttingDown = true;
    if(mDevices.empty() && mECMaster == nullptr) {
      return;
    }

    if(mECMaster != nullptr) {
      ecrt_release_master(mECMaster);
      mECMaster = nullptr;
    }

    mECDomain = nullptr;
    mECDomainPd = nullptr;

    for(ECSlaveHandler *it : mDevices) {
      delete it;
    }

    mDevices.clear();
  }

  forte::io::IOHandle *ECBusHandler::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
    HandleDescriptor &desc = static_cast<HandleDescriptor &>(paHandleDescriptor);
    ECSlaveHandler *slave = getSlave(desc.mSlaveIndex);
    // Remove stale mapper entry left by prior session before creating a new handle with same ID.
    forte::io::IOMapper::getInstance().deregisterHandle(desc.mId);
    if(slave == nullptr) {
      return nullptr;
    }

    switch (desc.mByteLength){
      case 1:
        return new ECSlaveHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_BYTE, desc.mOffset, desc.mId, slave);
      case 2:
        return new ECSlaveHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_WORD, desc.mOffset, desc.mId, slave);
      case 4:
        return new ECSlaveHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_DWORD, desc.mOffset, desc.mId, slave);
      case 8:
        return new ECSlaveHandle(this, desc.mDirection, CIEC_ANY::EDataTypeID::e_LWORD, desc.mOffset, desc.mId, slave);
      default:
        DEVLOG_ERROR("ethercat[BusHandler]: Unsupported handle byte length %u at slave %zu offset %u.\n",
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
      ECSlaveHandler *handler = mDevices[i];
      if(handler->mSlaveType == ECSlaveHandler::SlaveType::ECModule) {
        continue;
      }

      ECDeviceHandler *deviceHandler = static_cast<ECDeviceHandler *>(handler);
      ECDeviceModel &deviceModel = deviceHandler->mECDeviceModel;

      ec_slave_config_t *sc = ecrt_master_slave_config(
        mECMaster,
        deviceModel.mAlias,
        deviceModel.mPosition,
        deviceModel.mVendorId,
        deviceModel.mProductCode);
      
      if(sc == nullptr) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u),Slave Position:%u.\n",
          scmECConfigSlaveFailed,
          mConfig.mECMasterId,
          deviceModel.mPosition);
        return;
      }

      if(deviceModel.mSyncList.size() > 0) {
        ec_sync_info_t *syncInfo = deviceModel.getSyncs();
        if(ecrt_slave_config_pdos(sc, EC_END, syncInfo)) {
          DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u),Slave Position:%u.\n"
                       ,scmECConfigSlavePdoFailed
                       ,mConfig.mECMasterId
                       ,deviceModel.mPosition);
          return;
        }
      }

      if(deviceModel.mEntryRegList.size() > 0) {
        ec_pdo_entry_reg_t *reg = deviceModel.getDomainRegs();
        if(ecrt_domain_reg_pdo_entry_list(mECDomain, reg)) {
          DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u),Slave Position:%u.\n"
                       ,scmECRegisterPdoEntryFailed
                       ,mConfig.mECMasterId
                       ,deviceModel.mPosition);
          return;
        } else {
          DEVLOG_INFO("ethercat[BusHandler]: PDO entries registered. Offsets:\n");
          for(size_t i = 0; i < deviceModel.mEntryRegList.size(); i++) {
            if(deviceModel.mEntryRegList[i].mOffset) {
              DEVLOG_INFO("ethercat[BusHandler]: [%zu] 0x%04X:%u → offset=%u\n"
                          ,i
                          ,deviceModel.mEntryRegList[i].mIndex
                          ,deviceModel.mEntryRegList[i].mSubIndex
                          ,*deviceModel.mEntryRegList[i].mOffset);
            }
          }
        }
      }

      DEVLOG_INFO("ethercat[BusHandler]: Configured slave - Alias: %u, Position: %u, VendorId: 0x%08X, ProductCode: 0x%08X\n"
                  ,deviceModel.mAlias, deviceModel.mPosition, deviceModel.mVendorId, deviceModel.mProductCode);
      
      const int activateRet = ecrt_master_activate(mECMaster);
      if(activateRet) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u).\n", scmECMasterActivateFailed, mConfig.mECMasterId);
        return;
      }

      mECDomainPd = ecrt_domain_data(mECDomain);
      if(mECDomainPd == nullptr) {
        DEVLOG_ERROR("ethercat[BusHandler]: %s(Master ID: %u).\n", scmECGetDomainProcessDataFailed, mConfig.mECMasterId);
        return;
      }

      // struct sched_param param = {};
      // param.sched_priority = sched_get_priority_max(SCHED_FIFO);
      // if(sched_setscheduler(0, SCHED_FIFO, &param)) {
      //   DEVLOG_WARNING("ethercat[BusHandler]: sched_setscheduler failed:%s,(Master ID: %u)\n", strerror(errno), mConfig.mECMasterId);
      // }

      // if(mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
      //   DEVLOG_WARNING("ethercat[BusHandler]: Failed to lock memory: %s,(Master ID: %u)\n", strerror(errno), mConfig.mECMasterId);
      // }

      ec_master_state_t ms;
      ecrt_master_state(mECMaster, &ms);
      DEVLOG_INFO("ethercat[BusHandler]: Initial state - Slave responding: %u, AL States: 0x%02x\n", ms.slaves_responding, ms.al_states);

      size_t domainSize = ecrt_domain_size(mECDomain);
      DEVLOG_INFO("ethercat[BusHandler]: Domain size: %zu bytes\n", domainSize);

      DEVLOG_INFO("ethercat[BusHandler]: Master activated successfullly.\n");

      mLoopPreparedFlag = true;
    }
  }

  void ECBusHandler::runLoop() {
    if(!mInitializedFlag){
      return;
    }

    while(isAlive()) {
      // Wait until MasterFB enabled.
      while(!mEnableFlag && isAlive()) {
        sleepThread(1000);
      }

      if(!mLoopPreparedFlag) {
        DEVLOG_INFO("ethercat[BusHandler]: All slaves initialized. prepareLoop...\n");
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

      ecrt_master_receive(mECMaster);
      ecrt_domain_process(mECDomain);

      for(ECSlaveHandler *handler : mDevices) {
        // Update the slave with domain data.
        handler->update(mECDomainPd);
      }

      // Queue domain data for sending.
      ecrt_domain_queue(mECDomain);
      // Send process data to slaves.
      ecrt_master_send(mECMaster);

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