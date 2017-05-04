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
namespace Handlers {

const char * const Bus::scmOK = "OK";
const char * const Bus::scmWaitingForInit =
    "Waiting for initialization..";
const char * const Bus::scmFailedToInit = "Failed to init BusHandler.";
const char * const Bus::scmSlaveUpdateFailed = "Update of slave failed.";
const char * const Bus::scmNoSlavesFound = "No slave modules found.";

DEFINE_SINGLETON(Bus)

Bus::Bus() :
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
  sList = 0;

  // Default config
  config.BusInterface = 1;
  config.BusSelectPin = 49;
  config.BusInitSpeed = SPI::DefaultSpiSpeed;
  config.BusLoopSpeed = SPI::MaxSpiSpeed;
}

Bus::~Bus() {
}

void Bus::setConfig(struct Config config) {
  // Check if BusHandler is active -> config changes are not allowed
  if (isAlive()) {
    DEVLOG_ERROR("emBrick[BusHandler]: Cannot change config while running.\n");
    return;
  }

  this->config = config;
}

void Bus::run() {
  isReady = false;
  error = 0;

  // Init bus
  if (init()) {
    isReady = true;

    DEVLOG_INFO("emBrick[BusHandler]: Ready.\n");

    // Inform delegate about successful startup
    if (delegate != 0)
      startNewEventChain(delegate);

    // Lock loop before the start
    loopSync.lock();

    // Prepare loop
    prepareLoop();

    // Run main loop -> function is blocking
    runLoop();

    // Clean loop
    cleanLoop();

    // Release loop after the end
    loopSync.unlock();
  } else {
    // Handler occured during initialization. If no error is present, set general error
    if (!hasError())
      error = scmFailedToInit;

    // Inform delegate about error
    if (delegate != 0)
      startNewEventChain(delegate);
  }

  isReady = false;

  // Free memory -> delete all slave instances and hardware handlers
  TSlaveList::Iterator itEnd(slaves->end());
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it)
    delete *it;
  delete slaves;
  delete spi;
  delete slaveSelect;

  DEVLOG_INFO("emBrick[BusHandler]: Stopped.\n");

  // TODO Remove statement
  // DISCUSS The thread init fails without the statement.
  // Is it possible that the function returns before the thread was stopped?
  while (isAlive())
    sleep(1);
}

bool Bus::init() {
  // Start handlers
  spi = new SPI(config.BusInterface);
  slaveSelect = new Pin(config.BusSelectPin);
  slaves = new TSlaveList();

  // Check handlers
  if (checkHandlerError())
    return false;

  // Set SPI sped for intialization
  spi->setSpeed(config.BusInitSpeed);

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

  if (slaveCount == 0) {
    error = scmNoSlavesFound;
    return false;
  }

  // Increase the speed of the bus for data transfers
  spi->setSpeed(config.BusLoopSpeed);

  return true;
}

void Bus::prepareLoop() {
  // Get current time
  clock_gettime(CLOCK_MONOTONIC, &nextLoop);

  // Scheduling
  // TODO Combine slaves list and scheduling information
  // DISCUSS Speed of array and forte_list?
  TSlaveList::Iterator it = slaves->begin();
  sList = (struct SEntry**) forte_malloc(sizeof(struct SEntry*) * slaveCount);
  for (int i = 0; i < slaveCount; i++) {
    sList[i] = (struct SEntry*) forte_malloc(sizeof(struct SEntry));
    sList[i]->slave = *it;
    sList[i]->nextDeadline = nextLoop;
    sList[i]->lastDuration = 0;
    sList[i]->forced = false;
    sList[i]->delayed = false;

    ++it;
  }
  sNextIndex = 0;
  sNext = sList[sNextIndex];
  loopActive = false;
}

void Bus::runLoop() {
  // Init loop variables
  SEntry *sCur = 0;

  uint64_t ms;
  int i, res;

  while (isAlive()) {
    // Sleep till next deadline is reached or loop is waked up
    loopSync.waitUntil(sNext->nextDeadline);

    // Set current slave
    loopActive = true;
    sCur = sNext;

    // Set next deadline of current slave
    clock_gettime(CLOCK_MONOTONIC, &sCur->nextDeadline);
    addTime(sCur->nextDeadline, 1000000 / sCur->slave->config.UpdateInterval);

    // Remove delayed and forced flag
    sCur->forced = false;
    sCur->delayed = false;

    // Remove lock during blocking operation -> allows forced update interrupts
    loopSync.unlock();

    ms = micros();

    // Perform update on current slave
    res = sCur->slave->update();
    if (res == -1) {
      error = scmSlaveUpdateFailed;
      // Check for critical bus errors
      if (checkHandlerError() || hasError()) {
        // Inform delegate
        if (delegate != 0)
          startNewEventChain(delegate);
        break;
      }
    }

    // Search for next deadline -> set lock to avoid changes of list
    loopSync.lock();
    loopActive = false;

    // Store update duration
    sCur->lastDuration = (uint16_t) (micros() - ms);

    // If current slave is forced again -> add update duration to deadline
    if (sCur->forced)
      addTime(sCur->nextDeadline, sCur->lastDuration);

    for (i = 0; i < slaveCount; i++) {
      if (cmpTime(sList[i]->nextDeadline, sNext->nextDeadline)) {
        sNext = sList[i];
        sNextIndex = i;
      }
    }
  }

}

void Bus::cleanLoop() {
  // Free memory of list
  for (int i = 0; i < slaveCount; i++)
    forte_free(sList[i]);
  forte_free(sList);
  sList = 0;
}

bool Bus::hasError() {
  return error != 0;
}

bool Bus::checkHandlerError() {
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

const char* Bus::getStatus() {
  if (!isReady && error == 0)
    return scmWaitingForInit;

  if (hasError())
    return error;

  return scmOK;
}

Slave* Bus::getSlave(int index) {
  if (slaves == 0)
    return 0;

  TSlaveList::Iterator itEnd = slaves->end();
  int i = 0;
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it, i++)
    if (index == i)
      return *it;
  return 0;
}

void Bus::forceUpdate(int index) {
  loopSync.lock();

  if (sList == 0 || slaveCount <= index || sList[index]->forced) {
    loopSync.unlock();
    return;
  }

  SEntry *e = sList[index];

  e->forced = true;
  clock_gettime(CLOCK_MONOTONIC, &e->nextDeadline);
  if (!loopActive) {
    if(!sNext->delayed && !sNext->forced) {
      struct timespec ts = e->nextDeadline;
      addTime(ts, e->lastDuration * 2);

      if(!cmpTime(ts, sNext->nextDeadline)) {
        sNext->delayed = true;
      }

      sNext = e;
      sNextIndex = index;
    }

    // Force next loop
    loopSync.wakeUp();
  }

  loopSync.unlock();
}

bool Bus::transfer(unsigned int target, Command cmd,
    unsigned char* dataSend, int dataSendLength, unsigned char* dataReceive,
    int dataReceiveLength, SlaveStatus* status, CSyncObject *syncMutex) {
  unsigned int dataLength = std::max(dataSendLength, dataReceiveLength + 1); // + 1 status byte

  unsigned int bufferLength = sizeof(Packages::Header) + dataLength + 1; // + 1 checksum byte
  if (bufferLength > TransferBufferLength) {
    DEVLOG_ERROR("emBrick[BusHandler]: Transfer buffer size exceeded.\n");
    return false;
  }

  memset(sendBuffer, 0, bufferLength);
  memset(recvBuffer, 0, bufferLength);

  // Prepare header
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

//    // Send header
//    ok = spi->transfer(sendBuffer, recvBuffer, sizeof(Packages::Header));
//    lastTransfer = micros();
//    if (!ok) {
//      DEVLOG_ERROR("emBrick[BusHandler]: Failed to transfer buffer.\n");
//      break;
//    }
//
//    // Send data
//    ok = spi->transfer(sendBuffer + sizeof(Packages::Header),
//        recvBuffer + sizeof(Packages::Header),
//        bufferLength - sizeof(Packages::Header));
//    lastTransfer = micros();
//    if (!ok) {
//      DEVLOG_ERROR("emBrick[BusHandler]: Failed to transfer buffer.\n");
//      break;
//    }
    ok = spi->transfer(sendBuffer, recvBuffer, bufferLength);
    lastTransfer = micros();

    // Critical error of bus -> break immediately
    if (!ok)
      break;

    // Validate checksum
    ok = calcChecksum(recvBuffer + sizeof(Packages::Header),
        bufferLength - sizeof(Packages::Header)) == 0;
    if (!ok) {
      DEVLOG_DEBUG("emBrick[BusHandler]: Transfer - Invalid checksum\n");
    }
  } while (!ok && ++fails < attempts);

  // Check if command was transmitted successfully
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

unsigned char Bus::calcChecksum(unsigned char * data, int dataLen) {
  unsigned char c = 0;
  for (int i = 0; i < dataLen; i++)
    c += data[i];

  return ChecksumConstant - c;
}

void Bus::disableHandler() {

}

void Bus::enableHandler() {

}

void Bus::setPriority(int) {

}

int Bus::getPriority() const {
  return 0;
}

uint64_t Bus::micros() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e3) + (ts.tv_sec - initTime) * 1E6;
}

unsigned long Bus::millis() {
  // TODO Improve timing func. Maybe replace with existing forte implementation.
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e6) + (ts.tv_sec - initTime) * 1000;
}

void Bus::microsleep(unsigned long microseconds) {
  struct timespec ts;
  ts.tv_sec = microseconds / (unsigned long) 1E6;
  ts.tv_nsec = microseconds * 1E3 - ts.tv_sec * 1E9;
  nanosleep(&ts, 0);
}

void Bus::addTime(struct timespec& ts, unsigned long microseconds) {
  ts.tv_sec += microseconds / (unsigned long) 1E6;
  unsigned long t = ts.tv_nsec + microseconds * (unsigned long) 1E3
      - (microseconds / (unsigned long) 1E6) * (unsigned long) 1E9;
  if (t >= (unsigned long) 1E9) {
    t -= (unsigned long) 1E9;
    ts.tv_sec++;
  }
  ts.tv_nsec = t;
}

bool Bus::cmpTime(struct timespec& t1, struct timespec& t2) {
  return (t1.tv_nsec < t2.tv_nsec && t1.tv_sec == t2.tv_sec)
      || t1.tv_sec < t2.tv_sec;
}

} /* namespace Handlers */
} /* namespace EmBrick */

