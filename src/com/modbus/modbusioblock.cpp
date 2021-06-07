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


CModbusIOBlock::CModbusIOBlock()
  : m_pCache(0),m_nReadSize(0),m_nSendSize(0){
}

CModbusIOBlock::~CModbusIOBlock(){
  if (m_pCache) delete [] (uint8_t*)m_pCache;
}

void CModbusIOBlock::allocCache() {
  m_pCache = new uint8_t[m_nReadSize]();
}

void CModbusIOBlock::addNewRead(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses) {
  m_lReads.push_back((SModbusRange){pa_eFunction, pa_nStartAddress, pa_nNrAddresses});
  m_nReadSize += getRegisterSize(pa_eFunction) * pa_nNrAddresses;
  if (m_pCache) {
    delete [] (uint8_t*)m_pCache;
    allocCache();
  }
}

void CModbusIOBlock::addNewSend(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses) {
  m_lSends.push_back((SModbusRange){pa_eFunction, pa_nStartAddress, pa_nNrAddresses});
  m_nSendSize += getRegisterSize(pa_eFunction) * pa_nNrAddresses;
}

unsigned int CModbusIOBlock::getRegisterSize(EModbusFunction pa_eFunction) {
  switch (pa_eFunction) {
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
