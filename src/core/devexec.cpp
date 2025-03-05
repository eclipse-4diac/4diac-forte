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
#include "device.h"

CDeviceExecution::~CDeviceExecution() {
  // FIXME: deleting a handler should disable it automatically
  disableHandlers();
}

void CDeviceExecution::startNewEventChain(CEventSourceFB* paECStartFB) const {
  // maybe in the future here has to be added something for handling priority adaption and stuff like this.
  if(nullptr != paECStartFB) {
    CEventChainExecutionThread *poEventChainExecutor = paECStartFB->getEventChainExecutor();
    if(nullptr != poEventChainExecutor) {
      poEventChainExecutor->startEventChain(*paECStartFB->getEventSourceEventEntry());
    } else {
      DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event has no CEventChainExecutionThread");
    }
  } else {
    DEVLOG_ERROR("[CDeviceExecution] Couldn't start new event chain because the event source was null");
  }
}

CExternalEventHandler* CDeviceExecution::getExtEvHandler(size_t paIdentifer) const {
  return mRegisteredEventHandlers[paIdentifer].mHandler.get();
}

CTimerHandler& CDeviceExecution::getTimer() const {
  return *static_cast<CTimerHandler*>(mRegisteredEventHandlers[0].mHandler.get());
}

void CDeviceExecution::disableHandlers() {
  for (auto &handler : mRegisteredEventHandlers) {
    if (handler.mHandler) {
      handler.mHandler->disableHandler();
    }
  }
}

CDevice& CDeviceExecution::getDevice(){
  return mDevice;
}
