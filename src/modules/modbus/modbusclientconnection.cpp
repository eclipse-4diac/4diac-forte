/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "modbusclientconnection.h"
#include "devlog.h"
#include "modbuspoll.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace modbus_connection_event;

/*************************************
 * CModbusClientConnection class
 *************************************/

CModbusClientConnection::CModbusClientConnection() :
    CModbusConnection(), m_pModbusConnEvent(NULL), m_nNrOfPolls(0), m_nSlaveId(0xFF), m_unBufFillSize(0){
}

CModbusClientConnection::~CModbusClientConnection(){
  if (m_bConnected){
    disconnect();
  }
  TModbusPollList::Iterator itEnd(m_lstPollList.end());
  for(TModbusPollList::Iterator itRunner = m_lstPollList.begin(); itRunner != itEnd; ++itRunner){
    delete *itRunner;
  }
  if (m_pModbusConnEvent != NULL){
    delete m_pModbusConnEvent;
  }
}

int CModbusClientConnection::readData(uint8_t *pa_pData){
  for(unsigned int i = 0; i < m_unBufFillSize; i++){
    pa_pData[i] = m_acRecvBuffer[i];
  }

  return (int) m_unBufFillSize;
}

int CModbusClientConnection::writeData(uint16_t *pa_pData, unsigned int pa_nDataSize){
  unsigned int dataIndex = 0;

  TModbusSendList::Iterator itEnd = m_lstSendList.end();
  for (TModbusSendList::Iterator it = m_lstSendList.begin(); it!=itEnd; ++it) {
    if (dataIndex + it->m_nNrAddresses > pa_nDataSize) {
      break;
    }
    modbus_write_registers(m_pModbusConn, it->m_nStartAddress, it->m_nNrAddresses, &pa_pData[dataIndex]);
    dataIndex += it->m_nNrAddresses;
  }

  return (int)dataIndex;
}

int CModbusClientConnection::connect(){
  CModbusConnection::connect();

  if(m_nSlaveId != 0xFF){
    modbus_set_slave(m_pModbusConn, m_nSlaveId);
  }

  m_pModbusConnEvent = new CModbusConnectionEvent(1000);
  m_pModbusConnEvent->activate();

  this->start();

  return 0;
}

void CModbusClientConnection::disconnect(){
  this->end();
  if (m_bConnected){
    modbus_close(m_pModbusConn);
    m_bConnected = false;
  }
  CModbusConnection::disconnect();
}

void CModbusClientConnection::addNewPoll(TForteUInt32 pa_nPollInterval, unsigned int pa_nFunctionCode, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses){
  CModbusPoll *newPoll = NULL;

  TModbusPollList::Iterator itEnd = m_lstPollList.end();
  for(TModbusPollList::Iterator it = m_lstPollList.begin(); it != itEnd; ++it){
    if(it->getUpdateInterval() == pa_nPollInterval && it->getFunctionCode() == pa_nFunctionCode){
      it->addPollAddresses(pa_nStartAddress, pa_nNrAddresses);
      newPoll = *it;
      break;
    }
  }
  if(newPoll == NULL){
    m_lstPollList.push_back(new CModbusPoll(pa_nPollInterval, pa_nFunctionCode, pa_nStartAddress, pa_nNrAddresses));
    m_nNrOfPolls++;
    m_anRecvBuffPosition[m_nNrOfPolls - 1] = m_unBufFillSize;
  }

  // Count bytes
  unsigned int nrBytes = 0;
  switch (pa_nFunctionCode){
    case 1:
    case 2:
      nrBytes = pa_nNrAddresses;
      break;
    case 3:
    case 4:
      nrBytes = pa_nNrAddresses * 2;
      break;
  }
  for(unsigned int i = m_unBufFillSize; i < m_unBufFillSize + nrBytes; i++){
    m_acRecvBuffer[i] = 0;
  }
  m_unBufFillSize += nrBytes;
}

void CModbusClientConnection::addNewSend(unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses) {
  SSendInformation sendInfo = {pa_nStartAddress, pa_nNrAddresses};

  m_lstSendList.push_back(sendInfo);
}

void CModbusClientConnection::setSlaveId(unsigned int pa_nSlaveId){
  m_nSlaveId = pa_nSlaveId;
}

void CModbusClientConnection::run(){

  while(isAlive()){
    if(m_bConnected){
      tryPolling();
    }
    else{
      tryConnect();
    }

#ifdef WIN32
    Sleep(1);
#else
    usleep(1);
#endif
  }
}

void CModbusClientConnection::tryPolling(){
  unsigned int nrErrors = 0;
  bool dataReturned = false;

  unsigned int index = 0;
  TModbusPollList::Iterator itEnd(m_lstPollList.end());
  for(TModbusPollList::Iterator itPoll = m_lstPollList.begin(); itPoll != itEnd; ++itPoll, ++index){
    if(itPoll->readyToExecute()){
      int nrVals = itPoll->executeEvent(m_pModbusConn, (void*) &m_acRecvBuffer[m_anRecvBuffPosition[index]]); // retVal);

      if(nrVals < 0){
        DEVLOG_ERROR("Error reading input status :: %s\n", modbus_strerror(errno));
        itPoll->deactivate();

        nrErrors++;
      }
      else if(nrVals > 0){
        dataReturned = true;
      }
    }
  }

  if(dataReturned)
    CModbusHandler::getInstance().executeComCallback(m_nComCallbackId);

  if(nrErrors == m_nNrOfPolls){
    m_bConnected = false;
    m_pModbusConnEvent = new CModbusConnectionEvent(2000);
    m_pModbusConnEvent->activate();
  }
}

void CModbusClientConnection::tryConnect(){
  if(m_pModbusConnEvent != NULL){
    if(m_pModbusConnEvent->readyToExecute()){
      if(m_pModbusConnEvent->executeEvent(m_pModbusConn, NULL) < 0)
        DEVLOG_ERROR("Connection to Modbus server failed: %s\n", modbus_strerror(errno));
      else{
        DEVLOG_INFO("Connection to Modbus server succeded\n");

        delete m_pModbusConnEvent;
        m_pModbusConnEvent = NULL;

        m_bConnected = true;

        // Start polling
        TModbusPollList::Iterator itEnd(m_lstPollList.end());
        for(TModbusPollList::Iterator itPoll = m_lstPollList.begin(); itPoll != itEnd; ++itPoll){
          itPoll->activate();
        }
      }
    }
  }
}

/*************************************
 * CModbusConnectionEvent class
 *************************************/
CModbusConnectionEvent::CModbusConnectionEvent(long pa_nReconnectInterval) :
    CModbusTimedEvent(pa_nReconnectInterval){
}

int CModbusConnectionEvent::executeEvent(modbus_t *pa_pModbusConn, void *pa_pRetVal){
  (void)pa_pRetVal; // avoid warning

  restartTimer();

  return modbus_connect(pa_pModbusConn);
}

