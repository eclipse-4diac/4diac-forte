/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, fortiss GmbH, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#include "modbusconnection.h"
#include "modbuslayer.h"
#include "../../core/cominfra/basecommfb.h"

CModbusConnection::CModbusConnection(CModbusHandler* pa_modbusHandler) : mModbusConn(nullptr), mModbusHandler(pa_modbusHandler),
  mConnected(false), mIPAddress(nullptr), mPort(0),
  mDevice{0}, mBaud(0), mParity(0), mDataBit(0),
  mStopBit(0), mFlowControl(eFlowNone), mResponseTimeout(0), mByteTimeout(0){
}
    
CModbusConnection::~CModbusConnection(){
  modbus_free(mModbusConn);
}

int CModbusConnection::connect(){
  if (mIPAddress != nullptr) {
    mModbusConn = modbus_new_tcp(mIPAddress, mPort);
  } else {
    mModbusConn = modbus_new_rtu(mDevice, mBaud, mParity, mDataBit, mStopBit);
  }

#if 0
  if(mResponseTimeout > 0){
    timeval responseTimeout;
    responseTimeout.tv_sec = mResponseTimeout / 1000;
    responseTimeout.tv_usec = (mResponseTimeout % 1000)*1000;
    modbus_set_response_timeout(mModbusConn, &responseTimeout);
  }
  if(mByteTimeout > 0){
    timeval byteTimeout;
    byteTimeout.tv_sec = mByteTimeout / 1000;
    byteTimeout.tv_usec = (mByteTimeout % 1000)*1000;
    modbus_set_byte_timeout(mModbusConn, &byteTimeout);
  }
#endif

  return 0;
}

void CModbusConnection::disconnect(){

}

void CModbusConnection::setIPAddress(const char* paIPAddress){
  mIPAddress = paIPAddress;
}

void CModbusConnection::setPort(unsigned int paPort){
  mPort = paPort;
}

void CModbusConnection::setDevice(const char* paDevice) {
  strcpy(mDevice, paDevice);
}

void CModbusConnection::setBaud(int paBaud) {
  mBaud = paBaud;
}

void CModbusConnection::setParity(char paParity) {
  mParity = paParity;
}

void CModbusConnection::setDataBit(int paDataBit) {
  mDataBit = paDataBit;
}

void CModbusConnection::setStopBit(int paStopBit) {
  mStopBit = paStopBit;
}

void CModbusConnection::setFlowControl(EModbusFlowControl paFlowControl){
  mFlowControl = paFlowControl;
}

void CModbusConnection::setResponseTimeout(unsigned int paResponseTimeout){
  mResponseTimeout = paResponseTimeout;
}

void CModbusConnection::setByteTimeout(unsigned int paByteTimeout){
  mByteTimeout = paByteTimeout;
}

int CModbusConnection::writeData(CModbusIOBlock* paIOBlock, const void* paData, unsigned int paDataSize){
  unsigned int dataIndex = 0;
  const CModbusIOBlock::TModbusRangeList &lSends = paIOBlock->getSends();
  for (auto &it : lSends) {
    const unsigned int nextDataIndex = dataIndex + it.mNrAddresses * CModbusIOBlock::getRegisterSize(it.mFunction);
    if (nextDataIndex > paDataSize) {
      const unsigned int maxNrAddresses = (paDataSize - dataIndex) / CModbusIOBlock::getRegisterSize(it.mFunction);
      DEVLOG_WARNING("Modbus writing %u instead of %u registers from address %u\n", maxNrAddresses, it.mNrAddresses, it.mStartAddress);
      writeDataRange(it.mFunction, it.mStartAddress, maxNrAddresses, (const uint8_t*)paData + dataIndex);
      break;
    }
    writeDataRange(it.mFunction, it.mStartAddress, it.mNrAddresses, (const uint8_t*)paData + dataIndex);
    dataIndex = nextDataIndex;
  }
  return (int)dataIndex;
}
