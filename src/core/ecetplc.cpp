/*******************************************************************************
 * Copyright (c) 2020 TU Wien ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *    Martin Melik Merkumians - move shared methods to abstract base class
 *******************************************************************************/
#include <forte_config.h>
#include <fortenew.h>
#include "ecetplc.h"
#include "esfb.h"
#include "utils/criticalregion.h"
#include "../arch/devlog.h"
#include "utils/cycletimer.h"
#include <limits.h>

bool firstExecDone = false;

CEventChainExecutionThread61131::CEventChainExecutionThread61131() {
  clear();
}

CEventChainExecutionThread61131::~CEventChainExecutionThread61131() {

}

bool CEventChainExecutionThread61131::getExeDone() {
  return exeDone;
}

void CEventChainExecutionThread61131::setCycleTime(TForteUInt16 time_ms) {
  cycle_time = time_ms;
}

void CEventChainExecutionThread61131::run(void) {
  exeDone = false;
  CCycleTimer cyc_tim(this, cycle_time);
  TEventEntryPtr *m_startpos = &mEventList[cg_nEventChainEventListSize - 1];
  cyc_tim.start();
  while(isAlive()) {
    if((mEventListEnd == mEventListStart) && firstExecDone) {
      exeDone = true;
      selfSuspend();
      mEventListStart = m_startpos;
      cyc_tim.start();
      exeDone = false;
    } else {
      if(0 != *mEventListStart) {
        if(!firstExecDone) {
          m_startpos = mEventListStart;
          firstExecDone = true;
        }
        (*mEventListStart)->mFB->receiveInputEvent((*mEventListStart)->mPortId, *this);
      }
      if(mEventListStart == &mEventList[0]) {
        //wrap the ringbuffer
        mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];
      } else {
        mEventListStart--;
        if(!firstExecDone)
          mEventListStart--;
      }
    }
  }
}

void CEventChainExecutionThread61131::clear(void) {
  clearEventList();
  mInitEventListEnd = mInitEventListStart = &mInitEventList[cg_nEventChainEventListSize - 1];
}
void CEventChainExecutionThread61131::startEventChain(SEventEntry*) {
  mProcessingEvents = true;
}

void CEventChainExecutionThread61131::addInitEventEntry(SEventEntry *paEventToAdd) {
  if(0 == *mInitEventListEnd) {
    *mInitEventListEnd = paEventToAdd;
    TEventEntryPtr *pstNextEventListElem;

    if(mInitEventListEnd == &mInitEventList[0]) {
      //wrap the ringbuffer
      pstNextEventListElem = &mInitEventList[cg_nEventChainEventListSize - 1];
    } else {
      pstNextEventListElem = (mInitEventListEnd - 1);
    }

    if(mInitEventListStart != pstNextEventListElem) {
      //the list is not full
      mInitEventListEnd = pstNextEventListElem;
    }
  } else {
    DEVLOG_ERROR("Event queue is full, event dropped!\n");
  }
}

void CEventChainExecutionThread61131::changeExecutionState(EMGMCommandType paCommand) {
  switch(paCommand){
    case cg_nMGM_CMD_Start:
      if(!isAlive()) {
        //only start the thread when we are not already running
        //Executing events for initialization
        mInitEventListStart = &mInitEventList[cg_nEventChainEventListSize - 1];
        while(0 != *mInitEventListStart) {
          (*mInitEventListStart)->mFB->receiveInputEvent((*mInitEventListStart)->mPortId, *this);
          mInitEventListStart--;
        }
        start();
      }
      break;
    case cg_nMGM_CMD_Kill:
      clear();
    case cg_nMGM_CMD_Stop:
      setAlive(false); //end thread in both cases
      resumeSelfSuspend();
      break;
    default:
      break;
  }
}
