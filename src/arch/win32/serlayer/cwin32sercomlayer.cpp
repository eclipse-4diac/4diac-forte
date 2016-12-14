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
#include <forte_any.h>
#include <forte_string.h>
#include "cwin32sercomhandler.h"

CWin32SerComLayer::CWin32SerComLayer(forte::com_infra::CComLayer* pa_poUpperLayer,
    forte::com_infra::CCommFB * pa_poFB) :
    forte::com_infra::CComLayer(pa_poUpperLayer, pa_poFB), m_unBufFillSize(0), m_eInterruptResp(forte::com_infra::EComResponse::e_Nothing)  {
  memset(m_acRecvBuffer, 0, sizeof(m_acRecvBuffer)); //TODO change this to  m_acRecvBuffer{0} in the extended list when fully switching to C++11
  memset(m_acTerminationSymbol, 0, sizeof(m_acTerminationSymbol)); //TODO change this to  m_acTerminationSymbol{0} in the extended list when fully switching to C++11
  m_hSerial = INVALID_HANDLE_VALUE;
}

CWin32SerComLayer::~CWin32SerComLayer() {
  closeConnection();
}


forte::com_infra::EComResponse CWin32SerComLayer::recvData(const void *, unsigned int )  {
  m_eInterruptResp = forte::com_infra::e_Nothing;

  DWORD dwBytesRead = 0;
  if(ReadFile(m_hSerial, m_acRecvBuffer, m_unMaxRecvBuffer-1, &dwBytesRead, NULL))
  {  //TODO: Failure handling and send INITO-
	  if (0 < dwBytesRead)
	  {
		  m_unBufFillSize = dwBytesRead;
		  m_eInterruptResp = forte::com_infra::e_ProcessDataOk;
		  m_poFb->interruptCommFB(this);
	  } else{
	    m_eInterruptResp = forte::com_infra::e_ProcessDataRecvFaild;
	  }
  }
  return m_eInterruptResp;
}

forte::com_infra::EComResponse CWin32SerComLayer::processInterrupt(){
  if(forte::com_infra::e_ProcessDataOk == m_eInterruptResp){
    switch (m_eConnectionState){
      case forte::com_infra::e_Connected:
			  m_eInterruptResp = m_poTopLayer->recvData(m_acRecvBuffer, m_unBufFillSize);
			  m_unBufFillSize = 0;
        break;
      case forte::com_infra::e_Disconnected:
      case forte::com_infra::e_Listening:
      case forte::com_infra::e_ConnectedAndListening:
      default:
        break;
    }
  }
  return m_eInterruptResp;
}

void CWin32SerComLayer::closeConnection()  {
  CWin32SerComHandler::getInstance().unregisterSerComLayer(this);
  CloseHandle(m_hSerial);
}

forte::com_infra::EComResponse CWin32SerComLayer::sendData(void *pa_pvData, unsigned int pa_unSize)
{
  DWORD dwBytesWritten= 0, dwWaitResult = 0;
  char *pcData = static_cast<char*> (pa_pvData);
  unsigned int nToBeSent = pa_unSize;
  //Send payload
  if(!WriteFile(m_hSerial, pcData, nToBeSent, &dwBytesWritten, NULL))
  {
   return forte::com_infra::e_ProcessDataSendFailed;
  }
  if(dwBytesWritten != nToBeSent)
  {
    return forte::com_infra::e_ProcessDataSendFailed;
  }

  //Send termination symbol(s)
  if (!WriteFile(m_hSerial, m_acTerminationSymbol, strlen(m_acTerminationSymbol), &dwBytesWritten, NULL))
  {
	  return forte::com_infra::e_ProcessDataSendFailed;
  }
  if (strlen(m_acTerminationSymbol) != dwBytesWritten)
  {
	  return forte::com_infra::e_ProcessDataSendFailed;
  }

  return forte::com_infra::e_ProcessDataOk;
}


forte::com_infra::EComResponse CWin32SerComLayer::openConnection(char *pa_acLayerParameter)  {
  //Create Serial Com Handle
  CWin32SerComParameterParser parser(pa_acLayerParameter);
  parser.setSeparator(',');
  parser.parseParameters();

  //TODO: Parse 61131 string
  if (0 == strcmp("$n", parser[CWin32SerComParameterParser::eTerminationSymbol])) {
    strcpy(m_acTerminationSymbol, "\n");
  } else if (0 == strcmp("$r", parser[CWin32SerComParameterParser::eTerminationSymbol])) 
  {
	strcpy(m_acTerminationSymbol, "\r");
  } else if (0 == strcmp("$r$n", parser[CWin32SerComParameterParser::eTerminationSymbol]))
  {
	strcpy(m_acTerminationSymbol, "\r\n");
  }
  else
  {
    return forte::com_infra::e_InitInvalidId;
  }

  

  m_hSerial = CreateFile(
	  parser[CWin32SerComParameterParser::eInterface],
      GENERIC_READ | GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0);

  if(INVALID_HANDLE_VALUE == m_hSerial)  {
    if(ERROR_FILE_NOT_FOUND == GetLastError())  {
      return forte::com_infra::e_ProcessDataNoSocket;
    }
    return forte::com_infra::e_ProcessDataInvalidObject;
  }

  DCB dcbSerialParams = {0};

  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if(!GetCommState(m_hSerial, &dcbSerialParams))  {
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  //Check baud rate setting
  DWORD nBaudRate = atoi(parser[CWin32SerComParameterParser::eBaudrate]);
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
  BYTE nByteSize = atoi(parser[CWin32SerComParameterParser::eByteSize]);
  if (4 > nByteSize || 8 < nByteSize) {
	  return forte::com_infra::e_InitInvalidId;
  }

  //Check stopbits setting
  BYTE nStopBits = ONESTOPBIT;
  if (0 == stricmp(parser[CWin32SerComParameterParser::eStopBits], "1"))
  {
	  nStopBits = ONESTOPBIT;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eStopBits], "1.5"))
  {
	  nStopBits = ONE5STOPBITS;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eStopBits], "2"))
  {
	  nStopBits = TWOSTOPBITS;
  }
  else
  {
	  return forte::com_infra::e_InitInvalidId;
  }

  //Check parity setting
  BYTE nParity = NOPARITY;
  if (0 == stricmp(parser[CWin32SerComParameterParser::eParity], "NONE") )
  {
    nParity = NOPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eParity], "ODD"))
  {
	nParity = ODDPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eParity], "EVEN"))
  {
	nParity = EVENPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eParity], "MARK"))
  {
	nParity = MARKPARITY;
  }
  else if (0 == stricmp(parser[CWin32SerComParameterParser::eParity], "SPACE"))
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

  if(!SetCommState(m_hSerial, &dcbSerialParams))  {
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

  if(!SetCommTimeouts(m_hSerial, &timeouts))  {
    return forte::com_infra::e_ProcessDataNoSocket;
  }

  switch (m_poFb->getComServiceType()){
  case forte::com_infra::e_Server:
  case forte::com_infra::e_Client:
	  CWin32SerComHandler::getInstance().registerSerComLayer(this);
	  m_eConnectionState = forte::com_infra::e_Connected;
	  break;
  case forte::com_infra::e_Publisher:
	  break;
  case forte::com_infra::e_Subscriber:
	  break;
  }
	return forte::com_infra::e_InitOk;
}
