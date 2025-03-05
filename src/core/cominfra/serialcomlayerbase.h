/*******************************************************************************
 * Copyright (c) 2017 ACIN, fortiss GmbH
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

#include "comlayer.h"
#include "../datatypes/forte_string.h"
#include "core/util/parameterParser.h"
#include <criticalregion.h>

template <typename TSerialHandle, TSerialHandle nullHandle = static_cast<TSerialHandle>(0) >
class CSerialComLayerBase : public forte::com_infra::CComLayer{
  public:

    typedef TSerialHandle TSerialHandleType;

    CSerialComLayerBase(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    ~CSerialComLayerBase() override;

    forte::com_infra::EComResponse processInterrupt() override;

    /*! \brief Perform send to serial interface
    *   \param paData Sendable payload
    *   \param paSize Payload size in bytes
    *
    *   \return ComLayer response
    */
    forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize) override = 0;

    /*! \brief Perform reading from serial interface
     *
     * @return if not e_Nothing something was read and the FB should get an external event
     */
    forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize) override = 0;

    void closeConnection() override = 0;

  protected:

    enum EForteSerialBaudRate{
      e50 = 50,
      e75 = 75,
      e110 = 100,
      e134C5 = 134, //134.5
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

    enum EForteSerialByteSize{
      e5 = 5,
      e6 = 6,
      e7 = 7,
      e8 = 8,
      e9 = 9,
    };

    enum EForteSerialStopBits {
      eOneBit = 1,
      eTwoBits = 2,
      eOne5Bits
    };

    enum EForteSerialParity {
      eNoParity = 0,
      eODD = 1,
      eEven = 2,
      eMark,
      eSpace
    };

    struct SSerialParameters{
      CIEC_STRING interfaceName;
      EForteSerialBaudRate baudRate;
      EForteSerialByteSize byteSize;
      EForteSerialStopBits stopBits;
      EForteSerialParity parity;
    } ;

    char mTerminationSymbol[3]; //**< Space for CR, LF, or CR/LF + Terminating \0
    forte::com_infra::EComResponse openConnection(char *paLayerParameter) override;
    virtual forte::com_infra::EComResponse openSerialConnection(const SSerialParameters& paSerialParameters, TSerialHandle* paHandleResult) = 0;
    static const unsigned int mMaxRecvBuffer = 1000;

    forte::com_infra::EComResponse mInterruptResp;
    char mRecvBuffer[mMaxRecvBuffer];
    unsigned int mBufFillSize;
    CSyncObject mRecvLock;

    TSerialHandle mSerialHandle;
    TSerialHandle getSerialHandler(){
      return mSerialHandle;
    }

  private:

    enum EForteSerialCommunicationParameter{
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


template <typename TThreadHandle, TThreadHandle nullHandle>
CSerialComLayerBase<TThreadHandle, nullHandle>::CSerialComLayerBase(forte::com_infra::CComLayer* paUpperLayer,
    forte::com_infra::CBaseCommFB * paFB) :
    forte::com_infra::CComLayer(paUpperLayer, paFB), mInterruptResp(forte::com_infra::e_Nothing), mBufFillSize(0), mSerialHandle(nullHandle) {
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer)); //TODO change this to  mRecvBuffer{0} in the extended list when fully switching to C++11
  memset(mTerminationSymbol, 0, sizeof(mTerminationSymbol)); //TODO change this to  mTerminationSymbol{0} in the extended list when fully switching to C++11
}

template <typename TThreadHandle, TThreadHandle nullHandle>
CSerialComLayerBase<TThreadHandle, nullHandle>::~CSerialComLayerBase() = default;

template <typename TThreadHandle, TThreadHandle nullHandle>
forte::com_infra::EComResponse CSerialComLayerBase<TThreadHandle, nullHandle>::processInterrupt(){
  if(forte::com_infra::e_ProcessDataOk == mInterruptResp){
    CCriticalRegion lock(mRecvLock);
    switch (mConnectionState){
      case forte::com_infra::e_Connected:
        if(nullptr != mTopLayer){
          mInterruptResp = mTopLayer->recvData(mRecvBuffer, mBufFillSize);
          mBufFillSize = 0;
        }
        break;
      case forte::com_infra::e_Disconnected:
      case forte::com_infra::e_Listening:
      case forte::com_infra::e_ConnectedAndListening:
      default:
        break;
    }
  }
  return mInterruptResp;
}

template <typename TThreadHandle, TThreadHandle nullHandle>
forte::com_infra::EComResponse CSerialComLayerBase<TThreadHandle, nullHandle>::openConnection(char *paLayerParameter)  {
  //Create Serial Com Handle
  CParameterParser parser(paLayerParameter, ',', mNoOfParameters);
  if(mNoOfParameters != parser.parseParameters()){
    return forte::com_infra::e_InitInvalidId;
  }

  SSerialParameters parsedParameters;
  parsedParameters.interfaceName = CIEC_STRING(parser[CSerialComLayerBase::eInterface], strlen(parser[CSerialComLayerBase::eInterface]));

  //Check baud rate setting
  parsedParameters.baudRate = static_cast<EForteSerialBaudRate>(atoi(parser[CSerialComLayerBase::eBaudrate]));
  switch (parsedParameters.baudRate) {
  //These are ok baud rates
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
    case e1000000: break;
    //all other numbers are invalid!
    default: return forte::com_infra::e_InitInvalidId; break;
  }

  //Check byte size setting
  parsedParameters.byteSize = static_cast<EForteSerialByteSize>(atoi(parser[CSerialComLayerBase::eByteSize]));
  if(4 > parsedParameters.byteSize || 8 < parsedParameters.byteSize){
    return forte::com_infra::e_InitInvalidId;
  }

  //Check stopbits setting
  if(0 == strcmp(parser[CSerialComLayerBase::eStopBits], "1")){
    parsedParameters.stopBits = eOneBit;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eStopBits], "1.5")){
    parsedParameters.stopBits = eOne5Bits;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eStopBits], "2")){
    parsedParameters.stopBits = eTwoBits;
  }
  else{
    return forte::com_infra::e_InitInvalidId;
  }

  //Check parity setting
  if(0 == strcmp(parser[CSerialComLayerBase::eParity], "NONE")){
    parsedParameters.parity = eNoParity;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eParity], "ODD")){
    parsedParameters.parity = eODD;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eParity], "EVEN")){
    parsedParameters.parity = eEven;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eParity], "MARK")){
    parsedParameters.parity = eMark;
  }
  else if(0 == strcmp(parser[CSerialComLayerBase::eParity], "SPACE")){
    parsedParameters.parity = eSpace;
  }
  else{
    return forte::com_infra::e_InitInvalidId;
  }

  if(0 == strcmp("$n", parser[eTerminationSymbol])){
    strcpy(mTerminationSymbol, "\n");
  }
  else if(0 == strcmp("$r", parser[CSerialComLayerBase::eTerminationSymbol])){
    strcpy(mTerminationSymbol, "\r");
  }
  else if(0 == strcmp("$r$n", parser[CSerialComLayerBase::eTerminationSymbol])){
    strcpy(mTerminationSymbol, "\r\n");
  }
  else{
    return forte::com_infra::e_InitInvalidId;
  }

  forte::com_infra::EComResponse resp = openSerialConnection(parsedParameters, &mSerialHandle);
  if(forte::com_infra::e_InitOk == resp){
    mConnectionState = forte::com_infra::e_Connected;
  }
  return resp;
}
