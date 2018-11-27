/*******************************************************************************
  * Copyright (c) 2005 - 2012 ACIN, Profactor GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Thomas Strasser, Ingo Hegny
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "devexec.h"
#include "esfb.h"
#include "ecet.h"
#include "extevhan.h"
#include "../arch/timerha.h"
#include "../arch/devlog.h"

CDeviceExecution::CDeviceExecution(){
  memset(mRegisteredEventHandlers,  0, sizeof(SEventHandlerElement) * cg_unNumberOfHandlers);

  CDeviceExecution::createHandlers(*this);

  getTimer().enableHandler();
  if(0 == CTimerHandler::smFORTETimer){
    CTimerHandler::smFORTETimer = &getTimer(); //used for develog, CIEC_TIME() and in Modbus
  }
}

CDeviceExecution::~CDeviceExecution(){
  if(CTimerHandler::smFORTETimer == (&getTimer())){
    CTimerHandler::smFORTETimer = 0;
  }

  for(size_t i = 0; i < cg_unNumberOfHandlers; i++){
    if(0 != mRegisteredEventHandlers[i].mHandler){ //for the test cases, only the timer handler is created
      mRegisteredEventHandlers[i].mHandler->disableHandler();
      delete mRegisteredEventHandlers[i].mHandler;
      mRegisteredEventHandlers[i].mHandler = 0;
    }
  }
}

void CDeviceExecution::startNewEventChain(CEventSourceFB *pa_poECStartFB){
  // maybe in the future here has to be added something for handling priority adaption and stuff like this.
  if(0 != pa_poECStartFB){
    CEventChainExecutionThread *poEventChainExecutor = pa_poECStartFB->getEventChainExecutor();
    if(0 != poEventChainExecutor){
      poEventChainExecutor->startEventChain(pa_poECStartFB->getEventSourceEventEntry());
    }else{
      DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event has no CEventChainExecutionThread");
    }
  }else{
    DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event source was null");
  }
}

CExternalEventHandler* CDeviceExecution::getExtEvHandler(size_t paIdentifer) const{
  return mRegisteredEventHandlers[paIdentifer].mHandler;
}

CTimerHandler& CDeviceExecution::getTimer() const{
  return *static_cast<CTimerHandler*>(mRegisteredEventHandlers[0].mHandler);
}
