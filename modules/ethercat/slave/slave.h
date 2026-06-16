/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include "handle.h"
#include "forte/io/mapper/io_mapper.h"

namespace forte::eclipse4diac::io::ethercat {

	class ECBusHandler;

	class ECSlaveHandler {
		
		public:
			friend class ECBusHandler;

      enum SlaveStatus {
        NotInitialized = 0,
        Error = 1,
        OK = 2
      };

      enum SlaveType {
        ECSlave = 0,
        ECCoupler = 1,
        ECModule = 2
      };

      struct Config{
      };

      class Delegate {
        public:
          virtual void onSlaveStatus(SlaveStatus paStatus, SlaveStatus paOldStatus) = 0;
          virtual void onSlaveDestroy() = 0;
      };

      virtual void setConfig(Config* paConfig) = 0;
      Delegate *mDelegate;

      size_t index() const {
        return mSlaveIndex;
      }

      const SlaveType mSlaveType;

      uint16_t dataSendLength() const {
        return mDataSendLength;
      }

      uint16_t dataRecvLength() const {
        return mDataRecvLength;
      }

      void update(uint8_t *paECDomianPd);

      ECSlaveHandle *getInputHandle(size_t paIndex) {
        return getHandle(mInputs, paIndex);
      }

      ECSlaveHandle *getOutputHandle(size_t paIndex) {
        return getHandle(mOutputs, paIndex);
      }

      void addHandle(ECSlaveHandle *paHandle) {
        switch (paHandle->getDirection()){
          case forte::io::IOMapper::In: addHandle(mInputs, paHandle); break;
          case forte::io::IOMapper::Out: addHandle(mOutputs, paHandle); break;
          default: break;
        }
      }

      void dropHandles();

      unsigned char *mUpdateSendImage;
      unsigned char *mUpdateRecvImage;
      arch::CSyncObject mUpdateMutex;

      void initBuffer(uint16_t paDataSendLength, uint16_t paDataRecvLength);

    protected:
      size_t mSlaveIndex;
      
      ECSlaveHandler(ECBusHandler *paBus, SlaveType paSlaveType, size_t paSlaveIndex);
      virtual ~ECSlaveHandler();

      ECBusHandler *mBus;

      uint16_t mDataSendLength;
      uint16_t mDataRecvLength;
      SlaveStatus mStatus;
      SlaveStatus mOldStatus;
      unsigned char *mUpdateRecvImageOld;

      arch::CSyncObject mHandleMutex;
      std::vector<ECSlaveHandle *> mInputs;
      std::vector<ECSlaveHandle *> mOutputs;
      void addHandle(std::vector<ECSlaveHandle *> &paList, ECSlaveHandle *paHandle);
      ECSlaveHandle *getHandle(std::vector<ECSlaveHandle *> &paList, size_t paIndex);

    private:
      //! declared but undefined copy constructor as we don't want Slaves to be directly copied.
      ECSlaveHandler(const ECSlaveHandler &);
      
	};
}