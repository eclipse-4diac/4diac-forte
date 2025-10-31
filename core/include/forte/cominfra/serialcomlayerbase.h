/*******************************************************************************
 * Copyright (c) 2017, 2025 ACIN, fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/cominfra/comlayer.h"
#include "forte/datatypes/forte_string.h"
#include "forte/util/parameterParser.h"
#include "forte/util/criticalregion.h"

namespace forte::arch {
  template<typename TSerialHandle, TSerialHandle nullHandle = static_cast<TSerialHandle>(0)>
  class CSerialComLayerBase : public com_infra::CComLayer {
    public:
      typedef TSerialHandle TSerialHandleType;

      CSerialComLayerBase(CComLayer *paUpperLayer, com_infra::CBaseCommFB *paFB);
      ~CSerialComLayerBase() override;

      com_infra::EComResponse processInterrupt() override;

      /*! \brief Perform send to serial interface
       *   \param paData Sendable payload
       *   \param paSize Payload size in bytes
       *
       *   \return ComLayer response
       */
      com_infra::EComResponse sendData(void *paData, unsigned int paSize) override = 0;

      /*! \brief Perform reading from serial interface
       *
       * @return if not e_Nothing something was read and the FB should get an external event
       */
      com_infra::EComResponse recvData(const void *paData, unsigned int paSize) override = 0;

      void closeConnection() override = 0;

    protected:
      enum EForteSerialBaudRate {
        e50 = 50,
        e75 = 75,
        e110 = 100,
        e134C5 = 134, // 134.5
        e150 = 150,
        e200 = 200,
        e300 = 300,
        e600 = 600,
        e1200 = 1200,
        e1800 = 1800,
        e2400 = 2400,
        e4800 = 4800,
        e9600 = 9600,
        e14400 = 14400,
        e19200 = 19200,
        e38400 = 38400,
        e57600 = 57600,
        e115200 = 115200,
        e128000 = 128000,
        e256000 = 256000,
        e1000000 = 1000000,
      };

      enum EForteSerialByteSize {
        e5 = 5,
        e6 = 6,
        e7 = 7,
        e8 = 8,
        e9 = 9,
      };

      enum EForteSerialStopBits { eOneBit = 1, eTwoBits = 2, eOne5Bits };

      enum EForteSerialParity { eNoParity = 0, eODD = 1, eEven = 2, eMark, eSpace };

      struct SSerialParameters {
          CIEC_STRING mInterfaceName;
          EForteSerialBaudRate mBaudRate;
          EForteSerialByteSize mByteSize;
          EForteSerialStopBits mStopBits;
          EForteSerialParity mParity;
      };

      char mTerminationSymbol[3]; //**< Space for CR, LF, or CR/LF + Terminating \0
      com_infra::EComResponse openConnection(char *paLayerParameter) override;
      virtual com_infra::EComResponse openSerialConnection(const SSerialParameters &paSerialParameters,
                                                           TSerialHandle *paHandleResult) = 0;
      static const unsigned int mMaxRecvBuffer = 1000;

      com_infra::EComResponse mInterruptResp;
      char mRecvBuffer[mMaxRecvBuffer];
      unsigned int mBufFillSize;
      CSyncObject mRecvLock;

      TSerialHandle mSerialHandle;
      TSerialHandle getSerialHandler() {
        return mSerialHandle;
      }

    private:
      enum EForteSerialCommunicationParameter {
        eInterface = 0,
        eBaudrate,
        eByteSize,
        eStopBits,
        eParity,
        eTerminationSymbol,
        eSerComParamterAmount
      };

      static const unsigned int mNoOfParameters = eSerComParamterAmount;
  };

  template<typename TThreadHandle, TThreadHandle nullHandle>
  CSerialComLayerBase<TThreadHandle, nullHandle>::CSerialComLayerBase(CComLayer *paUpperLayer,
                                                                      com_infra::CBaseCommFB *paFB) :
      CComLayer(paUpperLayer, paFB),
      mInterruptResp(com_infra::e_Nothing),
      mBufFillSize(0),
      mSerialHandle(nullHandle) {
    memset(
        mRecvBuffer, 0,
        sizeof(mRecvBuffer)); // TODO change this to  mRecvBuffer{0} in the extended list when fully switching to C++11
    memset(mTerminationSymbol, 0, sizeof(mTerminationSymbol)); // TODO change this to  mTerminationSymbol{0} in the
    // extended list when fully switching to C++11
  }

  template<typename TThreadHandle, TThreadHandle nullHandle>
  CSerialComLayerBase<TThreadHandle, nullHandle>::~CSerialComLayerBase() = default;

  template<typename TThreadHandle, TThreadHandle nullHandle>
  com_infra::EComResponse CSerialComLayerBase<TThreadHandle, nullHandle>::processInterrupt() {
    if (com_infra::e_ProcessDataOk == mInterruptResp) {
      util::CCriticalRegion lock(mRecvLock);
      switch (mConnectionState) {
        case com_infra::e_Connected:
          if (nullptr != mTopLayer) {
            mInterruptResp = mTopLayer->recvData(mRecvBuffer, mBufFillSize);
            mBufFillSize = 0;
          }
          break;
        case com_infra::e_Disconnected:
        case com_infra::e_Listening:
        case com_infra::e_ConnectedAndListening:
        default: break;
      }
    }
    return mInterruptResp;
  }

  template<typename TThreadHandle, TThreadHandle nullHandle>
  com_infra::EComResponse CSerialComLayerBase<TThreadHandle, nullHandle>::openConnection(char *paLayerParameter) {
    // Create Serial Com Handle
    util::CParameterParser parser(paLayerParameter, ',', mNoOfParameters);
    if (mNoOfParameters != parser.parseParameters()) {
      return com_infra::e_InitInvalidId;
    }

    SSerialParameters parsedParameters;
    parsedParameters.mInterfaceName = CIEC_STRING(parser[eInterface], strlen(parser[eInterface]));

    // Check baud rate setting
    parsedParameters.mBaudRate = static_cast<EForteSerialBaudRate>(atoi(parser[eBaudrate]));
    switch (parsedParameters.mBaudRate) {
      // These are ok baud rates
      case e50: break;
      case e75: break;
      case e110: break;
      case e134C5: break;
      case e150: break;
      case e200: break;
      case e300: break;
      case e600: break;
      case e1200: break;
      case e1800: break;
      case e2400: break;
      case e4800: break;
      case e9600: break;
      case e14400: break;
      case e19200: break;
      case e38400: break;
      case e57600: break;
      case e115200: break;
      case e128000: break;
      case e256000: break;
      case e1000000:
        break;
        // all other numbers are invalid!
      default: return com_infra::e_InitInvalidId; break;
    }

    // Check byte size setting
    parsedParameters.mByteSize = static_cast<EForteSerialByteSize>(atoi(parser[eByteSize]));
    if (4 > parsedParameters.mByteSize || 8 < parsedParameters.mByteSize) {
      return com_infra::e_InitInvalidId;
    }

    // Check stopbits setting
    if (0 == strcmp(parser[eStopBits], "1")) {
      parsedParameters.mStopBits = eOneBit;
    } else if (0 == strcmp(parser[eStopBits], "1.5")) {
      parsedParameters.mStopBits = eOne5Bits;
    } else if (0 == strcmp(parser[eStopBits], "2")) {
      parsedParameters.mStopBits = eTwoBits;
    } else {
      return com_infra::e_InitInvalidId;
    }

    // Check parity setting
    if (0 == strcmp(parser[eParity], "NONE")) {
      parsedParameters.mParity = eNoParity;
    } else if (0 == strcmp(parser[eParity], "ODD")) {
      parsedParameters.mParity = eODD;
    } else if (0 == strcmp(parser[eParity], "EVEN")) {
      parsedParameters.mParity = eEven;
    } else if (0 == strcmp(parser[eParity], "MARK")) {
      parsedParameters.mParity = eMark;
    } else if (0 == strcmp(parser[eParity], "SPACE")) {
      parsedParameters.mParity = eSpace;
    } else {
      return com_infra::e_InitInvalidId;
    }

    if (0 == strcmp("$n", parser[eTerminationSymbol])) {
      strcpy(mTerminationSymbol, "\n");
    } else if (0 == strcmp("$r", parser[eTerminationSymbol])) {
      strcpy(mTerminationSymbol, "\r");
    } else if (0 == strcmp("$r$n", parser[eTerminationSymbol])) {
      strcpy(mTerminationSymbol, "\r\n");
    } else {
      return com_infra::e_InitInvalidId;
    }

    com_infra::EComResponse resp = openSerialConnection(parsedParameters, &mSerialHandle);
    if (com_infra::e_InitOk == resp) {
      mConnectionState = com_infra::e_Connected;
    }
    return resp;
  }
} // namespace forte::arch
