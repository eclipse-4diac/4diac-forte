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
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#include "bus_device_handler.h"
#include "../handler/bus.h"
#include "../device/ec_device.h"
#include "forte/io/mapper/io_mapper.h"
#include "forte/util/criticalregion.h"
#include <algorithm>

namespace forte::eclipse4diac::io::ethercat {
  ECBusDeviceHandler::ECBusDeviceHandler(ECBusHandler *paBus,
                                 DeviceType paDeviceType,
                                 size_t paDeviceIndex) : 
      mDelegate(nullptr),
      mDeviceIndex(paDeviceIndex),
      mDeviceType(paDeviceType),
      mBus(paBus),
      mDataSendLength(0),
      mDataRecvLength(0),
      mStatus(NotInitialized),
      mOldStatus(NotInitialized) {
  }

  ECBusDeviceHandler::~ECBusDeviceHandler() {
    dropHandles();

    if (mDelegate != nullptr) {
      if(mBus != nullptr && mBus->isShuttingDown()) {
        mDelegate = nullptr;
        return;
      }
      mDelegate->onDeviceDestroy();
    }
  }

  void ECBusDeviceHandler::initBuffer(uint16_t paDataSendLength, uint16_t paDataRecvLength) {
    mDataSendLength = paDataSendLength;
    mDataRecvLength = paDataRecvLength;

    mUpdateSendImage.assign(paDataSendLength, 0);
    mUpdateRecvImage.assign(paDataRecvLength, 0);
    mUpdateRecvImageOld.assign(paDataRecvLength, 0);
  }

  void ECBusDeviceHandler::update(uint8_t *paECDomainData) {
    util::CCriticalRegion critialRegion(mHandleMutex);

    for(ECDeviceHandle *handle : mInputs) {
      handle->syncDomainData(paECDomainData);
      if(handle->hasObserver() && !handle->equal(mUpdateRecvImageOld)){
        handle->onChange();
      }
    }

    std::copy(mUpdateRecvImage.begin(), mUpdateRecvImage.end(), mUpdateRecvImageOld.begin());

    for(ECDeviceHandle *handle : mOutputs) {
      handle->syncDomainData(paECDomainData);
    }
  }

  void ECBusDeviceHandler::dropHandles() {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    forte::io::IOMapper &mapper = forte::io::IOMapper::getInstance();

    if(mBus != nullptr && mBus->isShuttingDown()) {
      mInputs.clear();
      mOutputs.clear();
      return;
    }

    if(mBus != nullptr && mBus->isLoopPrepared()) {
      ECDeviceHandler *device = mBus->getParentDevice(this);
      if(device != nullptr) {
        auto persistHandleOffset = [&](ECDeviceHandle *handle) {
          for(EntryReg &reg : device->mECDeviceModel.mEntryRegList) {
            if(reg.mOffset == &handle->mECDomainDataOffset) {
              reg.mDomainOffset = handle->mECDomainDataOffset;
              reg.mOffsetValid = true;
            }
          }
        };
        for(ECDeviceHandle *handle : mInputs) {
          persistHandleOffset(handle);
        }
        for(ECDeviceHandle *handle : mOutputs) {
          persistHandleOffset(handle);
        }
      }
    }

    for(ECDeviceHandle *it : mInputs) {
      mapper.deregisterHandle(it->handleId());
      delete it;
    }

    for(ECDeviceHandle *it : mOutputs) {
      mapper.deregisterHandle(it->handleId());
      delete it;
    }

    mInputs.clear();
    mOutputs.clear();
  }

  void ECBusDeviceHandler::addHandle(std::vector<ECDeviceHandle *> &paList, ECDeviceHandle *paHandle) {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    paList.push_back(paHandle);

    //TODO Maybe send indication event after connecting
  }

  ECDeviceHandle *ECBusDeviceHandler::getHandle(std::vector<ECDeviceHandle *> &paList, size_t paIndex) {
    if(paList.size() <= paIndex) {
      return nullptr;
    }
    return paList[paIndex];
  }
}