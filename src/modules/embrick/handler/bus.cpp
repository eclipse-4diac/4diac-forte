/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "bus.h"

#include <algorithm>
#include <slave/slave.h>
#include <slave/packages.h>

namespace EmBrick {

const char * const BusHandler::scmOK = "OK";
const char * const BusHandler::scmWaitingForInit =
    "Waiting for initialization..";
const char * const BusHandler::scmFailedToInit = "Failed to init BusHandler.";

DEFINE_SINGLETON(BusHandler)

BusHandler::BusHandler() :
    delegate(0), spi(0), slaveSelect(0), slaves(0), slaveCount(0) {
  // Set init time
  struct timespec ts;
  // TODO Check compile error. Had to to add rt libary to c++ make flags
  clock_gettime(CLOCK_MONOTONIC, &ts);

  initTime = ts.tv_sec;
  lastTransfer = micros();

  // Sync
  isReady = false;
  error = 0;

  // TODO Wrap pthread logic into helper class
  pthread_condattr_t loopCondAttr;
  pthread_condattr_init(&loopCondAttr);
  pthread_condattr_setclock(&loopCondAttr, CLOCK_MONOTONIC);

  pthread_cond_init(&loopCond, &loopCondAttr);

  pthread_condattr_destroy(&loopCondAttr);

  pthread_mutexattr_t loopMutexAttr;
  pthread_mutexattr_init(&loopMutexAttr);
  pthread_mutexattr_settype(&loopMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);

  pthread_mutex_init(&loopMutex, &loopMutexAttr);

  pthread_mutexattr_destroy(&loopMutexAttr);
}

BusHandler::~BusHandler() {
  pthread_cond_destroy(&loopCond);
  pthread_mutex_destroy(&loopMutex);
}

void BusHandler::run() {
  isReady = false;
  error = 0;

  // Init bus
  if (init()) {
    isReady = true;

    DEVLOG_INFO("emBrick[BusHandler]: Ready.\n");

    // Inform delegate
    if (delegate != 0)
      startNewEventChain(delegate);

    uint64_t ms;
    int rc, i;

    clock_gettime(CLOCK_MONOTONIC, &nextLoop);

    // Scheduling
    // TODO Combine slaves list and scheduling information
    // DISCUSS Speed of array and forte_list?
    TSlaveList::Iterator it = slaves->begin();
    sList = (struct SEntry**) forte_malloc(sizeof(struct SEntry*) * slaveCount);
    for (i = 0; i < slaveCount; i++) {
      sList[i] = (struct SEntry*) forte_malloc(sizeof(struct SEntry));
      sList[i]->slave = *it;
      sList[i]->nextDeadline = nextLoop;
      sList[i]->durationI = 0;
      sList[i]->durations[0] = 0;
      sList[i]->forced = false;

      ++it;
    }
    sNextI = 0;
    sNext = sList[sNextI];
    loopPending = false;
    SEntry *sCur = 0;
    int sCurI = 0;

    pthread_mutex_lock(&loopMutex);
    while (isAlive()) {
      pthread_cond_timedwait(&loopCond, &loopMutex, &sNext->nextDeadline);

      loopPending = true;
      sCur = sNext;
      sCurI = sNextI;
      pthread_mutex_unlock(&loopMutex);

      ms = micros();

      // Perform update on current slave
      if (!sCur->slave->update()) {
        error = "Update failed.";
        // Check for critical bus errors
        if (checkHandlerError() || hasError()) {
          // Inform delegate
          if (delegate != 0)
            startNewEventChain(delegate);
          break;
        }
      }

      // Set next deadline of current slave
      clock_gettime(CLOCK_MONOTONIC, &sCur->nextDeadline);
      // TODO Replace with dynamic configuration
      addTime(sCur->nextDeadline, sCurI == 0 ? 3000 : 40000);
      sCur->forced = false;


      sCur->durationI = (sCur->durationI + 1) % 5;
      sCur->durations[sCur->durationI] = micros() - ms;

//      DEVLOG_INFO("%d\n", sCurI);

      // Search for next deadline
      pthread_mutex_lock(&loopMutex);
      loopPending = false;

      for (i = 0; i < slaveCount; i++) {
        if (cmpTime(sList[i]->nextDeadline, sNext->nextDeadline)) {
          sNext = sList[i];
          sNextI = i;
        }
      }
    }
    pthread_mutex_unlock(&loopMutex);

    DEVLOG_INFO("emBrick[BusHandler]: Last loop durations:\n");
    for (uint8_t i = 0; i < slaveCount; i++)
      for (uint8_t j = 0; j < 5; j++)
        DEVLOG_INFO("%d - %d\n", i, sList[i]->durations[j]);
  } else {
    if (!hasError())
      error = scmFailedToInit;

    // Inform delegate
    if (delegate != 0)
      startNewEventChain(delegate);
  }

  isReady = false;

  // Free memory
  TSlaveList::Iterator itEnd(slaves->end());
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it)
    delete *it;
  delete slaves;
  for (int i = 0; i < slaveCount; i++)
    forte_free(sList[i]);
  forte_free(sList);
  delete spi;
  delete slaveSelect;

  DEVLOG_INFO("emBrick[BusHandler]: Stopped.\n");

  // TODO Remove statement
  // DISCUSS The thread init fails without the statement.
  // Is it possible that the function returns before the thread was stopped?
  while (isAlive())
    sleep(1);
}

bool BusHandler::init() {
  // Start handlers
  spi = new SPIHandler();
  slaveSelect = new PinHandler(49);
  slaves = new TSlaveList();

  // Check handlers
  if (checkHandlerError())
    return false;

  // Disable slave select -> reset all slaves
  slaveSelect->disable();

  // Wait for reset
  sleep(1);

  // Enable slave select -> the first slave waits for initialization
  slaveSelect->enable();

  // Wait for init
  microsleep(SyncGapDuration * 2);

  // Init the slaves sequentially. Abort if the init package is ignored -> no further slaves found.
  int slaveCounter = 1;
  int attempts = 0;
  Slave *slave = 0;

  do {
    slave = Slave::sendInit(slaveCounter);

    if (slave != 0) {
      slaves->push_back(slave);

      // Activate next slave by sending the 'SelectNextSlave' command to the current slave
      // It enables the slave select pin for the next slave on the bus
      transfer(slave->address, SelectNextSlave);

      slaveCounter++;
      attempts = 0;
    }

    microsleep(SyncGapDuration * 2);
  } while (++attempts < 3);

  slaveCount = slaveCounter - 1;

  // Increase the speed of the bus for data transfers
  spi->setSpeed(SPIHandler::MaxSpiSpeed);

  return true;
}

bool BusHandler::hasError() {
  return error != 0;
}

bool BusHandler::checkHandlerError() {
  if (spi->hasError()) {
    error = spi->error;
    return true;
  }

  if (slaveSelect->hasError()) {
    error = slaveSelect->error;
    return true;
  }

  return false;
}

const char* BusHandler::getStatus() {
  if (!isReady && error == 0)
    return scmWaitingForInit;

  if (hasError())
    return error;

  return scmOK;
}

Slave* BusHandler::getSlave(int index) {
  if (slaves == 0)
    return 0;

  TSlaveList::Iterator itEnd = slaves->end();
  int i = 0;
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it, i++)
    if (index == i)
      return *it;
  return 0;
}

void BusHandler::forceUpdate(int index) {
  pthread_mutex_lock(&loopMutex);

  if (sList[index]->forced) {
    pthread_mutex_unlock(&loopMutex);
    return;
  }

  sList[index]->forced = true;
  clock_gettime(CLOCK_MONOTONIC, &sList[index]->nextDeadline);
  if (!loopPending) {
    // Allow delay of unforced deadlines
    struct timespec ts = sNext->nextDeadline;
    addTime(ts, 10000); // TODO Replace with constant
    if (cmpTime(sList[index]->nextDeadline, ts)) {
      sNext = sList[index];
      sNextI = index;
    }

    // Force next loop
    pthread_cond_signal(&loopCond);
  }

  pthread_mutex_unlock(&loopMutex);
}

bool BusHandler::transfer(unsigned int target, Command cmd,
    unsigned char* dataSend, int dataSendLength, unsigned char* dataReceive,
    int dataReceiveLength, SlaveStatus* status, CSyncObject *syncMutex) {
  int dataLength = std::max(dataSendLength, dataReceiveLength + 1); // + 1 status byte

  unsigned int bufferLength = sizeof(Packages::Header) + dataLength + 1; // + 1 checksum byte
  if (bufferLength > TransferBufferLength) {
    DEVLOG_ERROR("emBrick[BusHandler]: Transfer buffer size exceeded.\n");
    return false;
  }

  memset(sendBuffer, 0, bufferLength);
  memset(recvBuffer, 0, bufferLength);

  Packages::Header* header = (Packages::Header*) sendBuffer;

  header->address = (char) target;
  header->command = cmd;
  header->checksum = calcChecksum((unsigned char*) header, 2);

  if (syncMutex)
    syncMutex->lock();
  memcpy(sendBuffer + sizeof(Packages::Header), dataSend, dataSendLength);
  if (syncMutex)
    syncMutex->unlock();
  sendBuffer[sizeof(Packages::Header) + dataSendLength] = calcChecksum(
      sendBuffer + sizeof(Packages::Header), dataSendLength);

//	DEVLOG_INFO("emBrick[BusHandler]: TX - %s\n",
//			bytesToHex(buffer, bufferLength).c_str());

  // Invert data of master
  for (unsigned int i = 0; i < bufferLength; i++)
    sendBuffer[i] = (unsigned char) ~sendBuffer[i];

  // Send and receive buffer via SPI
  int attempts = 3;
  int fails = 0;
  bool ok;
  uint64_t microTime;
  do {
    // Wait required microseconds between messages
    microTime = micros();
    if (lastTransfer + SyncGapDuration > microTime)
      microsleep(lastTransfer + SyncGapDuration - microTime);

    ok = spi->transfer(sendBuffer, recvBuffer, bufferLength);
    lastTransfer = micros();

    // Critical error of bus -> break immediately
    if (!ok)
      break;

//		DEVLOG_INFO("emBrick[BusHandler]: RX - %s, CS - %d\n",
//				bytesToHex(recvBuffer, bufferLength).c_str(),
//				calcChecksum(recvBuffer, bufferLength));

    ok = calcChecksum(recvBuffer + sizeof(Packages::Header),
        bufferLength - sizeof(Packages::Header)) == 0;
    if (!ok) {
      DEVLOG_DEBUG("emBrick[BusHandler]: Transfer - Invalid checksum\n");
    }
  } while (!ok && ++fails < attempts);

  if (!ok) {
    if (target != 0) {
      DEVLOG_DEBUG(
          "emBrick[BusHandler]: Failed to send command %d to slave %d.\n", cmd,
          target);
    }
    return false;
  }

  // Copy result
  if (syncMutex)
    syncMutex->lock();
  if (status)
    *status = (SlaveStatus) recvBuffer[sizeof(Packages::Header)];

  memcpy(dataReceive, recvBuffer + sizeof(Packages::Header) + 1,
      dataReceiveLength);
  if (syncMutex)
    syncMutex->unlock();

  return true;
}

unsigned char BusHandler::calcChecksum(unsigned char * data, int dataLen) {
  unsigned char c = 0;
  for (int i = 0; i < dataLen; i++)
    c += data[i];

  return ChecksumConstant - c;
}

void BusHandler::disableHandler() {

}

void BusHandler::enableHandler() {

}

void BusHandler::setPriority(int) {

}

int BusHandler::getPriority() const {
  return 0;
}

uint64_t BusHandler::micros() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e3) + (ts.tv_sec - initTime) * 1E6;
}

unsigned long BusHandler::millis() {
  // TODO Improve timing func. Maybe replace with existing forte implementation.
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e6) + (ts.tv_sec - initTime) * 1000;
}

void BusHandler::microsleep(unsigned long microseconds) {
  struct timespec ts;
  ts.tv_sec = microseconds / (unsigned long) 1E6;
  ts.tv_nsec = microseconds * 1E3 - ts.tv_sec * 1E9;
  nanosleep(&ts, 0);
}

void BusHandler::addTime(struct timespec& ts, unsigned long microseconds) {
  ts.tv_sec += microseconds / (unsigned long) 1E6;
  unsigned long t = ts.tv_nsec + microseconds * (unsigned long) 1E3
      - (microseconds / (unsigned long) 1E6) * (unsigned long) 1E9;
  if (t >= (unsigned long) 1E9) {
    t -= (unsigned long) 1E9;
    ts.tv_sec++;
  }
  ts.tv_nsec = t;
}

bool BusHandler::cmpTime(struct timespec& t1, struct timespec& t2) {
  return (t1.tv_nsec < t2.tv_nsec && t1.tv_sec == t2.tv_sec)
      || t1.tv_sec < t2.tv_sec;
}

std::string BusHandler::bytesToHex(unsigned char* bytes, int length) {
  // TODO Move helper functions to helper class or general forte utils class
  char buffer[length * 3];
  buffer[length * 3 - 1] = 0;
  for (int i = 0; i < length; i++)
    sprintf(&buffer[3 * i], "%02X ", bytes[i]);

  return buffer;
}

} /* namespace EmBrick */

