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

#pragma once

#include <forte/io/device/io_controller_multi.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <pthread.h>
#include "spi.h"
#include "pin.h"
#include "../slave/slave.h"
#include "forte/arch/forte_sync.h"
#include "forte/arch/forte_sem.h"
#include "forte/arch/forte_thread.h"

namespace forte::eclipse4diac::io::embrick {

  const unsigned int TransferBufferLength = 150;
  const unsigned int SyncGapMultiplicator = 15;

  class EmbrickBusHandler : public forte::io::IODeviceMultiController {
      friend class EmbrickSlaveHandler;

    public:
      explicit EmbrickBusHandler(CDeviceExecution &paDeviceExecution);

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
         * If a slave, for example, has 8 digital outputs, the master sends 1 Byte (1 Bit for each digital output) to
         * set the slave state. Likewise if a slave reads 8 analog values, the slave sends 16 Bytes (2 Bytes for each
         * value) to the master. The structure of the process image is specified in the emBRICK products manual. The
         * Data command should be performed at least 20 times per second.
         */
        Data = 10,
      };

      struct Config : IODeviceController::Config {
          unsigned int
              mBusInterface; //!< Selects the SPI interface for the brickBUS. The default value is 1 (selects SPI1).
          unsigned int mBusSelectPin; //!< Sets the pin, which is connect to the slave select pin of the brickBUS.
          unsigned long mBusInitSpeed; //!< Sets the SPI speed for the brickBUS during the initialization of the slaves.
                                       //!< The default value is 300000 Hz.
          unsigned long mBusLoopSpeed; //!< Sets the maximal SPI speed for the brickBUS during the runtime updates of
                                       //!< the slaves. The default value is 700000 Hz.
      };

      enum HandleType { Bit, Analog, Analog10 };

      class HandleDescriptor : public IODeviceMultiController::HandleDescriptor {
        public:
          HandleType mType;
          uint8_t mOffset;
          uint8_t mPosition;

          HandleDescriptor(std::string const &paId,
                           forte::io::IOMapper::Direction paDirection,
                           size_t paSlaveIndex,
                           HandleType paType,
                           uint8_t paOffset,
                           uint8_t paPosition) :
              IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex),
              mType(paType),
              mOffset(paOffset),
              mPosition(paPosition) {
          }
      };

      void setConfig(struct IODeviceController::Config *paConfig) override;

      EmbrickSlaveHandler *getSlave(size_t paIndex);
      void forceUpdate(size_t paIndex);

      void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;
      void dropSlaveHandles(size_t paIndex) override;

    protected:
      const char *init() override;
      void deInit() override;

      forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) override;

      void prepareLoop();
      void runLoop() override;
      void cleanLoop();

      bool transfer(unsigned int paTarget,
                    Command paCmd,
                    unsigned char *paDataSend = nullptr,
                    size_t paDataSendLength = 0,
                    unsigned char *paDataReceive = nullptr,
                    size_t paDataReceiveLength = 0,
                    EmbrickSlaveHandler::SlaveStatus *paStatus = nullptr,
                    arch::CSyncObject *paSyncMutex = nullptr);
      bool broadcast(Command paCmd,
                     unsigned char *paDataSend = nullptr,
                     size_t paDataSendLength = 0,
                     unsigned char *paDataReceive = nullptr,
                     size_t paDataReceiveLength = 0) {
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

      // Devices
      std::vector<EmbrickSlaveHandler *> mDevices;

      // Sync
      bool mLoopActive;
      arch::CSemaphore mForceLoop;
      arch::CSyncObject mSyncObject;

      // Error
      bool checkHandlerError();

      // Scheduling
      struct SEntry {
          EmbrickSlaveHandler *mSlave;
          struct timespec mNextDeadline;
          uint16_t mLastDuration;
          bool mForced;
          bool mDelayed;
      };
      struct SEntry **mSList;
      SEntry *mSNext;

    private:
      bool isSlaveAvailable(size_t paIndex) override;
      bool checkSlaveType(size_t paIndex, int paType) override;

      uint64_t micros();
      unsigned long millis();
      time_t mInitTime;
      void microsleep(uint64_t paMicroseconds);
      void addTime(struct timespec &paTs, unsigned long paMicroseconds);

      unsigned char calcChecksum(unsigned char *paData, int paDataLen);

      unsigned char mSendBuffer[TransferBufferLength];
      unsigned char mRecvBuffer[TransferBufferLength];

      static const unsigned char scmChecksumConstant = 0x55;
  };
} // namespace forte::eclipse4diac::io::embrick
