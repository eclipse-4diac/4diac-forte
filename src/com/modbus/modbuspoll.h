/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef MODBUSPOLL_H_
#define MODBUSPOLL_H_

#include "modbustimedevent.h"
#include <fortelist.h>

class CModbusPoll : public CModbusTimedEvent{
  public:
    CModbusPoll(TForteUInt32 pa_nPollInterval, unsigned int pa_nFunctionCode, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses);
    ~CModbusPoll();

    int executeEvent(modbus_t *pa_pModbusConn, void *pa_pRetVal);

    void setFunctionCode(unsigned int pa_nFunctionCode){
      m_nFunctionCode = pa_nFunctionCode;
    }
    unsigned int getFunctionCode(){
      return m_nFunctionCode;
    }

    void addPollAddresses(unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses);

  private:

    struct SModbusPollData{
        unsigned int m_nStartAddress;
        unsigned int m_nNrAddresses;

        SModbusPollData(unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses) :
            m_nStartAddress(pa_nStartAddress), m_nNrAddresses(pa_nNrAddresses){
        }
        ;
    };

    unsigned int m_nFunctionCode;

    CSinglyLinkedList<SModbusPollData*> m_lPolls;
};

#endif /* MODBUSPOLL_H_ */
