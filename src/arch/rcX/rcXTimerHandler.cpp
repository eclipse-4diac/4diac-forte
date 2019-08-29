/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <fortenew.h>
#include "rcXTimerHandler.h"
#include "../devlog.h"


CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& pa_poDeviceExecution){
  return new CrcXTimerHandler(pa_poDeviceExecution);
}


CrcXTimerHandler::CrcXTimerHandler(CDeviceExecution& pa_poDeviceExecution) : CTimerHandler(pa_poDeviceExecution),
  mTimer(0), mFirstTime(true){
  mTimer = forte_malloc(RX_TIMER_SIZE);
  if(0 == mTimer){
    DEVLOG_ERROR("Not enough memory to allocate %l bytes for creating a new timer\n", RX_TIMER_SIZE);
  }
}

CrcXTimerHandler::~CrcXTimerHandler(){
  rX_TimStopTimer(mTimer);
  rX_TimDeleteTimer(mTimer);
  forte_free(mTimer);
}

void CrcXTimerHandler::timerCallback(void* arguments){
  if(arguments){
    static_cast<CTimerHandler*>(arguments)->nextTick();
  }
}

void CrcXTimerHandler::enableHandler(void){
  if (mFirstTime){
    rX_TimCreateTimer(mTimer, timerCallback, (void*) this, RX_TIM_AUTO_RELOAD, (1000000 / rX_SysGetSystemCycletime()) / getTicksPerSecond(),
            (1000000 / rX_SysGetSystemCycletime()) / getTicksPerSecond());
    mFirstTime = false;
  }else{
    rX_TimResetTimer(mTimer);
  }
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
