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
#ifndef _MODBUSTIMEDEVENT_H_
#define _MODBUSTIMEDEVENT_H_

#include "timerha.h"
#include <modbus.h>

class CModbusTimedEvent{
  public:
    explicit CModbusTimedEvent(TForteUInt32 pa_nUpdateInterval); // UpdateInterval = 0 => single shot event
    virtual ~CModbusTimedEvent(){
    }

    void setUpdateInterval(TForteUInt32 pa_nUpdateInterval);
    TForteUInt32 getUpdateInterval() const{
      return m_nUpdateInterval;
    }

    void activate();
    void deactivate();

    bool isStarted() const {
      return m_bIsStarted;
    }

    bool readyToExecute() const;

    // Classes impementing this should call restartTimer in executeEvent
    virtual int executeEvent(modbus_t* pa_pModbusConn, void* pa_pRetVal) = 0;

  protected:
    void restartTimer();

  private:
    uint_fast64_t m_nStartTime;

    TForteUInt32 m_nUpdateInterval; // Polling interval in milliseconds (0 => single shot event)

    bool m_bSingleShotEvent;
    bool m_bIsStarted;
};

#endif // _MODBUSTIMEDEVENT_H_
