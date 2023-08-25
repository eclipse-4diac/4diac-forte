/*******************************************************************************
 * Copyright (c) 2021 Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#include "modbusioblock.h"
#include "modbusconnection.h"


CModbusIOBlock::CModbusIOBlock(forte::com_infra::CModbusComLayer* paParent)
  : mParent(paParent),mCache(0),mReadSize(0),mSendSize(0){
}

CModbusIOBlock::~CModbusIOBlock(){
  if (mCache) delete [] (uint8_t*)mCache;
}

void CModbusIOBlock::allocCache() {
  mCache = new uint8_t[mReadSize]();
}

void CModbusIOBlock::addNewRead(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses) {
  mReads.push_back((SModbusRange){paFunction, paStartAddress, paNrAddresses});
  mReadSize += getRegisterSize(paFunction) * paNrAddresses;
  if (mCache) {
    delete [] (uint8_t*)mCache;
    allocCache();
  }
}

void CModbusIOBlock::addNewSend(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses) {
  mSends.push_back((SModbusRange){paFunction, paStartAddress, paNrAddresses});
  mSendSize += getRegisterSize(paFunction) * paNrAddresses;
}

unsigned int CModbusIOBlock::getRegisterSize(EModbusFunction paFunction) {
  switch (paFunction) {
    case eDiscreteInput:
    case eCoil:
        return sizeof(uint8_t);
    case eInputRegister:
    case eHoldingRegister:
        return sizeof(uint16_t);
    default:
        return 0;
  }
}
