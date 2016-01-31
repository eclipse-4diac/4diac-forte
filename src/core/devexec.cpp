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
#include "../arch/timerha.h"
#include "ecet.h"
#include "extevhan.h"



CDeviceExecution::CDeviceExecution():
  m_nNumberofExternalEventHandler(0){
  CExternalEventHandler::setDeviceExecution(this); // let all the external event handlers know where to report events to
  CTimerHandler::createTimerHandler();
  CTimerHandler::sm_poFORTETimer->enableHandler(); // will result in an registration for  the timer handler (automatically it will be the first registration)
}

CDeviceExecution::~CDeviceExecution(){
  delete CTimerHandler::sm_poFORTETimer;
  CTimerHandler::sm_poFORTETimer = 0;
};

int CDeviceExecution::registerExternalEventHandler(CExternalEventHandler *pa_poHandler){
  int retval = -1;
  if(cg_MaxRegisteredEventHandlers > m_nNumberofExternalEventHandler){
    retval = m_nNumberofExternalEventHandler;
    m_astRegisteredEventHandlers[retval].m_poHandler = pa_poHandler;
    m_astRegisteredEventHandlers[retval].m_bOccured = false;
    ++m_nNumberofExternalEventHandler;
  }
  return retval;
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

