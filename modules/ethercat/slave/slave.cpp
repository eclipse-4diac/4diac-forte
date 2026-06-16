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

#include "slave.h"
#include "../handler/bus.h"
#include "forte/io/mapper/io_mapper.h"
#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::ethercat {
  ECSlaveHandler::ECSlaveHandler(ECBusHandler *paBus,
                                 SlaveType paSlaveType,
                                 size_t paSlaveIndex) : 
      mDelegate(nullptr),
      mSlaveIndex(paSlaveIndex),
      mSlaveType(paSlaveType),
      mBus(paBus),
      mUpdateSendImage(nullptr),
      mUpdateRecvImage(nullptr),
      mUpdateRecvImageOld(nullptr),
      mDataSendLength(0),
      mDataRecvLength(0),
      mStatus(NotInitialized),
      mOldStatus(NotInitialized) {
  }

  ECSlaveHandler::~ECSlaveHandler() {
    dropHandles();

    delete[] mUpdateSendImage;
    delete[] mUpdateRecvImage;
    delete[] mUpdateRecvImageOld;

    if (mDelegate != nullptr) {
      if(mBus != nullptr && mBus->isShuttingDown()) {
        mDelegate = nullptr;
        return;
      }
      mDelegate->onSlaveDestroy();
    }
  }

  void ECSlaveHandler::initBuffer(uint16_t paDataSendLength, uint16_t paDataRecvLength) {
    mDataSendLength = paDataSendLength;
    mDataRecvLength = paDataRecvLength;

    mUpdateSendImage = new unsigned char[mDataSendLength];
    mUpdateRecvImage = new unsigned char[mDataRecvLength];
    mUpdateRecvImageOld = new unsigned char[mDataRecvLength];

    memset(mUpdateSendImage, 0, mDataSendLength);
    memset(mUpdateRecvImage, 0, mDataRecvLength);
    memset(mUpdateRecvImageOld, 0, mDataRecvLength);
  }

  void ECSlaveHandler::update(uint8_t *paECDomainData) {
    util::CCriticalRegion critialRegion(mHandleMutex);

    for(ECSlaveHandle *handle : mInputs) {
      handle->syncDomainData(paECDomainData);
      if(handle->hasObserver() && !handle->equal(mUpdateRecvImageOld)){
        handle->onChange();
      }
    }

    memcpy(mUpdateRecvImageOld, mUpdateRecvImage, mDataRecvLength);

    for(ECSlaveHandle *handle : mOutputs) {
      handle->syncDomainData(paECDomainData);
    }
  }

  void ECSlaveHandler::dropHandles() {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    forte::io::IOMapper &mapper = forte::io::IOMapper::getInstance();

    if(mBus != nullptr && mBus->isShuttingDown()) {
      mInputs.clear();
      mOutputs.clear();
      return;
    }

    for(ECSlaveHandle *it : mInputs) {
      mapper.deregisterHandle(it->handleId());
      delete it;
    }

    for(ECSlaveHandle *it : mOutputs) {
      mapper.deregisterHandle(it->handleId());
      delete it;
    }

    mInputs.clear();
    mOutputs.clear();
  }

  void ECSlaveHandler::addHandle(std::vector<ECSlaveHandle *> &paList, ECSlaveHandle *paHandle) {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    paList.push_back(paHandle);

    //TODO Maybe send indication event after connecting
  }

  ECSlaveHandle *ECSlaveHandler::getHandle(std::vector<ECSlaveHandle *> &paList, size_t paIndex) {
    if(paList.size() <= paIndex) {
      return nullptr;
    }
    return paList[paIndex];
  }
}