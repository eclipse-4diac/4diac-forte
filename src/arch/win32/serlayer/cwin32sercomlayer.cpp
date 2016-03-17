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
    forte::com_infra::CComLayer(pa_poUpperLayer, pa_poFB), m_unBufFillSize(0)  {
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
		  if (0 == m_poTopLayer)  {
			  CIEC_ANY *apoRDs = m_poFb->getRDs();
			  switch(apoRDs[0].getDataTypeID())  {
			  case CIEC_ANY::e_STRING:
				  static_cast<CIEC_STRING&>(apoRDs[0]).fromUTF8(m_acRecvBuffer, m_unBufFillSize, false);
				  break;
			  }
		  }
		  else  if((0 < m_unBufFillSize) && (0 != m_poTopLayer))  {
			  m_eInterruptResp = m_poTopLayer->recvData(m_acRecvBuffer, m_unBufFillSize);
			  m_unBufFillSize = 0;
		  }
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
  TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr > (pa_pvData);
  for (unsigned int i = 0; i < pa_unSize; ++i)
  {  
	  if (apoSDs[i].getDataTypeID() == CIEC_ANY::e_STRING)  
	  {  
		  unsigned int nToBeSent= static_cast<const CIEC_STRING&>(apoSDs[i]).length();
		  if(!WriteFile(m_hSerial, static_cast<const CIEC_STRING&>(apoSDs[i]).getValue(), nToBeSent, &dwBytesWritten, NULL))
		  {
			  return forte::com_infra::e_ProcessDataSendFailed;
		  }
		  if(dwBytesWritten != static_cast<const CIEC_STRING&>(apoSDs[i]).length())
		  {
			  return forte::com_infra::e_ProcessDataSendFailed;
		  }
	  }
  }
  return forte::com_infra::e_ProcessDataOk;
}


forte::com_infra::EComResponse CWin32SerComLayer::openConnection(char *pa_acLayerParameter)  {
  //Create Serial Com Handle
  m_hSerial = CreateFile(pa_acLayerParameter,
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

  dcbSerialParams.BaudRate = CBR_19200;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;

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
