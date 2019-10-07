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
#include <forte_sem.h>
#include <forte_thread.h>

#include <io/device/io_controller_multi.h>

const unsigned int TransferBufferLength = 150;
const unsigned int SyncGapMultiplicator = 15;
const unsigned int SyncGapDuration = (SyncGapMultiplicator - 1) * 32 + 10;

class EmbrickBusHandler : public forte::core::io::IODeviceMultiController {
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

    struct Config : forte::core::io::IODeviceController::Config {
        unsigned int mBusInterface; //!< Selects the SPI interface for the brickBUS. The default value is 1 (selects SPI1).
        unsigned int mBusSelectPin; //!< Sets the pin, which is connect to the slave select pin of the brickBUS.
        unsigned long mBusInitSpeed; //!< Sets the SPI speed for the brickBUS during the initialization of the slaves. The default value is 300000 Hz.
        unsigned long mBusLoopSpeed; //!< Sets the maximal SPI speed for the brickBUS during the runtime updates of the slaves. The default value is 700000 Hz.
    };

    enum HandleType {
      Bit,
      Analog,
      Analog10
    };

    class HandleDescriptor : public forte::core::io::IODeviceMultiController::HandleDescriptor {
      public:
        HandleType mType;
        uint8_t mOffset;
        uint8_t mPosition;

        HandleDescriptor(CIEC_WSTRING const &paId, forte::core::io::IOMapper::Direction paDirection, int paSlaveIndex, HandleType paType, uint8_t paOffset,
            uint8_t paPosition) :
            forte::core::io::IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex), mType(paType), mOffset(paOffset), mPosition(paPosition) {

        }
    };

    void setConfig(struct forte::core::io::IODeviceController::Config* paConfig);

    EmbrickSlaveHandler* getSlave(int paIndex);
    void forceUpdate(int paIndex);

    void addSlaveHandle(int paIndex, forte::core::io::IOHandle* paHandle);
    void dropSlaveHandles(int paIndex);
  protected:
    const char* init();
    void deInit();

    forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor);

    void prepareLoop();
    virtual void runLoop();
    void cleanLoop();

    bool transfer(unsigned int paTarget, Command paCmd, unsigned char* paDataSend =
    NULL, int paDataSendLength = 0, unsigned char* paDataReceive = NULL, int paDataReceiveLength = 0, EmbrickSlaveHandler::SlaveStatus* paStatus = NULL,
        CSyncObject *paSyncMutex = NULL);
    bool broadcast(Command paCmd, unsigned char* paDataSend =
    NULL, int paDataSendLength = 0, unsigned char* paDataReceive = NULL, int paDataReceiveLength = 0) {
      return transfer(0, paCmd, paDataSend, paDataSendLength, paDataReceive, paDataReceiveLength);
    }

    // Config
    struct Config mConfig;

    // Timing variables
    struct timespec mLastLoop;
    struct timespec mNextLoop;
    uint64_t mLastTransfer;

    // Handlers
    EmbrickSPIHandler *mSpi;
    EmbrickPinHandler *mSlaveSelect;

    // Slaves
    typedef CSinglyLinkedList<EmbrickSlaveHandler *> TSlaveList;
    TSlaveList *mSlaves;
    int mSlaveCount;

    // Sync
    bool mLoopActive;
    CSemaphore mForceLoop;
    CSyncObject mSyncObject;

    // Error
    bool checkHandlerError();

    // Scheduling
    struct SEntry {
        EmbrickSlaveHandler* mSlave;
        struct timespec mNextDeadline;
        uint16_t mLastDuration;
        bool mForced;
        bool mDelayed;
    };
    struct SEntry **mSList;
    SEntry *mSNext;

  private:
    bool isSlaveAvailable(int paIndex);
    bool checkSlaveType(int paIndex, int paType);

    uint64_t micros();
    unsigned long millis();
    time_t mInitTime;
    void microsleep(uint64_t paMicroseconds);
    void addTime(struct timespec& paTs, unsigned long paMicroseconds);

    unsigned char calcChecksum(unsigned char * paData, int paDataLen);

    unsigned char mSendBuffer[TransferBufferLength];
    unsigned char mRecvBuffer[TransferBufferLength];

    static const unsigned char scmChecksumConstant = 0x55;

    static const char * const scmSlaveUpdateFailed;
    static const char * const scmNoSlavesFound;
};

#endif /* SRC_MODULES_EMBRICK_BUSCONTROLLER_H_ */
