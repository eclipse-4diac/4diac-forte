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

#include "bus.h"

#include <algorithm>
#include <slave/slave.h>
#include <slave/packages.h>

#include <slave/handles/bit.h>
#include <slave/handles/analog.h>
#include <slave/handles/analog10.h>
#include "criticalregion.h"
#include <utils/timespec_utils.h>

const char * const EmbrickBusHandler::scmSlaveUpdateFailed = "Update of slave failed.";
const char * const EmbrickBusHandler::scmNoSlavesFound = "No slave modules found.";

EmbrickBusHandler::EmbrickBusHandler(CDeviceExecution& paDeviceExecution) : forte::core::io::IODeviceMultiController(paDeviceExecution),
    mSpi(0), mSlaveSelect(0), mSlaves(0), mSlaveCount(0), mLoopActive(false), mSList(0) {
  // Set init time
  struct timespec ts;
  // TODO Check compile error. Had to to add rt libary to c++ make flags
  clock_gettime(CLOCK_MONOTONIC, &ts);

  mInitTime = ts.tv_sec;
  mLastTransfer = micros();

  // Default config
  mConfig.mBusInterface = 1;
  mConfig.mBusSelectPin = 49;
  mConfig.mBusInitSpeed = EmbrickSPIHandler::scmDefaultSpiSpeed;
  mConfig.mBusLoopSpeed = EmbrickSPIHandler::scmMaxSpiSpeed;
}

void EmbrickBusHandler::setConfig(struct forte::core::io::IODeviceController::Config* paConfig) {
  // Check if BusHandler is active -> configuration changes are not allowed
  if (isAlive()) {
    DEVLOG_ERROR(
        "emBrick[BusHandler]: Cannot change configuration while running.\n");
    return;
  }

  this->mConfig = *static_cast<Config*>(paConfig);
}

const char* EmbrickBusHandler::init() {
  // Start handlers
  mSpi = new EmbrickSPIHandler(mConfig.mBusInterface);
  mSlaveSelect = new EmbrickPinHandler(mConfig.mBusSelectPin);
  mSlaves = new TSlaveList();

  // Check handlers
  if (checkHandlerError()) {
    return mError;
  }

  // Set SPI sped for initialization
  mSpi->setSpeed(mConfig.mBusInitSpeed);

  // Disable slave select -> reset all slaves
  mSlaveSelect->disable();

  // Wait for reset
  sleep(1);

  // Enable slave select -> the first slave waits for initialization
  mSlaveSelect->enable();

  // Wait for init
  microsleep(SyncGapDuration * 2);

  // Init the slaves sequentially. Abort if the init package is ignored -> no further slaves found.
  int slaveCounter = 1;
  int attempts = 0;

  do {
    EmbrickSlaveHandler *slave = EmbrickSlaveHandler::sendInit(this, slaveCounter);

    if (slave != 0) {
      mSlaves->pushBack(slave);

      // Activate next slave by sending the 'SelectNextSlave' command to the current slave
      // It enables the slave select pin for the next slave on the bus
      transfer(slave->mAddress, SelectNextSlave);

      slaveCounter++;
      attempts = 0;
    }

    microsleep(SyncGapDuration * 2);
  } while (++attempts < 3);

  mSlaveCount = slaveCounter - 1;

  if (mSlaveCount == 0) {
    return scmNoSlavesFound;
  }

  // Increase the speed of the bus for data transfers
  mSpi->setSpeed(mConfig.mBusLoopSpeed);

  return 0;
}

void EmbrickBusHandler::deInit() {
  // Free memory -> delete all slave instances and hardware handlers
  TSlaveList::Iterator itEnd(mSlaves->end());
  for(TSlaveList::Iterator it = mSlaves->begin(); it != itEnd; ++it) {
    delete *it;
  }
  delete mSlaves;
  mSlaves = 0;

  delete mSpi;
  delete mSlaveSelect;
}

forte::core::io::IOHandle* EmbrickBusHandler::initHandle(
    forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) {
  HandleDescriptor &desc = static_cast<HandleDescriptor&>(*paHandleDescriptor);

  EmbrickSlaveHandler *slave = getSlave(desc.mSlaveIndex);
  if(slave == 0) {
    return 0;
  }

  switch (desc.mType) {
  case Bit:
      return new EmbrickBitSlaveHandle(this, desc.mDirection, desc.mOffset, desc.mPosition,
        slave);
  case Analog:
      return new EmbrickAnalogSlaveHandle(this, desc.mDirection, desc.mOffset, slave);
  case Analog10:
      return new EmbrickAnalog10SlaveHandle(this, desc.mDirection, desc.mOffset, slave);
  }

  return 0;
}

void EmbrickBusHandler::prepareLoop() {
  // Get current time
  clock_gettime(CLOCK_MONOTONIC, &mNextLoop);

  // Scheduling
  // TODO Combine slaves list and scheduling information
  // DISCUSS Speed of array and forte_list?
  TSlaveList::Iterator it = mSlaves->begin();
  mSList = (struct SEntry**) forte_malloc(sizeof(struct SEntry*) * mSlaveCount);
  for (int i = 0; i < mSlaveCount; i++) {
    mSList[i] = (struct SEntry*) forte_malloc(sizeof(struct SEntry));
    mSList[i]->mSlave = *it;
    mSList[i]->mNextDeadline = mNextLoop;
    mSList[i]->mLastDuration = 0;
    mSList[i]->mForced = false;
    mSList[i]->mDelayed = false;

    ++it;
  }
  mSNext = mSList[0];
  mLoopActive = false;
}

void EmbrickBusHandler::runLoop() {
  prepareLoop();

  // Init loop variables
  SEntry *sCur = 0;

  while (isAlive()) {
    // Sleep till next deadline is reached or loop is waked up

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if(timespecLessThan(&now, &mSNext->mNextDeadline)){ //only wait if the deadline is in the future
      struct timespec timeToSleep;
      timespecSub(&mSNext->mNextDeadline, &now, &timeToSleep);
      mForceLoop.timedWait(timeToSleep.tv_sec * 1E9 + timeToSleep.tv_nsec);
    }

    // Set current slave
    mLoopActive = true;
    sCur = mSNext;

    // Set next deadline of current slave
    clock_gettime(CLOCK_MONOTONIC, &sCur->mNextDeadline);
    addTime(sCur->mNextDeadline, 1000000 / sCur->mSlave->mConfig.mUpdateInterval);

    // Remove delayed and forced flag
    sCur->mForced = false;
    sCur->mDelayed = false;

    // Remove lock during blocking operation -> allows forced update interrupts
    mSyncObject.unlock();

    uint64_t ms = micros();

    // Perform update on current slave
    if (-1 == sCur->mSlave->update()) {
      mError = scmSlaveUpdateFailed;
      // Check for critical bus errors
      if(checkHandlerError() || hasError()) {
        break;
      }
    }

    // Search for next deadline -> set lock to avoid changes of list
    mSyncObject.lock();
    mLoopActive = false;

    // Store update duration
    sCur->mLastDuration = (uint16_t) (micros() - ms);

    // If current slave is forced again -> add update duration to deadline
    if(sCur->mForced) {
      addTime(sCur->mNextDeadline, sCur->mLastDuration);
    }

    for (unsigned int i = 0; i < mSlaveCount; i++) {
      if (timespecLessThan(&mSList[i]->mNextDeadline, &mSNext->mNextDeadline)) {
        mSNext = mSList[i];
      }
    }
  }

  // Clean loop
  cleanLoop();
}

void EmbrickBusHandler::cleanLoop() {
  // Free memory of list
  for(int i = 0; i < mSlaveCount; i++) {
    forte_free(mSList[i]);
  }
  forte_free(mSList);
  mSList = 0;
}

bool EmbrickBusHandler::checkHandlerError() {
  if (mSpi->hasError()) {
    mError = mSpi->mError;
    return true;
  }

  if (mSlaveSelect->hasError()) {
    mError = mSlaveSelect->paError;
    return true;
  }

  return false;
}

EmbrickSlaveHandler* EmbrickBusHandler::getSlave(int paIndex) {
  if (mSlaves == 0) {
    return 0;
  }

  TSlaveList::Iterator itEnd = mSlaves->end();
  int i = 0;
  for(TSlaveList::Iterator it = mSlaves->begin(); it != itEnd; ++it, i++) {
    if (paIndex == i) {
      return *it;
    }
  }

  return 0;
}

void EmbrickBusHandler::forceUpdate(int paIndex) {
  mSyncObject.lock();

  if (mSList == 0 || mSlaveCount <= paIndex || mSList[paIndex]->mForced) {
    mSyncObject.unlock();
    return;
  }

  SEntry *e = mSList[paIndex];

  e->mForced = true;
  clock_gettime(CLOCK_MONOTONIC, &e->mNextDeadline);
  if (!mLoopActive) {
    if(!mSNext->mDelayed && !mSNext->mForced) {
      struct timespec ts = e->mNextDeadline;
      addTime(ts, e->mLastDuration * 2);

      if(!timespecLessThan(&ts, &mSNext->mNextDeadline)) {
        mSNext->mDelayed = true;
      }

      mSNext = e;
    }

    // Force next loop
    mForceLoop.inc();
  }
  mSyncObject.unlock();
}

void EmbrickBusHandler::addSlaveHandle(int paIndex, forte::core::io::IOHandle* paHandle) {
  EmbrickSlaveHandler* slave = getSlave(paIndex);
  if(slave == 0) {
    return;
  }

  slave->addHandle((EmbrickSlaveHandle*) paHandle);
}

void EmbrickBusHandler::dropSlaveHandles(int paIndex) {
  EmbrickSlaveHandler* slave = getSlave(paIndex);
  if(slave == 0) {
    return;
  }

  slave->dropHandles();
}

bool EmbrickBusHandler::isSlaveAvailable(int paIndex) {
  return getSlave(paIndex) != 0;
}

bool EmbrickBusHandler::checkSlaveType(int paIndex, int paType) {
  EmbrickSlaveHandler* slave = getSlave(paIndex);
  if(slave == 0) {
    return false;
  }

  return slave->mType == paType;
}

bool EmbrickBusHandler::transfer(unsigned int paTarget, Command paCmd,
    unsigned char* paDataSend, int paDataSendLength, unsigned char* paDataReceive,
    int paDataReceiveLength, EmbrickSlaveHandler::SlaveStatus* paStatus, CSyncObject *paSyncMutex) {
  unsigned int dataLength = std::max(paDataSendLength, paDataReceiveLength + 1); // + 1 status byte

  unsigned int bufferLength = sizeof(EmbrickHeaderPackage) + dataLength + 1; // + 1 checksum byte
  if (bufferLength > TransferBufferLength) {
    DEVLOG_ERROR("emBrick[BusHandler]: Transfer buffer size exceeded.\n");
    return false;
  }

  memset(mSendBuffer, 0, bufferLength);
  memset(mRecvBuffer, 0, bufferLength);

  // Prepare header
  EmbrickHeaderPackage* header = (EmbrickHeaderPackage*) mSendBuffer;

  header->mAddress = (char) paTarget;
  header->mCommand = paCmd;
  header->mChecksum = calcChecksum((unsigned char*) header, 2);

  if(paSyncMutex) {
    paSyncMutex->lock();
  }
  memcpy(mSendBuffer + sizeof(EmbrickHeaderPackage), paDataSend, paDataSendLength);
  if(paSyncMutex) {
   paSyncMutex->unlock();
  }
  mSendBuffer[sizeof(EmbrickHeaderPackage) + paDataSendLength] = calcChecksum(
      mSendBuffer + sizeof(EmbrickHeaderPackage), paDataSendLength);

  // Invert data of master
  for(unsigned int i = 0; i < bufferLength; i++) {
    mSendBuffer[i] = (unsigned char) ~mSendBuffer[i];
  }

  // Send and receive buffer via SPI
  int attempts = 3;
  int fails = 0;
  bool ok;
  do {
    // Wait required microseconds between messages
    uint64_t microTime = micros();
    if(mLastTransfer + SyncGapDuration > microTime) {
      microsleep(mLastTransfer + (uint64_t) SyncGapDuration - microTime);
    }

    ok = mSpi->transfer(mSendBuffer, mRecvBuffer, bufferLength);
    mLastTransfer = micros();

    // Critical error of bus -> break immediately
    if(!ok) {
      break;
    }

    // Validate checksum
    ok = calcChecksum(mRecvBuffer + sizeof(EmbrickHeaderPackage),
        bufferLength - sizeof(EmbrickHeaderPackage)) == 0;
    if (!ok) {
      DEVLOG_DEBUG("emBrick[BusHandler]: Transfer - Invalid checksum\n");
    }
  } while (!ok && ++fails < attempts);

  // Check if command was transmitted successfully
  if (!ok) {
    if (paTarget != 0 && paCmd != Data) {
      DEVLOG_DEBUG(
          "emBrick[BusHandler]: Failed to send command %d to slave %d.\n", paCmd,
          paTarget);
    }
    return false;
  }

  // Copy result
  if(paSyncMutex) {
    paSyncMutex->lock();
  }

  if(paStatus) {
    *paStatus = (EmbrickSlaveHandler::SlaveStatus) mRecvBuffer[sizeof(EmbrickHeaderPackage)];
  }

  memcpy(paDataReceive, mRecvBuffer + sizeof(EmbrickHeaderPackage) + 1,
         paDataReceiveLength);
  if(paSyncMutex) {
    paSyncMutex->unlock();
  }

  return true;
}

unsigned char EmbrickBusHandler::calcChecksum(unsigned char * paData, int paDataLen) {
  unsigned char c = 0;
  for(int i = 0; i < paDataLen; i++) {
    c = static_cast<unsigned char>(c + paData[i]);
  }

  return static_cast<unsigned char>(scmChecksumConstant - c);
}

uint64_t EmbrickBusHandler::micros() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e3) + (ts.tv_sec - mInitTime) * 1E6;
}

unsigned long EmbrickBusHandler::millis() {
  // TODO Improve timing func. Maybe replace with existing forte implementation.
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return static_cast<unsigned long>(round(ts.tv_nsec / 1.0e6)) + (ts.tv_sec - mInitTime) * 1000;
}

void EmbrickBusHandler::microsleep(uint64_t paMicroseconds) {
  struct timespec ts;
  ts.tv_sec = (long) (paMicroseconds / (uint64_t) 1E6);
  ts.tv_nsec = (long) paMicroseconds * (long) 1E3 - ts.tv_sec * (long) 1E9;
  nanosleep(&ts, 0);
}

void EmbrickBusHandler::addTime(struct timespec& paTs, unsigned long paMicroseconds) {
  paTs.tv_sec += paMicroseconds / (unsigned long) 1E6;
  unsigned long t = paTs.tv_nsec + paMicroseconds * (unsigned long) 1E3
      - (paMicroseconds / (unsigned long) 1E6) * (unsigned long) 1E9;
  if (t >= (unsigned long) 1E9) {
    t -= (unsigned long) 1E9;
    paTs.tv_sec++;
  }
  paTs.tv_nsec = t;
}
