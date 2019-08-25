/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortenew.h>
#include "ecostiha.h"
#include "../../core/devexec.h"

cyg_sem_t CECOSTimerHandler::m_stSemaphore;

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& pa_poDeviceExecution){
  return new CECOSTimerHandler(pa_poDeviceExecution);
}

CECOSTimerHandler::CECOSTimerHandler(CDeviceExecution& pa_poDeviceExecution) : CTimerHandler(pa_poDeviceExecution)  {
  cyg_semaphore_init(&m_stSemaphore, 0);

  m_stSystemclockHandle = cyg_real_time_clock();
  cyg_clock_to_counter(m_stSystemclockHandle, &m_stCounterHandle);

  cyg_alarm_create(m_stCounterHandle, timerHandlerFunc, (cyg_addrword_t) 0, &m_stAlarmHandle, &m_stAlarm);
  start();
}

CECOSTimerHandler::~CECOSTimerHandler(){
  cyg_semaphore_destroy(&m_stSemaphore);
}

void CECOSTimerHandler::enableHandler(void){
  cyg_alarm_initialize(m_stAlarmHandle, cyg_current_time() + 1, 1);
}

void CECOSTimerHandler::disableHandler(void){
  cyg_alarm_disable(m_stAlarmHandle);
}

void CECOSTimerHandler::setPriority(int ){

}

int CECOSTimerHandler::getPriority(void) const{
  return 0;
}

void CECOSTimerHandler::run(void){
  CECOSThread::setPriority(0); //we want to be a very important thread
  while(isAlive()){
    cyg_semaphore_wait(&m_stSemaphore);
    //FIXME add compensation code for timer activation jitter similar to the code in the posix architecture
    nextTick();
  }
}

