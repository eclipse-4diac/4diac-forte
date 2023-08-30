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

cyg_sem_t CECOSTimerHandler::mSemaphore;

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& paDeviceExecution){
  return new CECOSTimerHandler(paDeviceExecution);
}

CECOSTimerHandler::CECOSTimerHandler(CDeviceExecution& paDeviceExecution) : CTimerHandler(paDeviceExecution)  {
  cyg_semaphore_init(&mSemaphore, 0);

  mSystemclockHandle = cyg_real_time_clock();
  cyg_clock_to_counter(mSystemclockHandle, &mCounterHandle);

  cyg_alarm_create(mCounterHandle, timerHandlerFunc, (cyg_addrword_t) 0, &mAlarmHandle, &mAlarm);
  start();
}

CECOSTimerHandler::~CECOSTimerHandler(){
  cyg_semaphore_destroy(&mSemaphore);
}

void CECOSTimerHandler::enableHandler(){
  cyg_alarm_initialize(mAlarmHandle, cyg_current_time() + 1, 1);
}

void CECOSTimerHandler::disableHandler(){
  cyg_alarm_disable(mAlarmHandle);
}

void CECOSTimerHandler::setPriority(int ){

}

int CECOSTimerHandler::getPriority() const{
  return 0;
}

void CECOSTimerHandler::run(){
  CECOSThread::setPriority(0); //we want to be a very important thread
  while(isAlive()){
    cyg_semaphore_wait(&mSemaphore);
    //FIXME add compensation code for timer activation jitter similar to the code in the posix architecture
    nextTick();
  }
}

