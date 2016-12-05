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
#include <thread.h>
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

namespace EmBrick {

enum Command {
  Init = 2, SelectNextSlave = 3, Data = 10
};

const unsigned int SyncGapMultiplicator = 15;
const unsigned int SyncGapDuration = (SyncGapMultiplicator - 1) * 32 + 10;

class BusHandler: public CExternalEventHandler, public CThread {
DECLARE_SINGLETON(BusHandler)

  friend class Slave;

public:
  void init();

protected:
  bool transfer(unsigned int target, Command cmd, unsigned char* dataSend =
  NULL, int dataSendLength = 0, unsigned char* dataReceive = NULL,
      int dataReceiveLength = 0, SlaveStatus* status = NULL);
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

  unsigned long nextLoop;
  uint64_t lastTransfer;

  // Handlers
  SPIHandler *spi;
  PinHandler *slaveSelect;

  // Slaves
  typedef CSinglyLinkedList<Slave *> TSlaveList;
  TSlaveList *slaves;

private:
  uint64_t micros();
  unsigned long millis();
  time_t initTime;
  void microsleep(unsigned long microseconds);
  unsigned char calcChecksum(unsigned char * data, int dataLen);

  std::string bytesToHex(unsigned char* bytes, int length);

  static const unsigned char ChecksumConstant = 0x55;
};

}

#endif /* SRC_MODULES_EMBRICK_BUSCONTROLLER_H_ */
