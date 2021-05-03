/*******************************************************************************
 * Copyright (c) 2013, 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "cwin32sercomlayer.h"
#include "cwin32sercomhandler.h"
#include "../../../core/cominfra/commfb.h"

CWin32SerComLayer::CWin32SerComLayer(forte::com_infra::CComLayer* paUpperLayer,
    forte::com_infra::CBaseCommFB* paFB) :
    CSerialComLayerBase(paUpperLayer, paFB){
}

CWin32SerComLayer::~CWin32SerComLayer() {
  closeConnection();
}

forte::com_infra::EComResponse CWin32SerComLayer::recvData(const void *, unsigned int )  {
  mInterruptResp = forte::com_infra::e_Nothing;

  DWORD dwBytesRead = 0;
  if(ReadFile(static_cast<HANDLE>(mSerialHandle), mRecvBuffer, mMaxRecvBuffer - 1, &dwBytesRead, NULL)){ //TODO: Failure handling and send INITO-
    if(0 < dwBytesRead){
      mBufFillSize = dwBytesRead;
      mInterruptResp = forte::com_infra::e_ProcessDataOk;
      m_poFb->interruptCommFB(this);
    }
    else{
      mInterruptResp = forte::com_infra::e_ProcessDataRecvFaild;
    }
  }
  return mInterruptResp;
}

forte::com_infra::EComResponse CWin32SerComLayer::sendData(void *paData, unsigned int paSize)
{
  DWORD dwBytesWritten= 0;
  char *pcData = static_cast<char*> (paData);
  unsigned int nToBeSent = paSize;
  //Send payload
  if(!WriteFile(static_cast<HANDLE>(mSerialHandle), pcData, nToBeSent, &dwBytesWritten, NULL))
  {
   return forte::com_infra::e_ProcessDataSendFailed;
  }
  if(dwBytesWritten != nToBeSent)
  {
    return forte::com_infra::e_ProcessDataSendFailed;
  }

  //Send termination symbol(s)
  if (!WriteFile(static_cast<HANDLE>(mSerialHandle), mTerminationSymbol, strlen(mTerminationSymbol), &dwBytesWritten, NULL))
  {
    return forte::com_infra::e_ProcessDataSendFailed;
  }
  if (strlen(mTerminationSymbol) != dwBytesWritten)
  {
    return forte::com_infra::e_ProcessDataSendFailed;
  }

  return forte::com_infra::e_ProcessDataOk;
}

forte::com_infra::EComResponse CWin32SerComLayer::openSerialConnection(const SSerialParameters& paSerialParameters, CSerialComLayerBase<HANDLE>::TSerialHandleType* paHandleResult){

  HANDLE serialHandle = CreateFile(
      paSerialParameters.interfaceName.getValue(),
      GENERIC_READ | GENERIC_WRITE, 0, 0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, 0);

  if(INVALID_HANDLE_VALUE == serialHandle){
    if(ERROR_FILE_NOT_FOUND == GetLastError()){
      return forte::com_infra::e_ProcessDataNoSocket;
    }
    return forte::com_infra::e_ProcessDataInvalidObject;
  }

  DCB dcbSerialParams = DCB();

  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if(!GetCommState(serialHandle, &dcbSerialParams)){
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  dcbSerialParams.BaudRate = static_cast<DWORD>(paSerialParameters.baudRate);
  switch (paSerialParameters.baudRate) {
    //These are ok baud rates
      case e110: break;
      case e300: break;
      case e600: break;
      case e1200: break;
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
      //all other numbers are invalid!
      default: return forte::com_infra::e_InitInvalidId; break;
    }

  dcbSerialParams.ByteSize = paSerialParameters.byteSize;

  switch (paSerialParameters.stopBits){
    case EForteSerialStopBits::eOneBit:
      dcbSerialParams.StopBits = ONESTOPBIT;
      break;
    case EForteSerialStopBits::eOne5Bits:
      dcbSerialParams.StopBits = ONE5STOPBITS;
      break;
    case EForteSerialStopBits::eTwoBits:
      dcbSerialParams.StopBits = TWOSTOPBITS;
      break;
  }

  switch (paSerialParameters.parity){
    case EForteSerialParity::eNoParity:
      dcbSerialParams.Parity = NOPARITY;
      break;
    case EForteSerialParity::eODD:
      dcbSerialParams.Parity = ODDPARITY;
      break;
    case EForteSerialParity::eEven:
      dcbSerialParams.Parity = EVENPARITY;
      break;
    case EForteSerialParity::eMark:
      dcbSerialParams.Parity = MARKPARITY;
      break;
    case EForteSerialParity::eSpace:
      dcbSerialParams.Parity = SPACEPARITY;
      break;
  }

  if(!SetCommState(serialHandle, &dcbSerialParams)){
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  //Timeouts for non-blocking behaviour
  COMMTIMEOUTS timeouts = COMMTIMEOUTS();
  //Read timeouts
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  //Write timeouts
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;

  if(!SetCommTimeouts(serialHandle, &timeouts)){
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  switch (m_poFb->getComServiceType()){
    case forte::com_infra::e_Server:
    case forte::com_infra::e_Client:
      getExtEvHandler<CWin32SerComHandler>().registerSerComLayer(this);
      m_eConnectionState = forte::com_infra::e_Connected;
      break;
      case forte::com_infra::e_Publisher:
      break;
      case forte::com_infra::e_Subscriber:
      break;
    }

  *paHandleResult = static_cast<void*>(serialHandle);

  return forte::com_infra::e_InitOk;
}

void CWin32SerComLayer::closeConnection()  {
  getExtEvHandler<CWin32SerComHandler>().unregisterSerComLayer(this);
  CloseHandle(static_cast<HANDLE>(mSerialHandle));
}
