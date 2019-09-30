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

#include "serialcomlayerbase.h"
#include "../utils/parameterParser.h"
#include <criticalregion.h>

template <typename TThreadHandle, TThreadHandle nullHandle>
CSerialComLayerBase<TThreadHandle, nullHandle>::CSerialComLayerBase(forte::com_infra::CComLayer* paUpperLayer,
    forte::com_infra::CBaseCommFB * paFB) :
    forte::com_infra::CComLayer(paUpperLayer, paFB), mInterruptResp(forte::com_infra::e_Nothing), mBufFillSize(0), mSerialHandle(nullHandle) {
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer)); //TODO change this to  mRecvBuffer{0} in the extended list when fully switching to C++11
  memset(mTerminationSymbol, 0, sizeof(mTerminationSymbol)); //TODO change this to  mTerminationSymbol{0} in the extended list when fully switching to C++11
}

template <typename TThreadHandle, TThreadHandle nullHandle>
CSerialComLayerBase<TThreadHandle, nullHandle>::~CSerialComLayerBase() {
}

template <typename TThreadHandle, TThreadHandle nullHandle>
forte::com_infra::EComResponse CSerialComLayerBase<TThreadHandle, nullHandle>::processInterrupt(){
  if(forte::com_infra::e_ProcessDataOk == mInterruptResp){
    CCriticalRegion lock(mRecvLock);
    switch (m_eConnectionState){
      case forte::com_infra::e_Connected:
        if(0 != m_poTopLayer){
          mInterruptResp = m_poTopLayer->recvData(mRecvBuffer, mBufFillSize);
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
  parsedParameters.interfaceName = parser[CSerialComLayerBase::eInterface];

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
    m_eConnectionState = forte::com_infra::e_Connected;
  }
  return resp;
}
