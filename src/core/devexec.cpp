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

CDeviceExecution::CDeviceExecution(){
  memset(mRegisteredEventHandlers,  0, sizeof(SEventHandlerElement) * cg_unNumberOfHandlers);

  CDeviceExecution::createHandlers(*this);

  getTimer().enableHandler();
  if(0 == CTimerHandler::sm_poFORTETimer){
    CTimerHandler::sm_poFORTETimer = &getTimer(); //used for develog, CIEC_TIME() and in Modbus
  }
}

CDeviceExecution::~CDeviceExecution(){
  if(CTimerHandler::sm_poFORTETimer == (&getTimer())){
    CTimerHandler::sm_poFORTETimer = 0;
  }

  for(unsigned int i = 0; i < cg_unNumberOfHandlers; i++){
    if(0 != mRegisteredEventHandlers[i].m_poHandler){ //for the test cases, only the timer handler is created
      mRegisteredEventHandlers[i].m_poHandler->disableHandler();
      delete mRegisteredEventHandlers[i].m_poHandler;
    }
  }
}

void CDeviceExecution::startNewEventChain(CEventSourceFB *pa_poECStartFB){
	// maybe in the future here has to be added something for handling priority adaption and stuff like this.
  if(0 != pa_poECStartFB){
    CEventChainExecutionThread *poEventChainExecutor = pa_poECStartFB->getEventChainExecutor();
    if(0 != poEventChainExecutor){
      poEventChainExecutor->startEventChain(pa_poECStartFB->getEventSourceEventEntry());
    }
      // TODO add log output if one if these two ifs is not met
  }
}

CExternalEventHandler* CDeviceExecution::getHandler(unsigned int paIdentifer) const{
  return mRegisteredEventHandlers[paIdentifer].m_poHandler;
}

CTimerHandler& CDeviceExecution::getTimer() const{
  return *static_cast<CTimerHandler*>(mRegisteredEventHandlers[0].m_poHandler);
}
