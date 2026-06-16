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
#include "slave.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/io/mapper/io_mapper.h"

namespace forte::eclipse4diac::io::ethercat {
  ECSlaveHandle::ECSlaveHandle(forte::io::IODeviceController *paController,
                                forte::io::IOMapper::Direction paDirection,
                                CIEC_ANY::EDataTypeID paType,
                                uint8_t paOffset,
                                const std::string &paHandleId,
                                ECSlaveHandler *paSlave) :
      IOHandle(paController, paDirection, paType),
      mOffset(paOffset),
      mSlave(paSlave),
      mECDomainDataOffset(0),
      mUpdateMutex(&mSlave->mUpdateMutex),
      mHandleId(paHandleId) {
    if(paDirection == forte::io::IOMapper::In){
      mBuffer = mSlave->mUpdateRecvImage;
    } else if(paDirection == forte::io::IOMapper::Out) {
      mBuffer = mSlave->mUpdateSendImage;
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

  ECSlaveHandle::~ECSlaveHandle() = default;

  void ECSlaveHandle::syncDomainData(uint8_t *paECDomainData) {
    uint8_t *ecDomainData = paECDomainData + mECDomainDataOffset;
    if(isInput()) {
      memcpy(mBuffer + mOffset, ecDomainData, mByteLength);
    } else if(isOutput()) {
      memcpy(ecDomainData, mBuffer + mOffset, mByteLength);
    }
  }

  void ECSlaveHandle::set(const CIEC_ANY &paValue) {
    switch (mType) {
      case CIEC_ANY::EDataTypeID::e_BYTE: {
        uint8_t value = static_cast<const CIEC_BYTE &>(paValue);
        memcpy(mBuffer + mOffset, &value, sizeof(uint8_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_WORD: {
        uint16_t value = static_cast<const CIEC_WORD &>(paValue);
        uint16_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer + mOffset, &littleEndValue, sizeof(uint16_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_DWORD: {
        uint32_t value = static_cast<const CIEC_DWORD &>(paValue);
        uint32_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer + mOffset, &littleEndValue, sizeof(uint32_t));
      }
      break;
      case CIEC_ANY::EDataTypeID::e_LWORD: {
        uint64_t value = static_cast<const CIEC_LWORD &>(paValue);
        uint64_t littleEndValue = hostToLittleEndian(value);
        memcpy(mBuffer + mOffset, &littleEndValue, sizeof(uint64_t));
      }
      break;
      default:
        DEVLOG_ERROR("ethercat[ECSlaveHandle]:Unsupported data type for set.\n");
      break;
    }
  }

  void ECSlaveHandle::get(CIEC_ANY &paValue) {
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
        DEVLOG_ERROR("ethercat[ECSlaveHandle]:Unsupported data type for get.\n");
        break;
    }
  }

  bool ECSlaveHandle::equal(unsigned char *paOldBuffer) {
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
        DEVLOG_ERROR("ethercat(ECSlaveHandle):Unsupported type for equal.\n");
        break;
    }

    return result;
  }

  const CIEC_BYTE ECSlaveHandle::getByteValue(const unsigned char* paBuffer){
    uint8_t *p = (uint8_t*)(paBuffer + mOffset);
    return CIEC_BYTE(*p);
  }

  const CIEC_WORD ECSlaveHandle::getWordValue(const unsigned char* paBuffer){
    uint16_t value;
    memcpy(&value, paBuffer + mOffset, sizeof(uint16_t));
    uint16_t hostValue = littleEndianToHost(value);   
    return CIEC_WORD(hostValue);
  }

  const CIEC_DWORD ECSlaveHandle::getDWordValue(const unsigned char* paBuffer){
    uint32_t value;
    memcpy(&value, paBuffer + mOffset, sizeof(uint32_t));
    uint32_t hostValue = littleEndianToHost(value);  
    return CIEC_DWORD(hostValue);
  }

  const CIEC_LWORD ECSlaveHandle::getLWordValue(const unsigned char* paBuffer){
    uint64_t value;
    memcpy(&value, paBuffer + mOffset, sizeof(uint64_t));
    uint64_t hostValue = littleEndianToHost(value);
    return CIEC_LWORD(hostValue);
  }

  void ECSlaveHandle::onObserver(forte::io::IOObserver *paObserver) {
    reset();
    IOHandle::onObserver(paObserver);
  }

  void ECSlaveHandle::dropObserver() {
    IOHandle::dropObserver();
    reset();
  }
}