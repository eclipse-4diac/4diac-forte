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

CModbusTimedEvent::CModbusTimedEvent(TForteUInt32 pa_nUpdateInterval) 
  : m_bIsStarted(false){
  m_nUpdateInterval = pa_nUpdateInterval;
  m_oUpdateInterval += (m_nUpdateInterval * cg_nForteTicksPerSecond) / 1000;

  if(pa_nUpdateInterval == 0){
    m_bSingleShotEvent = true;

    activate();
  }
  else
    m_bSingleShotEvent = false;
}

void CModbusTimedEvent::setUpdateInterval(TForteUInt32 pa_nUpdateInterval){
  m_nUpdateInterval = pa_nUpdateInterval;
  SForteTime newTime;
  newTime += (m_nUpdateInterval * cg_nForteTicksPerSecond) / 1000;
  m_oUpdateInterval = newTime;
}

void CModbusTimedEvent::activate(){
  m_oStartTime = CTimerHandler::sm_poFORTETimer->getForteTime();
  m_bIsStarted = true;
}

void CModbusTimedEvent::deactivate(){
  m_bIsStarted = false;
}

bool CModbusTimedEvent::readyToExecute() const{
  SForteTime currentTime = CTimerHandler::sm_poFORTETimer->getForteTime();
  if (m_oUpdateInterval > currentTime) {
    return false;
  }
  currentTime -= m_oUpdateInterval;

  if(isStarted() && (currentTime > m_oStartTime || currentTime == m_oStartTime)){
    return true;
  }

  return false;
}

void CModbusTimedEvent::restartTimer(){

  activate();
}
