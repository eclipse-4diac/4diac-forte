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

#ifndef SRC_MODULES_EMBRICK_BUSCONTROLLER_H_
#define SRC_MODULES_EMBRICK_BUSCONTROLLER_H_

#include <extevhan.h>
#include <devlog.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <timerha.h>
#include <string>
#include <pthread.h>
#include <fortelist.h>
#include "spi.h"
#include "pin.h"
#include <slave/slave.h>
#include <forte_sync.h>
#include <forte_thread.h>
#include <utils/conditionSync.h>

namespace EmBrick {

enum Command {
  Init = 2, SelectNextSlave = 3, Data = 10
};

const unsigned int TransferBufferLength = 150;
const unsigned int SyncGapMultiplicator = 15;
const unsigned int SyncGapDuration = (SyncGapMultiplicator - 1) * 32 + 10;

class BusHandler: public CExternalEventHandler, public CThread {
DECLARE_SINGLETON(BusHandler)

  friend class Slave;

public:
  struct Config {
    unsigned int BusInterface;
    unsigned long BusInitSpeed;
    unsigned long BusLoopSpeed;
  };

  void setConfig(struct Config config);
  CEventSourceFB *delegate;

  bool hasError();
  const char* getStatus();

  Slave* getSlave(int index);
  void forceUpdate(int index);
protected:
  bool init();

  void prepareLoop();
  void runLoop();
  void cleanLoop();

  bool transfer(unsigned int target, Command cmd, unsigned char* dataSend =
  NULL, int dataSendLength = 0, unsigned char* dataReceive = NULL,
      int dataReceiveLength = 0, SlaveStatus* status = NULL,
      CSyncObject *syncMutex = NULL);
  bool broadcast(Command cmd, unsigned char* dataSend =
  NULL, int dataSendLength = 0, unsigned char* dataReceive = NULL,
      int dataReceiveLength = 0) {
    return transfer(0, cmd, dataSend, dataSendLength, dataReceive,
        dataReceiveLength);
  }

  virtual void run();

  // Functions needed for the external event handler interface
  void enableHandler(void);
  void disableHandler(void);
  void setPriority(int paPriority);
  int getPriority(void) const;

  // Config
  struct Config config;

  // Timing variables
  struct timespec lastLoop;
  struct timespec nextLoop;
  uint64_t lastTransfer;

  // Handlers
  SPIHandler *spi;
  PinHandler *slaveSelect;

  // Slaves
  typedef CSinglyLinkedList<Slave *> TSlaveList;
  TSlaveList *slaves;
  int slaveCount;

  // Sync
  bool isReady;
  bool loopActive;
  ConditionSync loopSync;

  // Error
  const char* error;
  bool checkHandlerError();

  // Scheduling
  struct SEntry {
    Slave* slave;
    struct timespec nextDeadline;
    uint16_t lastDuration;
    bool forced;
    bool delayed;
  };
  struct SEntry **sList;
  SEntry *sNext;
  int sNextIndex;

private:
  uint64_t micros();
  unsigned long millis();
  time_t initTime;
  void microsleep(unsigned long microseconds);
  void addTime(struct timespec& t, unsigned long microseconds);
  bool cmpTime(struct timespec& t1, struct timespec& t2);

  unsigned char calcChecksum(unsigned char * data, int dataLen);

  unsigned char sendBuffer[TransferBufferLength];
  unsigned char recvBuffer[TransferBufferLength];

  static const unsigned char ChecksumConstant = 0x55;

  static const char * const scmOK;
  static const char * const scmWaitingForInit;
  static const char * const scmFailedToInit;
  static const char * const scmSlaveUpdateFailed;
  static const char * const scmNoSlavesFound;
};

}

#endif /* SRC_MODULES_EMBRICK_BUSCONTROLLER_H_ */
