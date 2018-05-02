/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
#include <timespec_utils.h>

const char * const EmbrickBusHandler::scmSlaveUpdateFailed = "Update of slave failed.";
const char * const EmbrickBusHandler::scmNoSlavesFound = "No slave modules found.";

EmbrickBusHandler::EmbrickBusHandler(CDeviceExecution& paDeviceExecution) : forte::core::IO::IODeviceMultiController(paDeviceExecution),
    spi(0), slaveSelect(0), slaves(0), slaveCount(0), sList(0) {
  // Set init time
  struct timespec ts;
  // TODO Check compile error. Had to to add rt libary to c++ make flags
  clock_gettime(CLOCK_MONOTONIC, &ts);

  initTime = ts.tv_sec;
  lastTransfer = micros();

  // Default config
  config.BusInterface = 1;
  config.BusSelectPin = 49;
  config.BusInitSpeed = EmbrickSPIHandler::DefaultSpiSpeed;
  config.BusLoopSpeed = EmbrickSPIHandler::MaxSpiSpeed;
}

void EmbrickBusHandler::setConfig(struct forte::core::IO::IODeviceController::Config* config) {
  // Check if BusHandler is active -> configuration changes are not allowed
  if (isAlive()) {
    DEVLOG_ERROR(
        "emBrick[BusHandler]: Cannot change configuration while running.\n");
    return;
  }

  this->config = *static_cast<Config*>(config);
}

const char* EmbrickBusHandler::init() {
  // Start handlers
  spi = new EmbrickSPIHandler(config.BusInterface);
  slaveSelect = new EmbrickPinHandler(config.BusSelectPin);
  slaves = new TSlaveList();

  // Check handlers
  if (checkHandlerError()) {
    return error;
  }

  // Set SPI sped for initialization
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

  do {
    EmbrickSlaveHandler *slave = EmbrickSlaveHandler::sendInit(this, slaveCounter);

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
    return scmNoSlavesFound;
  }

  // Increase the speed of the bus for data transfers
  spi->setSpeed(config.BusLoopSpeed);

  return 0;
}

void EmbrickBusHandler::deInit() {
  // Free memory -> delete all slave instances and hardware handlers
  TSlaveList::Iterator itEnd(slaves->end());
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it)
    delete *it;
  delete slaves;
  slaves = 0;

  delete spi;
  delete slaveSelect;
}

forte::core::IO::IOHandle* EmbrickBusHandler::initHandle(
    forte::core::IO::IODeviceMultiController::HandleDescriptor *handleDescriptor) {
  HandleDescriptor desc = *static_cast<HandleDescriptor*>(handleDescriptor);

  EmbrickSlaveHandler *slave = getSlave(desc.slaveIndex);
  if (slave == 0)
    return 0;

  switch (desc.type) {
  case Bit:
    return new EmbrickBitSlaveHandle(this, desc.direction, desc.offset, desc.position,
        slave);
  case Analog:
    return new EmbrickAnalogSlaveHandle(this, desc.direction, desc.offset, slave);
  case Analog10:
    return new EmbrickAnalog10SlaveHandle(this, desc.direction, desc.offset, slave);
  }

  return 0;
}

void EmbrickBusHandler::prepareLoop() {
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
  sNext = sList[0];
  loopActive = false;
}

void EmbrickBusHandler::runLoop() {
  prepareLoop();

  // Init loop variables
  SEntry *sCur = 0;

  while (isAlive()) {
    // Sleep till next deadline is reached or loop is waked up

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if(timespecLessThan(&now, &sNext->nextDeadline)){ //only wait if the deadline is in the future
      struct timespec timeToSleep;
      timespecSub(&sNext->nextDeadline, &now, &timeToSleep);
      mForceLoop.timedWait(timeToSleep.tv_sec * 1E9 + timeToSleep.tv_nsec);
    }

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
    mSyncObject.unlock();

    uint64_t ms = micros();

    // Perform update on current slave
    if (-1 == sCur->slave->update()) {
      error = scmSlaveUpdateFailed;
      // Check for critical bus errors
      if (checkHandlerError() || hasError())
        break;
    }

    // Search for next deadline -> set lock to avoid changes of list
    mSyncObject.lock();
    loopActive = false;

    // Store update duration
    sCur->lastDuration = (uint16_t) (micros() - ms);

    // If current slave is forced again -> add update duration to deadline
    if (sCur->forced)
      addTime(sCur->nextDeadline, sCur->lastDuration);

    for (unsigned int i = 0; i < slaveCount; i++) {
      if (timespecLessThan(&sList[i]->nextDeadline, &sNext->nextDeadline)) {
        sNext = sList[i];
      }
    }
  }

  // Clean loop
  cleanLoop();
}

void EmbrickBusHandler::cleanLoop() {
  // Free memory of list
  for (int i = 0; i < slaveCount; i++)
    forte_free(sList[i]);
  forte_free(sList);
  sList = 0;
}

bool EmbrickBusHandler::checkHandlerError() {
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

EmbrickSlaveHandler* EmbrickBusHandler::getSlave(int index) {
  if (slaves == 0) {
    return 0;
  }

  TSlaveList::Iterator itEnd = slaves->end();
  int i = 0;
  for (TSlaveList::Iterator it = slaves->begin(); it != itEnd; ++it, i++)
    if (index == i) {
      return *it;
    }

  return 0;
}

void EmbrickBusHandler::forceUpdate(int index) {
  mSyncObject.lock();

  if (sList == 0 || slaveCount <= index || sList[index]->forced) {
    mSyncObject.unlock();
    return;
  }

  SEntry *e = sList[index];

  e->forced = true;
  clock_gettime(CLOCK_MONOTONIC, &e->nextDeadline);
  if (!loopActive) {
    if(!sNext->delayed && !sNext->forced) {
      struct timespec ts = e->nextDeadline;
      addTime(ts, e->lastDuration * 2);

      if(!timespecLessThan(&ts, &sNext->nextDeadline)) {
        sNext->delayed = true;
      }

      sNext = e;
    }

    // Force next loop
    mForceLoop.inc();
  }
  mSyncObject.unlock();
}

void EmbrickBusHandler::addSlaveHandle(int index, forte::core::IO::IOHandle* handle) {
  EmbrickSlaveHandler* slave = getSlave(index);
  if (slave == 0)
    return;

  slave->addHandle((EmbrickSlaveHandle*) handle);
}

void EmbrickBusHandler::dropSlaveHandles(int index) {
  EmbrickSlaveHandler* slave = getSlave(index);
  if (slave == 0)
    return;

  slave->dropHandles();
}

bool EmbrickBusHandler::isSlaveAvailable(int index) {
  return getSlave(index) != 0;
}

bool EmbrickBusHandler::checkSlaveType(int index, int type) {
  EmbrickSlaveHandler* slave = getSlave(index);
  if (slave == 0)
    return false;

  return slave->type == type;
}

bool EmbrickBusHandler::transfer(unsigned int target, Command cmd,
    unsigned char* dataSend, int dataSendLength, unsigned char* dataReceive,
    int dataReceiveLength, EmbrickSlaveHandler::SlaveStatus* status, CSyncObject *syncMutex) {
  unsigned int dataLength = std::max(dataSendLength, dataReceiveLength + 1); // + 1 status byte

  unsigned int bufferLength = sizeof(EmbrickHeaderPackage) + dataLength + 1; // + 1 checksum byte
  if (bufferLength > TransferBufferLength) {
    DEVLOG_ERROR("emBrick[BusHandler]: Transfer buffer size exceeded.\n");
    return false;
  }

  memset(sendBuffer, 0, bufferLength);
  memset(recvBuffer, 0, bufferLength);

  // Prepare header
  EmbrickHeaderPackage* header = (EmbrickHeaderPackage*) sendBuffer;

  header->address = (char) target;
  header->command = cmd;
  header->checksum = calcChecksum((unsigned char*) header, 2);

  if (syncMutex)
    syncMutex->lock();
  memcpy(sendBuffer + sizeof(EmbrickHeaderPackage), dataSend, dataSendLength);
  if (syncMutex)
   syncMutex->unlock();
  sendBuffer[sizeof(EmbrickHeaderPackage) + dataSendLength] = calcChecksum(
      sendBuffer + sizeof(EmbrickHeaderPackage), dataSendLength);

  // Invert data of master
  for (unsigned int i = 0; i < bufferLength; i++)
    sendBuffer[i] = (unsigned char) ~sendBuffer[i];

  // Send and receive buffer via SPI
  int attempts = 3;
  int fails = 0;
  bool ok;
  do {
    // Wait required microseconds between messages
    uint64_t microTime = micros();
    if (lastTransfer + SyncGapDuration > microTime)
      microsleep(lastTransfer + (uint64_t) SyncGapDuration - microTime);

//    // Send header
//    ok = spi->transfer(sendBuffer, recvBuffer, sizeof(EmbrickHeaderPackage));
//    lastTransfer = micros();
//    if (!ok) {
//      DEVLOG_ERROR("emBrick[BusHandler]: Failed to transfer header buffer.\n");
//      break;
//    }
//
//    // Wait required microseconds between messages
//    microTime = micros();
//    if (lastTransfer + 4 > microTime)
//      microsleep(lastTransfer + 4 - microTime);
//
//    // Send data
//    ok = spi->transfer(sendBuffer + sizeof(EmbrickHeaderPackage),
//        recvBuffer + sizeof(EmbrickHeaderPackage),
//        bufferLength - sizeof(EmbrickHeaderPackage));
//    lastTransfer = micros();
//    if (!ok) {
//      DEVLOG_ERROR("emBrick[BusHandler]: Failed to transfer data buffer.\n");
//      break;
//    }
    ok = spi->transfer(sendBuffer, recvBuffer, bufferLength);
    lastTransfer = micros();

    // Critical error of bus -> break immediately
    if (!ok)
      break;

    // Validate checksum
    ok = calcChecksum(recvBuffer + sizeof(EmbrickHeaderPackage),
        bufferLength - sizeof(EmbrickHeaderPackage)) == 0;
    if (!ok) {
//      DEVLOG_DEBUG("emBrick[BusHandler]: Transfer - Invalid checksum\n");
    }
  } while (!ok && ++fails < attempts);

  // Check if command was transmitted successfully
  if (!ok) {
    if (target != 0 && cmd != Data) {
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
  *status = (EmbrickSlaveHandler::SlaveStatus) recvBuffer[sizeof(EmbrickHeaderPackage)];

  memcpy(dataReceive, recvBuffer + sizeof(EmbrickHeaderPackage) + 1,
         dataReceiveLength);
  if (syncMutex)
    syncMutex->unlock();

  return true;
}

unsigned char EmbrickBusHandler::calcChecksum(unsigned char * data, int dataLen) {
  unsigned char c = 0;
  for (int i = 0; i < dataLen; i++)
    c = static_cast<unsigned char>(c + data[i]);

  return static_cast<unsigned char>(ChecksumConstant - c);
}

uint64_t EmbrickBusHandler::micros() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return round(ts.tv_nsec / 1.0e3) + (ts.tv_sec - initTime) * 1E6;
}

unsigned long EmbrickBusHandler::millis() {
  // TODO Improve timing func. Maybe replace with existing forte implementation.
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return static_cast<unsigned long>(round(ts.tv_nsec / 1.0e6)) + (ts.tv_sec - initTime) * 1000;
}

void EmbrickBusHandler::microsleep(uint64_t microseconds) {
  struct timespec ts;
  ts.tv_sec = (long) (microseconds / (uint64_t) 1E6);
  ts.tv_nsec = (long) microseconds * (long) 1E3 - ts.tv_sec * (long) 1E9;
  nanosleep(&ts, 0);
}

void EmbrickBusHandler::addTime(struct timespec& ts, unsigned long microseconds) {
  ts.tv_sec += microseconds / (unsigned long) 1E6;
  unsigned long t = ts.tv_nsec + microseconds * (unsigned long) 1E3
      - (microseconds / (unsigned long) 1E6) * (unsigned long) 1E9;
  if (t >= (unsigned long) 1E9) {
    t -= (unsigned long) 1E9;
    ts.tv_sec++;
  }
  ts.tv_nsec = t;
}
