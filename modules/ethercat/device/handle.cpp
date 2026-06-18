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

#include "handle.h"
#include "bus_device_handler.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/io/mapper/io_mapper.h"

namespace forte::eclipse4diac::io::ethercat {
  ECDeviceHandle::ECDeviceHandle(forte::io::IODeviceController *paController,
                                forte::io::IOMapper::Direction paDirection,
                                CIEC_ANY::EDataTypeID paType,
                                uint8_t paOffset,
                                const std::string &paHandleId,
                                ECBusDeviceHandler &paDevice) :
      IOHandle(paController, paDirection, paType),
      mOffset(paOffset),
      mECDomainDataOffset(0),
      mDevice(paDevice),
      mUpdateMutex(mDevice.mUpdateMutex),
      mHandleId(paHandleId) {
    if(paDirection == forte::io::IOMapper::In){
      mBuffer = std::span<unsigned char>(mDevice.mUpdateRecvImage.data(), mDevice.mUpdateRecvImage.size());
    } else if(paDirection == forte::io::IOMapper::Out) {
      mBuffer = std::span<unsigned char>(mDevice.mUpdateSendImage.data(), mDevice.mUpdateSendImage.size());
    }

    switch (paType) {
      case CIEC_ANY::EDataTypeID::e_BYTE:
        mByteLength = 1;
        break;
      case CIEC_ANY::EDataTypeID::e_WORD:
        mByteLength = 2;
        break;
      case CIEC_ANY::EDataTypeID::e_DWORD:
        mByteLength = 4;
        break;
      case CIEC_ANY::EDataTypeID::e_LWORD:
        mByteLength = 8;
        break;
      default:
        break;
    }

  }

  ECDeviceHandle::~ECDeviceHandle() = default;

  void ECDeviceHandle::syncDomainData(uint8_t *paECDomainData) {
    uint8_t *ecDomainData = paECDomainData + mECDomainDataOffset;
    if(isInput()) {
      memcpy(mBuffer.data() + mOffset, ecDomainData, mByteLength);
    } else if(isOutput()) {
      memcpy(ecDomainData, mBuffer.data() + mOffset, mByteLength);
    }
  }

  void ECDeviceHandle::set(const CIEC_ANY &paValue) {
    switch (mType) {
      case CIEC_ANY::EDataTypeID::e_BYTE: {
        uint8_t value = static_cast<const CIEC_BYTE &>(paValue);
        memcpy(mBuffer.data() + mOffset, &value, sizeof(uint8_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_WORD: {
        uint16_t value = static_cast<const CIEC_WORD &>(paValue);
        uint16_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer.data() + mOffset, &littleEndValue, sizeof(uint16_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_DWORD: {
        uint32_t value = static_cast<const CIEC_DWORD &>(paValue);
        uint32_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer.data() + mOffset, &littleEndValue, sizeof(uint32_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_LWORD: {
        uint64_t value = static_cast<const CIEC_LWORD &>(paValue);
        uint64_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer.data() + mOffset, &littleEndValue, sizeof(uint64_t));
      }
      break;
      default:
        DEVLOG_ERROR("ethercat[ECDeviceHandle]:Unsupported data type for set.\n");
      break;
    }
  }

  void ECDeviceHandle::get(CIEC_ANY &paValue) {
    switch (mType){
      case CIEC_ANY::EDataTypeID::e_BYTE:
        static_cast<CIEC_BYTE &>(paValue) = getByteValue(mBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_WORD:
        static_cast<CIEC_WORD &>(paValue) = getWordValue(mBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_DWORD:
        static_cast<CIEC_DWORD &>(paValue) = getDWordValue(mBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_LWORD:
        static_cast<CIEC_LWORD &>(paValue) = getLWordValue(mBuffer);
        break;
      default:
        DEVLOG_ERROR("ethercat[ECDeviceHandle]:Unsupported data type for get.\n");
        break;
    }
  }

  bool ECDeviceHandle::equal(std::span<const unsigned char> paOldBuffer) {
    bool result = false;

    switch (mType){
      case CIEC_ANY::EDataTypeID::e_BYTE:
        result = getByteValue(mBuffer) == getByteValue(paOldBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_WORD:
        result = getWordValue(mBuffer) == getWordValue(paOldBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_DWORD:
        result = getDWordValue(mBuffer) == getDWordValue(paOldBuffer);
        break;
      case CIEC_ANY::EDataTypeID::e_LWORD:
        result = getLWordValue(mBuffer) == getLWordValue(paOldBuffer);
        break;
      default:
        result = false;
        DEVLOG_ERROR("ethercat(ECDeviceHandle):Unsupported type for equal.\n");
        break;
    }

    return result;
  }

  const CIEC_BYTE ECDeviceHandle::getByteValue(std::span<const unsigned char> paBuffer){
    return CIEC_BYTE(paBuffer[mOffset]);
  }

  const CIEC_WORD ECDeviceHandle::getWordValue(std::span<const unsigned char> paBuffer){
    uint16_t value;
    memcpy(&value, paBuffer.data() + mOffset, sizeof(uint16_t));
    uint16_t hostValue = littleEndianToHost(value);   
    return CIEC_WORD(hostValue);
  }

  const CIEC_DWORD ECDeviceHandle::getDWordValue(std::span<const unsigned char> paBuffer){
    uint32_t value;
    memcpy(&value, paBuffer.data() + mOffset, sizeof(uint32_t));
    uint32_t hostValue = littleEndianToHost(value);  
    return CIEC_DWORD(hostValue);
  }

  const CIEC_LWORD ECDeviceHandle::getLWordValue(std::span<const unsigned char> paBuffer){
    uint64_t value;
    memcpy(&value, paBuffer.data() + mOffset, sizeof(uint64_t));
    uint64_t hostValue = littleEndianToHost(value);
    return CIEC_LWORD(hostValue);
  }

  void ECDeviceHandle::onObserver(forte::io::IOObserver *paObserver) {
    reset();
    IOHandle::onObserver(paObserver);
  }

  void ECDeviceHandle::dropObserver() {
    IOHandle::dropObserver();
    reset();
  }
}
