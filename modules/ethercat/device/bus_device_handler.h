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
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#pragma once

#include "handle.h"
#include "forte/io/mapper/io_mapper.h"
#include <vector>

namespace forte::eclipse4diac::io::ethercat {

	class ECBusHandler;

	class ECBusDeviceHandler {
		
		public:
			friend class ECBusHandler;

      enum DeviceStatus {
        NotInitialized = 0,
        Error = 1,
        OK = 2
      };

      enum DeviceType {
        ECDevice = 0,
        ECCoupler = 1,
        ECModule = 2
      };

      struct Config{
      };

      class Delegate {
        public:
          virtual void onDeviceStatus(DeviceStatus paStatus, DeviceStatus paOldStatus) = 0;
          virtual void onDeviceDestroy() = 0;
      };

      virtual void setConfig(Config* paConfig) = 0;
      Delegate *mDelegate;

      size_t index() const {
        return mDeviceIndex;
      }

      const DeviceType mDeviceType;

      uint16_t dataSendLength() const {
        return mDataSendLength;
      }

      uint16_t dataRecvLength() const {
        return mDataRecvLength;
      }

      void update(uint8_t *paECDomianPd);

      ECDeviceHandle *getInputHandle(size_t paIndex) {
        return getHandle(mInputs, paIndex);
      }

      ECDeviceHandle *getOutputHandle(size_t paIndex) {
        return getHandle(mOutputs, paIndex);
      }

      void addHandle(ECDeviceHandle *paHandle) {
        switch (paHandle->getDirection()){
          case forte::io::IOMapper::In: addHandle(mInputs, paHandle); break;
          case forte::io::IOMapper::Out: addHandle(mOutputs, paHandle); break;
          default: break;
        }
      }

      void dropHandles();

      std::vector<unsigned char> mUpdateSendImage;
      std::vector<unsigned char> mUpdateRecvImage;
      arch::CSyncObject mUpdateMutex;

      void initBuffer(uint16_t paDataSendLength, uint16_t paDataRecvLength);

    protected:
      size_t mDeviceIndex;
      
      ECBusDeviceHandler(ECBusHandler *paBus, DeviceType paDeviceType, size_t paDeviceIndex);
      virtual ~ECBusDeviceHandler();

      ECBusHandler *mBus;

      uint16_t mDataSendLength;
      uint16_t mDataRecvLength;
      DeviceStatus mStatus;
      DeviceStatus mOldStatus;
      std::vector<unsigned char> mUpdateRecvImageOld;

      arch::CSyncObject mHandleMutex;
      std::vector<ECDeviceHandle *> mInputs;
      std::vector<ECDeviceHandle *> mOutputs;
      void addHandle(std::vector<ECDeviceHandle *> &paList, ECDeviceHandle *paHandle);
      ECDeviceHandle *getHandle(std::vector<ECDeviceHandle *> &paList, size_t paIndex);

    private:
      //! declared but undefined copy constructor as we don't want devices to be directly copied.
      ECBusDeviceHandler(const ECBusDeviceHandler &);
      
	};
}