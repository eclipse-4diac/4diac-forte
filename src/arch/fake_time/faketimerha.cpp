/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Christoph Binder - initial implementation
 *******************************************************************************/
#include "faketimerha.h"
#include "../../core/devexec.h"
#include "ecet.h"
#include "device.h"

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution &paDeviceExecution) {
  return new CFakeTimerHandler(paDeviceExecution);
}

CFakeTimerHandler::CFakeTimerHandler(CDeviceExecution &paDeviceExecution) :
  CTimerHandler(paDeviceExecution), sleepTime(0), fakeSleepFb(nullptr) {
}

CFakeTimerHandler::~CFakeTimerHandler() {
  disableHandler();
}

void CFakeTimerHandler::run() {
  while(isAlive()) {
    if(sleepTime > 0) {
      CEventChainExecutionThread *execThread = getExecThread(); //get execThread for every sleep fake, since resource potentially changed
      while(sleepTime > 0) {
        nextTick();
        trackFakeForteTime();
        sleepTime--;
        while(execThread != 0 && execThread->isProcessingEvents()) {
          sleepThread(0);
        }
      }
      startOutputEvent();
    } else {
      sleepThread(1);
    }
  }
}

CEventChainExecutionThread* CFakeTimerHandler::getExecThread() {
    return fakeSleepFb ? fakeSleepFb->getResourcePtr()->getResourceEventExecution() : nullptr;
}

void CFakeTimerHandler::setSleepTime(CIEC_TIME &t, CFunctionBlock *fb) {
  fakeSleepFb = fb;
  if(sleepTime == 0) {
    sleepTime = t.getInMilliSeconds();
    if(sleepTime == 0) {
      startOutputEvent();
    }
  }
}

void CFakeTimerHandler::startOutputEvent() {
  if(fakeSleepFb) {
    CEventChainExecutionThread *execThread = getExecThread();
    if(execThread) {
        fakeSleepFb->receiveInputEvent(cgExternalEventID, execThread);
        execThread->resumeSelfSuspend();
    }
  }
}

void CFakeTimerHandler::enableHandler(void) {
  start();
}

void CFakeTimerHandler::disableHandler(void) {
  end();
}

void CFakeTimerHandler::setPriority(int) {
  // empty implementation
}

int CFakeTimerHandler::getPriority(void) const {
  return 1;
}
