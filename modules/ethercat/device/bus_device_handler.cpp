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
#include "forte/util/devlog.h"
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

    for (auto &handle : mInputs) {
      handle->syncDomainData(paECDomainData);
      if (handle->hasObserver() && !handle->equal(mUpdateRecvImageOld)) {
        handle->onChange();
      }
    }

    std::copy(mUpdateRecvImage.begin(), mUpdateRecvImage.end(), mUpdateRecvImageOld.begin());

    for (auto &handle : mOutputs) {
      handle->syncDomainData(paECDomainData);
    }
  }

  void ECBusDeviceHandler::addHandle(std::unique_ptr<ECDeviceHandle> paHandle) {
    std::vector<std::unique_ptr<ECDeviceHandle>> *list = nullptr;
    switch (paHandle->getDirection()) {
      case forte::io::IOMapper::In: list = &mInputs; break;
      case forte::io::IOMapper::Out: list = &mOutputs; break;
      default: return;
    }

    // The domain offset has to be in place before update() can see the handle.
    ECDeviceHandler *device = (mBus != nullptr) ? mBus->getParentDevice(this) : nullptr;
    if (device == nullptr ||
        !device->mECDeviceModel.bindHandleOffset(paHandle->entryRegIndex(), paHandle->ecDomainOffsetPtr())) {
      DEVLOG_ERROR("ethercat[BusDeviceHandler]: PDO entry 0x%04X:%02X of handle '%s' is not registered or already "
                   "mapped by another handle.\n",
                   paHandle->entryIndex(), paHandle->entrySubIndex(), paHandle->handleId().c_str());
      return;
    }
    addHandle(*list, std::move(paHandle));
  }

  void ECBusDeviceHandler::dropHandles() {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    forte::io::IOMapper &mapper = forte::io::IOMapper::getInstance();

    // During shutdown the parent device and its model may already be destroyed.
    if (mBus != nullptr && !mBus->isShuttingDown()) {
      if (ECDeviceHandler *device = mBus->getParentDevice(this); device != nullptr) {
        for (auto &handle : mInputs) {
          device->mECDeviceModel.unbindHandleOffset(handle->ecDomainOffsetPtr());
        }
        for (auto &handle : mOutputs) {
          device->mECDeviceModel.unbindHandleOffset(handle->ecDomainOffsetPtr());
        }
      }
    }

    deregisterHandles(mapper);
  }

  void ECBusDeviceHandler::deregisterHandles(forte::io::IOMapper &mapper) {
    for (auto &it : mInputs) {
      mapper.deregisterHandle(it->handleId());
    }

    for (auto &it : mOutputs) {
      mapper.deregisterHandle(it->handleId());
    }

    mInputs.clear();
    mOutputs.clear();
  }

  void ECBusDeviceHandler::addHandle(std::vector<std::unique_ptr<ECDeviceHandle>> &paList,
                                     std::unique_ptr<ECDeviceHandle> paHandle) {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    paList.push_back(std::move(paHandle));

    // TODO Maybe send indication event after connecting
  }
}