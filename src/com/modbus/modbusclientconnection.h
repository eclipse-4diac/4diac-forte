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
#ifndef _MODBUSCLIENTCONNECTION_H_
#define _MODBUSCLIENTCONNECTION_H_

#include <vector>
#include "modbusconnection.h"
#include "modbustimedevent.h"

class CModbusPoll;

namespace modbus_connection_event {
  class CModbusConnectionEvent : public CModbusTimedEvent{
    public:
      explicit CModbusConnectionEvent(long pa_nReconnectInterval, EModbusFlowControl pa_enFlowControl, const char *pa_acDevice); //ReconnectInterval = 0 => only one connection try
      ~CModbusConnectionEvent() override = default;

      int executeEvent(modbus_t *pa_pModbusConn, void *pa_pRetVal) override;

    private:
      EModbusFlowControl m_enFlowControl;
      char m_acDevice[256];
  };
}

class CModbusClientConnection : public CModbusConnection{
  public:
    explicit CModbusClientConnection(CModbusHandler* pa_modbusHandler);
    ~CModbusClientConnection() override;

    int readData(CModbusIOBlock* pa_pIOBlock, void* pa_pData, unsigned int pa_nMaxDataSize) override;
    void writeDataRange(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses, const void *pa_pData);
    int connect() override;
    void disconnect() override;

    void addNewPoll(long pa_nPollInterval, CModbusIOBlock* pa_pIOBlock);

    void setSlaveId(unsigned int pa_nSlaveId);

  protected:
    void run() override;

  private:
    void tryConnect();
    void tryPolling();

    modbus_connection_event::CModbusConnectionEvent *m_pModbusConnEvent;

    typedef std::vector<CModbusPoll*> TModbusPollList;
    TModbusPollList m_lstPollList;

    unsigned int m_nSlaveId;

    CSyncObject m_oModbusLock;
};

#endif
