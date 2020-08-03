/*******************************************************************************
 * Copyright (c) 2005 - 2012 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

CDeviceExecution::CDeviceExecution() {
  memset(mRegisteredEventHandlers, 0, sizeof(SEventHandlerElement) * cg_unNumberOfHandlers);

  CDeviceExecution::createHandlers(*this);

  getTimer().enableHandler();
}

CDeviceExecution::~CDeviceExecution() {
  for(size_t i = 0; i < cg_unNumberOfHandlers; i++) {
    if(0 != mRegisteredEventHandlers[i].mHandler) { //for the test cases, only the timer handler is created
      mRegisteredEventHandlers[i].mHandler->disableHandler();
      delete mRegisteredEventHandlers[i].mHandler;
      mRegisteredEventHandlers[i].mHandler = 0;
    }
  }
}

void CDeviceExecution::startNewEventChain(CEventSourceFB* paECStartFB) const {
  // maybe in the future here has to be added something for handling priority adaption and stuff like this.
  if(0 != paECStartFB) {
    CEventChainExecutionThread *poEventChainExecutor = paECStartFB->getEventChainExecutor();
    if(0 != poEventChainExecutor) {
      poEventChainExecutor->startEventChain(paECStartFB->getEventSourceEventEntry());
    } else {
      DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event has no CEventChainExecutionThread");
    }
  } else {
    DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event source was null");
  }
}

CExternalEventHandler* CDeviceExecution::getExtEvHandler(size_t paIdentifer) const {
  return mRegisteredEventHandlers[paIdentifer].mHandler;
}

CTimerHandler& CDeviceExecution::getTimer() const {
  return *static_cast<CTimerHandler*>(mRegisteredEventHandlers[0].mHandler);
}

void CDeviceExecution::disableHandlers() {
  for(size_t i = 0; i < cg_unNumberOfHandlers; i++) {
    if(0 != mRegisteredEventHandlers[i].mHandler) {
      mRegisteredEventHandlers[i].mHandler->disableHandler();
    }
  }
}
