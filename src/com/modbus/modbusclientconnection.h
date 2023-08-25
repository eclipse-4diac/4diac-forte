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
      explicit CModbusConnectionEvent(long paReconnectInterval, EModbusFlowControl paFlowControl, const char *paDevice); //ReconnectInterval = 0 => only one connection try
      ~CModbusConnectionEvent() override = default;

      int executeEvent(modbus_t *paModbusConn, void *paRetVal) override;

    private:
      EModbusFlowControl mFlowControl;
      char mDevice[256];
  };
}

class CModbusClientConnection : public CModbusConnection{
  public:
    explicit CModbusClientConnection(CModbusHandler* pa_modbusHandler);
    ~CModbusClientConnection() override;

    int readData(CModbusIOBlock* paIOBlock, void* paData, unsigned int paMaxDataSize) override;
    void writeDataRange(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses, const void *paData);
    int connect() override;
    void disconnect() override;

    void addNewPoll(long paPollInterval, CModbusIOBlock* paIOBlock);

    void setSlaveId(unsigned int paSlaveId);

  protected:
    void run() override;

  private:
    void tryConnect();
    void tryPolling();

    modbus_connection_event::CModbusConnectionEvent *mModbusConnEvent;

    typedef std::vector<CModbusPoll*> TModbusPollList;
    TModbusPollList mPollList;

    unsigned int mSlaveId;

    CSyncObject mModbusLock;
};

#endif
