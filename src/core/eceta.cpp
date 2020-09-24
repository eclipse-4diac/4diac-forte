/*******************************************************************************
 * Copyright (c) 2019, 2020 TU Wien ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - inital API for virtual interface for several types of
 *       event chain thread execution mechanisms
 *    Martin Melik Merkumians - move shared methods to abstract base class
 *******************************************************************************/

#include "eceta.h"

void CEventChainExecutionThreadAbstract::joinEventChainExecutionThread() {
  CThread::join();
}

void CEventChainExecutionThreadAbstract::addEventEntry(SEventEntry *paEventToAdd) {
  if(0 == *mEventListEnd) {
    *mEventListEnd = paEventToAdd;
    TEventEntryPtr *pstNextEventListElem;

    if(mEventListEnd == &mEventList[0]) {
      //wrap the ringbuffer
      pstNextEventListElem = &mEventList[cg_nEventChainEventListSize - 1];
    } else {
      pstNextEventListElem = (mEventListEnd - 1);
    }

    if(mEventListStart != pstNextEventListElem) {
      //the list is not full
      mEventListEnd = pstNextEventListElem;
    }
  } else {
    DEVLOG_ERROR("Event queue is full, event dropped!\n");
  }
}

void CEventChainExecutionThreadAbstract::resumeSelfSuspend() {
  mSuspendSemaphore.inc();
}

void CEventChainExecutionThreadAbstract::selfSuspend() {
  mSuspendSemaphore.waitIndefinitely();
}

bool CEventChainExecutionThreadAbstract::isProcessingEvents() const {
  return mProcessingEvents;
}

void CEventChainExecutionThreadAbstract::setDeadline(const CIEC_TIME &paVal) {
  CThread::setDeadline(paVal);
}

void CEventChainExecutionThreadAbstract::end(void) {
  setAlive(false);
  resumeSelfSuspend();
  CThread::end();
}

void CEventChainExecutionThreadAbstract::clearEventList() {
  memset(mEventList, 0, cg_nEventChainEventListSize * sizeof(TEventEntryPtr));
  mEventListEnd = mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];
}

