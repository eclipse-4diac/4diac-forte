/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <fortenew.h>
#include "rcXTimerHandler.h"
#include "../devlog.h"

void CTimerHandler::createTimerHandler(void){
  if(0 == sm_poFORTETimer)
    sm_poFORTETimer = new CrcXTimerHandler();
}

CrcXTimerHandler::CrcXTimerHandler() : mTimer(0){
  mTimer = new char[RX_TIMER_SIZE];
  if(0 == mTimer){
    DEVLOG_ERROR("Not enough memory to allocate %l bytes for creating a new timer\n", RX_TIMER_SIZE);
  }else{
    rX_TimCreateTimer(mTimer, timerCallback, (void*) &sm_poFORTETimer, RX_TIM_AUTO_RELOAD, (1000000 / rX_SysGetSystemCycletime()) / getTicksPerSecond(),
        (1000000 / rX_SysGetSystemCycletime()) / getTicksPerSecond());
  }
}

CrcXTimerHandler::~CrcXTimerHandler(){
  rX_TimStopTimer(mTimer);
  rX_TimDeleteTimer(mTimer);
  delete[] mTimer;
}

void CrcXTimerHandler::timerCallback(void* arguments){
  CTimerHandler* forteTimer = static_cast<CTimerHandler*>(arguments); //TODO: check if using directly the static sm_poFORTETimer instead of the argument is more efficient
  forteTimer->nextTick();
}

void CrcXTimerHandler::enableHandler(void){
  rX_TimResetTimer(mTimer);
}

void CrcXTimerHandler::disableHandler(void){
  rX_TimStopTimer(mTimer);
}

void CrcXTimerHandler::setPriority(int ){
  //TODO think on how to handle this.
}

int CrcXTimerHandler::getPriority(void) const {
  //TODO think on how to handle this.
  return 1;
}
