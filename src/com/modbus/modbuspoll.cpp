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

#include <devlog.h>

#include <modbus.h>

CModbusPoll::CModbusPoll(TForteUInt32 pa_nPollInterval, unsigned int pa_nFunctionCode, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses) :
    CModbusTimedEvent(pa_nPollInterval){
  setFunctionCode(pa_nFunctionCode);
  addPollAddresses(pa_nStartAddress, pa_nNrAddresses);
}

CModbusPoll::~CModbusPoll(){
  CSinglyLinkedList<SModbusPollData*>::Iterator itEnd = m_lPolls.end();
  for(CSinglyLinkedList<SModbusPollData*>::Iterator it = m_lPolls.begin(); it != itEnd; ++it){
    delete *it;
  }
  m_lPolls.clearAll();
}

void CModbusPoll::addPollAddresses(unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses){
  m_lPolls.pushBack(new SModbusPollData(pa_nStartAddress, pa_nNrAddresses));
}

int CModbusPoll::executeEvent(modbus_t *pa_pModbusConn, void *pa_pRetVal){
  restartTimer();

  int nrVals = 0;
  CSinglyLinkedList<SModbusPollData*>::Iterator itEnd = m_lPolls.end();
  for(CSinglyLinkedList<SModbusPollData*>::Iterator it = m_lPolls.begin(); it != itEnd; ++it){
    switch (m_nFunctionCode){
      case 1:
        nrVals += modbus_read_bits(pa_pModbusConn, it->m_nStartAddress, it->m_nNrAddresses, &((uint8_t*) pa_pRetVal)[nrVals]);
        break;
      case 2:
        nrVals += modbus_read_input_bits(pa_pModbusConn, it->m_nStartAddress, it->m_nNrAddresses, &((uint8_t*) pa_pRetVal)[nrVals]);
        break;
      case 3:
        nrVals += modbus_read_registers(pa_pModbusConn, it->m_nStartAddress, it->m_nNrAddresses, &((uint16_t*) pa_pRetVal)[nrVals]);
        break;
      case 4:
        nrVals += modbus_read_input_registers(pa_pModbusConn, it->m_nStartAddress, it->m_nNrAddresses, &((uint16_t*) pa_pRetVal)[nrVals]);
        break;
      default:
        //TODO Error
        break;
    }
  }
  return nrVals;
}
