/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "modbuspoll.h"
#include "modbusioblock.h"

#include <devlog.h>

#include <modbus.h>

CModbusPoll::CModbusPoll(long pa_nPollInterval) :
    CModbusTimedEvent((TForteUInt32)pa_nPollInterval){
}

CModbusPoll::~CModbusPoll(){
  m_lPolls.clear();
}

void CModbusPoll::addPollBlock(CModbusIOBlock *pa_pIOBlock){
  m_lPolls.push_back(pa_pIOBlock);
  pa_pIOBlock->allocCache();
}

int CModbusPoll::executeEvent(modbus_t *pa_pModbusConn, void *){
  restartTimer();

  int nrVals = 0;
  for(auto it : m_lPolls){
    nrVals += readOneBlock(pa_pModbusConn, it);
  }
  return nrVals;
}

int CModbusPoll::readOneBlock(modbus_t *pa_pModbusConn, CModbusIOBlock *pa_pIOBlock){
  int nrVals = 0;
  unsigned int dataIndex = 0;
  const CModbusIOBlock::TModbusRangeList &lReads = pa_pIOBlock->getReads();
  void *pData = pa_pIOBlock->getCache();
  for (auto &it : lReads) {
    const unsigned int nextDataIndex = dataIndex + it.m_nNrAddresses * CModbusIOBlock::getRegisterSize(it.m_nFunctionCode);
    if (nextDataIndex > pa_pIOBlock->getReadSize()) {
      break;
    }
    switch (it.m_nFunctionCode){
      case 1:
        nrVals += modbus_read_bits(pa_pModbusConn, it.m_nStartAddress, it.m_nNrAddresses, &((uint8_t*)pData)[dataIndex]);
        break;
      case 2:
        nrVals += modbus_read_input_bits(pa_pModbusConn, it.m_nStartAddress, it.m_nNrAddresses, &((uint8_t*)pData)[dataIndex]);
        break;
      case 3:
        nrVals += modbus_read_registers(pa_pModbusConn, it.m_nStartAddress, it.m_nNrAddresses, &((uint16_t*)pData)[dataIndex]);
        break;
      case 4:
        nrVals += modbus_read_input_registers(pa_pModbusConn, it.m_nStartAddress, it.m_nNrAddresses, &((uint16_t*)pData)[dataIndex]);
        break;
      default:
        //TODO Error
        break;
    }
    dataIndex = nextDataIndex;
  }
  return nrVals;
}
