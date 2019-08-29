/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
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
#include <handler/bus.h>
#include <io/mapper/io_mapper.h>
#include <processinterface.h>
#include "criticalregion.h"

const int EmbrickSlaveHandler::scmMaxUpdateErrors = 50;

EmbrickSlaveHandler::EmbrickSlaveHandler(EmbrickBusHandler* paBus, int paAddress, EmbrickSlaveInitPackage paInit) :
    mDelegate(0), mAddress(paAddress), mType((SlaveType) paInit.mDeviceId), mBus(paBus), mDataSendLength(paInit.mDataSendLength),
        mDataReceiveLength(paInit.mDataReceiveLength), mStatus(NotInitialized), mOldStatus(NotInitialized) {
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

  if(mDelegate != 0) {
    mDelegate->onSlaveDestroy();
  }
}

void EmbrickSlaveHandler::setConfig(Config paConfig) {
  if(paConfig.mUpdateInterval < 20) {
    DEVLOG_WARNING("embrick[EmbrickSlaveHandler]: Configured UpdateInterval not in range (>= 20). Set to 25.\n");
    paConfig.mUpdateInterval = 25;
  }

  this->mConfig = paConfig;
}

EmbrickSlaveHandler* EmbrickSlaveHandler::sendInit(EmbrickBusHandler* paBus, int paAddress) {
  EmbrickMasterInitPackage masterInit;
  masterInit.mSlaveAddress = (unsigned char) paAddress;
  masterInit.mSyncGapMultiplicator = SyncGapMultiplicator;

  unsigned char sendBuffer[sizeof(EmbrickMasterInitPackage)];
  unsigned char receiveBuffer[sizeof(EmbrickSlaveInitPackage)];

  masterInit.toBuffer(sendBuffer);

  // Send init via broadcast. Due to the sequential slave select activation, only one slave will respond.
  if(!paBus->broadcast(EmbrickBusHandler::Init, sendBuffer, sizeof(EmbrickMasterInitPackage), receiveBuffer, sizeof(EmbrickSlaveInitPackage))) {
    return 0;
  }

  EmbrickSlaveInitPackage initPackage = EmbrickSlaveInitPackage::fromBuffer(receiveBuffer);

  // Alter the value of data receive length -> the status byte is handled in the BusHandler
  initPackage.mDataReceiveLength--;

  DEVLOG_INFO("emBrick[EmbrickSlaveHandler]: ID - %d, ReceiveLength - %d, SendLength - %d, Producer - %d \n", initPackage.mDeviceId,
    initPackage.mDataReceiveLength, initPackage.mDataSendLength, initPackage.mProducerId);

  // Return slave instance
  return new EmbrickSlaveHandler(paBus, paAddress, initPackage);
}

int EmbrickSlaveHandler::update() {
  // Send update request to bus
  if(!mBus->transfer(mAddress, EmbrickBusHandler::Data, mUpdateSendImage, mDataSendLength, mUpdateReceiveImage, mDataReceiveLength, &mStatus, &mUpdateMutex)) {
    mUpdateErrorCounter++;
    if(mUpdateErrorCounter % 10 == 0) {
      DEVLOG_WARNING("emBrick[EmbrickSlaveHandler]: Slave %d reached transfer error threshold - %d out of %d\n", mAddress, mUpdateErrorCounter,
        scmMaxUpdateErrors);
    }
    return mUpdateErrorCounter <= scmMaxUpdateErrors ? 0 : -1;
  }

  // forte::core::IO::IOHandle the received image
  {
    CCriticalRegion criticalRegion(mHandleMutex);
    TSlaveHandleList::Iterator itEnd = mInputs.end();
    for(TSlaveHandleList::Iterator it = mInputs.begin(); it != itEnd; ++it) {
      if((*it)->hasObserver() && !(*it)->equal(mUpdateReceiveImageOld)) {
        // Inform Process Interface about change
        (*it)->onChange();
      }
    }
  }

  // Clone current image to old image
  memcpy(mUpdateReceiveImageOld, mUpdateReceiveImage, mDataReceiveLength);

  // Reset error counter
  if(mUpdateErrorCounter > 0) {
    mUpdateErrorCounter = 0;
  }

  // Check status
  if(mDelegate != 0 && mOldStatus != mStatus) {
    mDelegate->onSlaveStatus(mStatus, mOldStatus);
    mOldStatus = mStatus;
  }

  return 1;
}

void EmbrickSlaveHandler::forceUpdate() {
  return mBus->forceUpdate(index());
}

void EmbrickSlaveHandler::dropHandles() {
  CCriticalRegion criticalRegion(mHandleMutex);

  forte::core::io::IOMapper& mapper = forte::core::io::IOMapper::getInstance();

  TSlaveHandleList::Iterator itEnd = mInputs.end();
  for(TSlaveHandleList::Iterator it = mInputs.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }
  itEnd = mOutputs.end();
  for(TSlaveHandleList::Iterator it = mOutputs.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }

  mInputs.clearAll();
  mOutputs.clearAll();

}

void EmbrickSlaveHandler::addHandle(TSlaveHandleList* paList, EmbrickSlaveHandle* paHandle) {
  CCriticalRegion criticalRegion(mHandleMutex);
  paList->pushBack(paHandle);

  // TODO Maybe send indication event after connecting
}

EmbrickSlaveHandle* EmbrickSlaveHandler::getHandle(TSlaveHandleList* paList, int paIndex) {
  TSlaveHandleList::Iterator itEnd = paList->end();
  int i = 0;
  for(TSlaveHandleList::Iterator it = paList->begin(); it != itEnd; ++it, i++) {
    if(paIndex == i) {
      return *it;
    }
  }
  return NULL;
}

