/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "slave.h"
#include <cstddef>
#include "../handler/bus.h"
#include "forte/io/mapper/io_mapper.h"
#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::embrick {

  namespace {
    const int scmMaxUpdateErrors = 50;
  }

  EmbrickSlaveHandler::EmbrickSlaveHandler(EmbrickBusHandler *paBus, int paAddress, EmbrickSlaveInitPackage paInit) :
      mDelegate(nullptr),
      mAddress(paAddress),
      mType((SlaveType) paInit.mDeviceId),
      mBus(paBus),
      mDataSendLength(paInit.mDataSendLength),
      mDataReceiveLength(paInit.mDataReceiveLength),
      mStatus(NotInitialized),
      mOldStatus(NotInitialized) {
    mUpdateSendImage = new unsigned char[mDataSendLength];
    mUpdateReceiveImage = new unsigned char[mDataReceiveLength];
    mUpdateReceiveImageOld = new unsigned char[mDataReceiveLength];

    memset(mUpdateSendImage, 0, mDataSendLength);
    memset(mUpdateReceiveImage, 0, mDataReceiveLength);
    memset(mUpdateReceiveImageOld, 0, mDataReceiveLength);

    mUpdateErrorCounter = 0;

    // Default config
    mConfig.mUpdateInterval = 25;
  }

  EmbrickSlaveHandler::~EmbrickSlaveHandler() {
    dropHandles();

    delete mUpdateSendImage;
    delete mUpdateReceiveImage;
    delete mUpdateReceiveImageOld;

    if (mDelegate != nullptr) {
      mDelegate->onSlaveDestroy();
    }
  }

  void EmbrickSlaveHandler::setConfig(Config paConfig) {
    if (paConfig.mUpdateInterval < 20) {
      DEVLOG_WARNING("embrick[EmbrickSlaveHandler]: Configured UpdateInterval not in range (>= 20). Set to 25.\n");
      paConfig.mUpdateInterval = 25;
    }

    this->mConfig = paConfig;
  }

  EmbrickSlaveHandler *EmbrickSlaveHandler::sendInit(EmbrickBusHandler *paBus, int paAddress) {
    EmbrickMasterInitPackage masterInit;
    masterInit.mSlaveAddress = (unsigned char) paAddress;
    masterInit.mSyncGapMultiplicator = SyncGapMultiplicator;

    unsigned char sendBuffer[sizeof(EmbrickMasterInitPackage)];
    unsigned char receiveBuffer[sizeof(EmbrickSlaveInitPackage)];

    masterInit.toBuffer(sendBuffer);

    // Send init via broadcast. Due to the sequential slave select activation, only one slave will respond.
    if (!paBus->broadcast(EmbrickBusHandler::Init, sendBuffer, sizeof(EmbrickMasterInitPackage), receiveBuffer,
                          sizeof(EmbrickSlaveInitPackage))) {
      return nullptr;
    }

    EmbrickSlaveInitPackage initPackage = EmbrickSlaveInitPackage::fromBuffer(receiveBuffer);

    // Alter the value of data receive length -> the status byte is handled in the BusHandler
    initPackage.mDataReceiveLength--;

    DEVLOG_INFO("emBrick[EmbrickSlaveHandler]: ID - %d, ReceiveLength - %d, SendLength - %d, Producer - %d \n",
                initPackage.mDeviceId, initPackage.mDataReceiveLength, initPackage.mDataSendLength,
                initPackage.mProducerId);

    // Return slave instance
    return new EmbrickSlaveHandler(paBus, paAddress, initPackage);
  }

  int EmbrickSlaveHandler::update() {
    // Send update request to bus
    if (!mBus->transfer(mAddress, EmbrickBusHandler::Data, mUpdateSendImage, mDataSendLength, mUpdateReceiveImage,
                        mDataReceiveLength, &mStatus, &mUpdateMutex)) {
      mUpdateErrorCounter++;
      if (mUpdateErrorCounter % 10 == 0) {
        DEVLOG_WARNING("emBrick[EmbrickSlaveHandler]: Slave %d reached transfer error threshold - %d out of %d\n",
                       mAddress, mUpdateErrorCounter, scmMaxUpdateErrors);
      }
      return mUpdateErrorCounter <= scmMaxUpdateErrors ? 0 : -1;
    }

    // forte::IO::IOHandle the received image
    {
      util::CCriticalRegion criticalRegion(mHandleMutex);
      for (EmbrickSlaveHandle *it : mInputs) {
        if (it->hasObserver() && !it->equal(mUpdateReceiveImageOld)) {
          // Inform Process Interface about change
          it->onChange();
        }
      }
    }

    // Clone current image to old image
    memcpy(mUpdateReceiveImageOld, mUpdateReceiveImage, mDataReceiveLength);

    // Reset error counter
    if (mUpdateErrorCounter > 0) {
      mUpdateErrorCounter = 0;
    }

    // Check status
    if (mDelegate != nullptr && mOldStatus != mStatus) {
      mDelegate->onSlaveStatus(mStatus, mOldStatus);
      mOldStatus = mStatus;
    }

    return 1;
  }

  void EmbrickSlaveHandler::forceUpdate() {
    return mBus->forceUpdate(index());
  }

  void EmbrickSlaveHandler::dropHandles() {
    util::CCriticalRegion criticalRegion(mHandleMutex);

    forte::io::IOMapper &mapper = forte::io::IOMapper::getInstance();

    for (EmbrickSlaveHandle *it : mInputs) {
      mapper.deregisterHandle(*it);
      delete it;
    }

    for (EmbrickSlaveHandle *it : mOutputs) {
      mapper.deregisterHandle(*it);
      delete it;
    }

    mInputs.clear();
    mOutputs.clear();
  }

  void EmbrickSlaveHandler::addHandle(std::vector<EmbrickSlaveHandle *> &paList, EmbrickSlaveHandle *paHandle) {
    util::CCriticalRegion criticalRegion(mHandleMutex);
    paList.push_back(paHandle);

    // TODO Maybe send indication event after connecting
  }

  EmbrickSlaveHandle *EmbrickSlaveHandler::getHandle(std::vector<EmbrickSlaveHandle *> &paList, size_t paIndex) {
    if (paList.size() <= paIndex) {
      return nullptr;
    }
    return paList[paIndex];
  }
} // namespace forte::eclipse4diac::io::embrick
