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
#include <conditionSync.h>

#include <io/device/io_controller_multi.h>

const unsigned int TransferBufferLength = 150;
const unsigned int SyncGapMultiplicator = 15;
const unsigned int SyncGapDuration = (SyncGapMultiplicator - 1) * 32 + 10;

class EmbrickBusHandler: public IODeviceMultiController {
  friend class EmbrickSlaveHandler;

public:
  explicit EmbrickBusHandler(CDeviceExecution& paDeviceExecution);

  enum Command {
    /**
     * Initializes the slave and assigns an individual address to it.
     * The slave provides hardware specifications and length information for the data exchange.
     * The master sends configuration parameter to synchronize transfer timings.
     */
    Init = 2,

    /**
     * It enables the slave select signal of the addressed slave.
     * The subsequent slave waits for initialization.
     */
    SelectNextSlave = 3,

    /**
     * It sets and gets the current state, called process image, of the slave.
     * The amount of exchanged data bytes depends on the functionality of the slave.
     * If a slave, for example, has 8 digital outputs, the master sends 1 Byte (1 Bit for each digital output) to set the slave state.
     * Likewise if a slave reads 8 analog values, the slave sends 16 Bytes (2 Bytes for each value) to the master.
     * The structure of the process image is specified in the emBRICK products manual.
     * The Data command should be performed at least 20 times per second.
     */
    Data = 10,
  };

  struct Config: IODeviceController::Config {
    unsigned int BusInterface; //!< Selects the SPI interface for the brickBUS. The default value is 1 (selects SPI1).
    unsigned int BusSelectPin; //!< Sets the pin, which is connect to the slave select pin of the brickBUS.
    unsigned long BusInitSpeed; //!< Sets the SPI speed for the brickBUS during the initialization of the slaves. The default value is 300000 Hz.
    unsigned long BusLoopSpeed; //!< Sets the maximal SPI speed for the brickBUS during the runtime updates of the slaves. The default value is 700000 Hz.
  };

  enum HandleType {
    Bit, Analog, Analog10
  };

  struct HandleDescriptor: IODeviceMultiController::HandleDescriptor {
    HandleType type;
    uint8_t offset;
    uint8_t position;

    HandleDescriptor(CIEC_WSTRING const &id, IOMapper::Direction direction,
        int slaveIndex, HandleType type, uint8_t offset,
        uint8_t position) :
        IODeviceMultiController::HandleDescriptor(id, direction,
            slaveIndex), type(type), offset(offset), position(position) {

    }
  };

  void setConfig(struct IODeviceController::Config* config);

  EmbrickSlaveHandler* getSlave(int index);
  void forceUpdate(int index);

  void addSlaveHandle(int index, IOHandle* handle);
  void dropSlaveHandles(int index);
protected:
  const char* init();
  void deInit();

  IOHandle* initHandle(
      IODeviceMultiController::HandleDescriptor *handleDescriptor);

  void prepareLoop();
  virtual void runLoop();
  void cleanLoop();

  bool transfer(unsigned int target, Command cmd, unsigned char* dataSend =
                  NULL, int dataSendLength = 0, unsigned char* dataReceive = NULL,
                int dataReceiveLength = 0, EmbrickSlaveHandler::SlaveStatus* status = NULL,
                CSyncObject *syncMutex = NULL);
  bool broadcast(Command cmd, unsigned char* dataSend =
                   NULL, int dataSendLength = 0, unsigned char* dataReceive = NULL,
                 int dataReceiveLength = 0) {
    return transfer(0, cmd, dataSend, dataSendLength, dataReceive,
                    dataReceiveLength);
  }

  // Config
  struct Config config;

  // Timing variables
  struct timespec lastLoop;
  struct timespec nextLoop;
  uint64_t lastTransfer;

  // Handlers
  EmbrickSPIHandler *spi;
  EmbrickPinHandler *slaveSelect;

  // Slaves
  typedef CSinglyLinkedList<EmbrickSlaveHandler *> TSlaveList;
  TSlaveList *slaves;
  int slaveCount;

  // Sync
  bool loopActive;
  Utils::ConditionSync loopSync;

  // Error
  bool checkHandlerError();

  // Scheduling
  struct SEntry {
    EmbrickSlaveHandler* slave;
    struct timespec nextDeadline;
    uint16_t lastDuration;
    bool forced;
    bool delayed;
  };
  struct SEntry **sList;
  SEntry *sNext;
  int sNextIndex;

private:
  bool isSlaveAvailable(int index);
  bool checkSlaveType(int index, int type);

  uint64_t micros();
  unsigned long millis();
  time_t initTime;
  void microsleep(uint64_t microseconds);
  void addTime(struct timespec& t, unsigned long microseconds);
  bool cmpTime(struct timespec& t1, struct timespec& t2);

  unsigned char calcChecksum(unsigned char * data, int dataLen);

  unsigned char sendBuffer[TransferBufferLength];
  unsigned char recvBuffer[TransferBufferLength];

  static const unsigned char ChecksumConstant = 0x55;

  static const char * const scmSlaveUpdateFailed;
  static const char * const scmNoSlavesFound;
};

#endif /* SRC_MODULES_EMBRICK_BUSCONTROLLER_H_ */
