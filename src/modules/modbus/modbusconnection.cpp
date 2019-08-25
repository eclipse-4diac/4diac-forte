/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "modbusconnection.h"
#include "modbuslayer.h"
#include "../../core/cominfra/basecommfb.h"

CModbusConnection::CModbusConnection(CModbusHandler* pa_modbusHandler) : m_pModbusConn(NULL), m_pModbusHandler(pa_modbusHandler), m_nComCallbackId(0),
  m_bConnected(false), m_paIPAddress(NULL), m_nPort(0),
  m_chDevice(NULL), m_nBaud(0), m_cParity(0), m_nDataBit(0),
   m_nStopBit(0), m_nResponseTimeout(0), m_nByteTimeout(0){
}
    
CModbusConnection::~CModbusConnection(){
  modbus_free(m_pModbusConn);
}

int CModbusConnection::connect(){
  if (m_paIPAddress != NULL) {
    m_pModbusConn = modbus_new_tcp(m_paIPAddress, m_nPort);
  } else if (m_chDevice != NULL) {
    m_pModbusConn = modbus_new_rtu(m_chDevice, m_nBaud, m_cParity, m_nDataBit, m_nStopBit);
  } else {
    return -1;
  }

  if(m_nResponseTimeout > 0){
    timeval responseTimeout;
    responseTimeout.tv_sec = m_nResponseTimeout / 1000;
    responseTimeout.tv_usec = (m_nResponseTimeout % 1000)*1000;
    //modbus_set_response_timeout(m_pModbusConn, &responseTimeout);
  }
  if(m_nByteTimeout > 0){
    timeval byteTimeout;
    byteTimeout.tv_sec = m_nByteTimeout / 1000;
    byteTimeout.tv_usec = (m_nByteTimeout % 1000)*1000;
    //modbus_set_byte_timeout(m_pModbusConn, &byteTimeout);
  }

  return 0;
}

void CModbusConnection::disconnect(){

}

void CModbusConnection::setIPAddress(const char* pa_poIPAddress){
  m_paIPAddress = pa_poIPAddress;
}

void CModbusConnection::setPort(unsigned int pa_nPort){
  m_nPort = pa_nPort;
}

void CModbusConnection::setDevice(const char* pa_chDevice) {
  m_chDevice = pa_chDevice;
}

void CModbusConnection::setBaud(int pa_nBaud) {
  m_nBaud = pa_nBaud;
}

void CModbusConnection::setParity(char pa_cParity) {
  m_cParity = pa_cParity;
}

void CModbusConnection::setDataBit(int pa_nDataBit) {
  m_nDataBit = pa_nDataBit;
}

void CModbusConnection::setStopBit(int pa_nStopBit) {
  m_nStopBit = pa_nStopBit;
}

void CModbusConnection::setResponseTimeout(unsigned int pa_nResponseTimeout){
  m_nResponseTimeout = pa_nResponseTimeout;
}

void CModbusConnection::setByteTimeout(unsigned int pa_nByteTimeout){
  m_nByteTimeout = pa_nByteTimeout;
}

void CModbusConnection::setComCallback(forte::com_infra::CModbusComLayer* pa_poModbusLayer){
  m_nComCallbackId = m_pModbusHandler->addComCallback(pa_poModbusLayer);
}
