/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "cwin32sercomlayer.h"
#include "cwin32sercomhandler.h"
#include "../../../core/utils/parameterParser.h"
#include "../../../core/datatypes/forte_any.h"
#include "../../../core/datatypes/forte_string.h"

CWin32SerComLayer::CWin32SerComLayer(forte::com_infra::CComLayer* paUpperLayer,
    forte::com_infra::CBaseCommFB * paFB) :
    forte::com_infra::CComLayer(paUpperLayer, paFB), mInterruptResp(forte::com_infra::EComResponse::e_Nothing), mBufFillSize(0) {
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer)); //TODO change this to  m_acRecvBuffer{0} in the extended list when fully switching to C++11
  memset(mTerminationSymbol, 0, sizeof(mTerminationSymbol)); //TODO change this to  m_acTerminationSymbol{0} in the extended list when fully switching to C++11
  mSerial = INVALID_HANDLE_VALUE;
}

CWin32SerComLayer::~CWin32SerComLayer() {
  closeConnection();
}


forte::com_infra::EComResponse CWin32SerComLayer::recvData(const void *, unsigned int )  {
  mInterruptResp = forte::com_infra::e_Nothing;

  DWORD dwBytesRead = 0;
  if(ReadFile(mSerial, mRecvBuffer, mMaxRecvBuffer-1, &dwBytesRead, NULL))
  {  //TODO: Failure handling and send INITO-
	  if (0 < dwBytesRead)
	  {
		  mBufFillSize = dwBytesRead;
		  mInterruptResp = forte::com_infra::e_ProcessDataOk;
		  m_poFb->interruptCommFB(this);
	  } else{
	    mInterruptResp = forte::com_infra::e_ProcessDataRecvFaild;
	  }
  }
  return mInterruptResp;
}

forte::com_infra::EComResponse CWin32SerComLayer::processInterrupt(){
  if(forte::com_infra::e_ProcessDataOk == mInterruptResp){
    switch (m_eConnectionState){
      case forte::com_infra::e_Connected:
			  mInterruptResp = m_poTopLayer->recvData(mRecvBuffer, mBufFillSize);
			  mBufFillSize = 0;
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

forte::com_infra::EComResponse CWin32SerComLayer::sendData(void *paData, unsigned int paSize)
{
  DWORD dwBytesWritten= 0, dwWaitResult = 0;
  char *pcData = static_cast<char*> (paData);
  unsigned int nToBeSent = paSize;
  //Send payload
  if(!WriteFile(mSerial, pcData, nToBeSent, &dwBytesWritten, NULL))
  {
   return forte::com_infra::e_ProcessDataSendFailed;
  }
  if(dwBytesWritten != nToBeSent)
  {
    return forte::com_infra::e_ProcessDataSendFailed;
  }

  //Send termination symbol(s)
  if (!WriteFile(mSerial, mTerminationSymbol, strlen(mTerminationSymbol), &dwBytesWritten, NULL))
  {
	  return forte::com_infra::e_ProcessDataSendFailed;
  }
  if (strlen(mTerminationSymbol) != dwBytesWritten)
  {
	  return forte::com_infra::e_ProcessDataSendFailed;
  }

  return forte::com_infra::e_ProcessDataOk;
}

forte::com_infra::EComResponse CWin32SerComLayer::openConnection(char *paLayerParameter)  {
  //Create Serial Com Handle
  CParameterParser parser(paLayerParameter, mNoOfParameters);
  if(mNoOfParameters != parser.parseParameters()){
    return forte::com_infra::e_InitInvalidId;
  }

  //TODO: Parse 61131 string
  if (0 == strcmp("$n", parser[eTerminationSymbol])) {
    strcpy(mTerminationSymbol, "\n");
  } else if (0 == strcmp("$r", parser[CWin32SerComLayer::eTerminationSymbol]))
  {
	strcpy(mTerminationSymbol, "\r");
  } else if (0 == strcmp("$r$n", parser[CWin32SerComLayer::eTerminationSymbol]))
  {
	strcpy(mTerminationSymbol, "\r\n");
  }
  else
  {
    return forte::com_infra::e_InitInvalidId;
  }

  

  mSerial = CreateFile(
	  parser[CWin32SerComLayer::eInterface],
      GENERIC_READ | GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0);

  if(INVALID_HANDLE_VALUE == mSerial)  {
    if(ERROR_FILE_NOT_FOUND == GetLastError())  {
      return forte::com_infra::e_ProcessDataNoSocket;
    }
    return forte::com_infra::e_ProcessDataInvalidObject;
  }

  DCB dcbSerialParams = {0};

  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if(!GetCommState(mSerial, &dcbSerialParams))  {
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  //Check baud rate setting
  DWORD nBaudRate = atoi(parser[CWin32SerComLayer::eBaudrate]);
  switch (nBaudRate) {
	//These are ok baud rates
    case CBR_110: break;
    case CBR_300: break;
    case CBR_600: break;
    case CBR_1200: break;
    case CBR_2400: break;
    case CBR_4800: break;
    case CBR_9600: break;
    case CBR_14400: break;
    case CBR_19200: break;
    case CBR_38400: break;
    case CBR_56000: break;
    case CBR_57600: break;
    case CBR_115200: break;
    case CBR_128000: break;
    case CBR_256000: break;
	//all other numbers are invalid!
    default: return forte::com_infra::e_InitInvalidId; break;
  }

  //Check byte size setting
  BYTE nByteSize = atoi(parser[CWin32SerComLayer::eByteSize]);
  if (4 > nByteSize || 8 < nByteSize) {
	  return forte::com_infra::e_InitInvalidId;
  }

  //Check stopbits setting
  BYTE nStopBits = ONESTOPBIT;
  if (0 == stricmp(parser[CWin32SerComLayer::eStopBits], "1"))
  {
	  nStopBits = ONESTOPBIT;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eStopBits], "1.5"))
  {
	  nStopBits = ONE5STOPBITS;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eStopBits], "2"))
  {
	  nStopBits = TWOSTOPBITS;
  }
  else
  {
	  return forte::com_infra::e_InitInvalidId;
  }

  //Check parity setting
  BYTE nParity = NOPARITY;
  if (0 == stricmp(parser[CWin32SerComLayer::eParity], "NONE") )
  {
    nParity = NOPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eParity], "ODD"))
  {
	nParity = ODDPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eParity], "EVEN"))
  {
	nParity = EVENPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eParity], "MARK"))
  {
	nParity = MARKPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComLayer::eParity], "SPACE"))
  {
	nParity = SPACEPARITY;
  }
  else
  {
	  return forte::com_infra::e_InitInvalidId;
  }

  dcbSerialParams.BaudRate = nBaudRate;
  dcbSerialParams.ByteSize = nByteSize;
  dcbSerialParams.StopBits = nStopBits;
  dcbSerialParams.Parity = nParity;

  if(!SetCommState(mSerial, &dcbSerialParams))  {
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  //Timeouts for non-blocking behaviour
  COMMTIMEOUTS timeouts = {0};
  //Read timeouts
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  //Write timeouts
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;

  if(!SetCommTimeouts(mSerial, &timeouts))  {
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  switch (m_poFb->getComServiceType()){
  case forte::com_infra::e_Server:
  case forte::com_infra::e_Client:
    GET_HANDLER_FROM_LAYER(*m_poFb, CWin32SerComHandler)->registerSerComLayer(this);
	  m_eConnectionState = forte::com_infra::e_Connected;
	  break;
  case forte::com_infra::e_Publisher:
	  break;
  case forte::com_infra::e_Subscriber:
	  break;
  }
	return forte::com_infra::e_InitOk;
}

void CWin32SerComLayer::closeConnection()  {
  GET_HANDLER_FROM_LAYER(*m_poFb, CWin32SerComHandler)->unregisterSerComLayer(this);
  CloseHandle(mSerial);
}
