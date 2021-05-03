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
#include "modbustimedevent.h"
#include "../../core/iec61131_functions.h"

CModbusTimedEvent::CModbusTimedEvent(TForteUInt32 pa_nUpdateInterval) 
  : m_bIsStarted(false){
  m_nUpdateInterval = pa_nUpdateInterval;

  if(pa_nUpdateInterval == 0){
    m_bSingleShotEvent = true;

    activate();
  }
  else
    m_bSingleShotEvent = false;
}

void CModbusTimedEvent::setUpdateInterval(TForteUInt32 pa_nUpdateInterval){
  m_nUpdateInterval = pa_nUpdateInterval;
}

void CModbusTimedEvent::activate(){
  m_nStartTime = NOW_MONOTONIC().getInMilliSeconds();
  m_bIsStarted = true;
}

void CModbusTimedEvent::deactivate(){
  m_bIsStarted = false;
}

bool CModbusTimedEvent::readyToExecute() const{
  uint_fast64_t currentTime = NOW_MONOTONIC().getInMilliSeconds();
  if (m_nUpdateInterval > currentTime) {
    return false;
  }
  currentTime -= m_nUpdateInterval;

  if(isStarted() && (currentTime > m_nStartTime || currentTime == m_nStartTime)){
    return true;
  }

  return false;
}

void CModbusTimedEvent::restartTimer(){

  activate();
}
