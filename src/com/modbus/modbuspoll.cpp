/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#include "modbuspoll.h"
#include "modbushandler.h"
#include "modbusioblock.h"

#include <devlog.h>

#include <modbus.h>

CModbusPoll::CModbusPoll(CModbusHandler* paModbusHandler, long paPollInterval) :
  CModbusTimedEvent((TForteUInt32)paPollInterval),mModbusHandler(paModbusHandler){
}

CModbusPoll::~CModbusPoll(){
  mPolls.clear();
}

void CModbusPoll::addPollBlock(CModbusIOBlock *paIOBlock){
  mPolls.push_back(paIOBlock);
  paIOBlock->allocCache();
}

int CModbusPoll::executeEvent(modbus_t *paModbusConn, void *){
  restartTimer();

  int nrVals = 0;
  for(auto it : mPolls){
    nrVals += readOneBlock(paModbusConn, it);
  }
  return nrVals;
}

int CModbusPoll::readOneBlock(modbus_t *paModbusConn, CModbusIOBlock *paIOBlock){
  int nrVals = 0;
  unsigned int dataIndex = 0;
  const CModbusIOBlock::TModbusRangeList &lReads = paIOBlock->getReads();
  uint8_t *pData = (uint8_t*)paIOBlock->getCache();
  for (auto &it : lReads) {
    const unsigned int nextDataIndex = dataIndex + it.mNrAddresses * CModbusIOBlock::getRegisterSize(it.mFunction);
    if (nextDataIndex > paIOBlock->getReadSize()) {
      break;
    }
    switch (it.mFunction){
      case eCoil:
        nrVals += modbus_read_bits(paModbusConn, it.mStartAddress, it.mNrAddresses, &pData[dataIndex]);
        break;
      case eDiscreteInput:
        nrVals += modbus_read_input_bits(paModbusConn, it.mStartAddress, it.mNrAddresses, &pData[dataIndex]);
        break;
      case eHoldingRegister:
        nrVals += modbus_read_registers(paModbusConn, it.mStartAddress, it.mNrAddresses, (uint16_t*)&pData[dataIndex]);
        break;
      case eInputRegister:
        nrVals += modbus_read_input_registers(paModbusConn, it.mStartAddress, it.mNrAddresses, (uint16_t*)&pData[dataIndex]);
        break;
      default:
        //TODO Error
        break;
    }
    dataIndex = nextDataIndex;
  }

  if (nrVals > 0) {
    mModbusHandler->executeComCallback(paIOBlock->getParent());
  }

  return nrVals;
}
