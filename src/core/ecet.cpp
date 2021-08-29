 /*******************************************************************************
  * Copyright (c) 2005, 2021 ACIN, Profactor GmbH, fortiss GmbH, Hit robot group
  *
  * This program and the accompanying materials are made available under the
  * terms of the Eclipse Public License 2.0 which is available at
  * http://www.eclipse.org/legal/epl-2.0.
  *
  * SPDX-License-Identifier: EPL-2.0
  *
  * Contributors:
  *    Alois Zoitl, Thomas Strasser, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny
  *    Micheal Hofmann
  *      - initial implementation and rework communication infrastructure
  *    zhaoxin
  *      -  fix that external event queue becomes event locker after it is full
  *******************************************************************************/
#include <forte_config.h>
#include <fortenew.h>
#include "ecet.h"
#include "esfb.h"
#include "utils/criticalregion.h"
#include "../arch/devlog.h"

CEventChainExecutionThread::CEventChainExecutionThread() :
    CThread(), mSuspendSemaphore(0), mProcessingEvents(false){
  clear();
}

CEventChainExecutionThread::~CEventChainExecutionThread(){
}

void CEventChainExecutionThread::run(void){
  while(isAlive()){ //thread is allowed to execute
    mainRun();
  }
}

void CEventChainExecutionThread::mainRun(){
  if(externalEventOccured()){
    transferExternalEvents();
  }
  if(mEventListEnd == mEventListStart){
    mProcessingEvents = false;
    selfSuspend();
    mProcessingEvents = true; //set this flag here to true as well in case the suspend just went through and processing was not finished
  }
  else{
    if(0 != *mEventListStart){
      (*mEventListStart)->mFB->receiveInputEvent((*mEventListStart)->mPortId, this);
    }
    *mEventListStart = 0;

    if(mEventListStart == &mEventList[0]){
      //wrap the ringbuffer
      mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];
    }
    else{
      mEventListStart--;
    }
  }
}

void CEventChainExecutionThread::clear(void){
  memset(mEventList, 0, cg_nEventChainEventListSize * sizeof(TEventEntryPtr));
  mEventListEnd = mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];

  {
    CCriticalRegion criticalRegion(mExternalEventListSync);
    memset(mExternalEventList, 0, cg_nEventChainExternalEventListSize * sizeof(TEventEntryPtr));
    mExternalEventListEnd = mExternalEventListStart = &mExternalEventList[cg_nEventChainExternalEventListSize - 1];
  }
}

void CEventChainExecutionThread::transferExternalEvents(){
  CCriticalRegion criticalRegion(mExternalEventListSync);
  //this while is built in a way that it checks also if we got here by accident
  while(mExternalEventListStart != mExternalEventListEnd){
    if(0 != *mExternalEventListStart){
      //add only valid entries
      addEventEntry(*mExternalEventListStart);
      *mExternalEventListStart = 0;

      if(mExternalEventListStart == &mExternalEventList[0]){
        //wrap the ringbuffer
        mExternalEventListStart = &mExternalEventList[cg_nEventChainExternalEventListSize - 1];
      }
      else{
        mExternalEventListStart--;
      }
    }
  }
  //the queue is full and pop out last one
  if(0 != *mExternalEventListStart){
      addEventEntry(*mExternalEventListStart);
      *mExternalEventListStart = 0;
  }
}

void CEventChainExecutionThread::startEventChain(SEventEntry *paEventToAdd){
  FORTE_TRACE("CEventChainExecutionThread::startEventChain\n");
  {
    CCriticalRegion criticalRegion(mExternalEventListSync);
    if(0 == *mExternalEventListEnd){
      *mExternalEventListEnd = paEventToAdd;
      TEventEntryPtr* pstNextEventListElem;

      if(mExternalEventListEnd == &mExternalEventList[0]){
        //wrap the ringbuffer
        pstNextEventListElem = &mExternalEventList[cg_nEventChainExternalEventListSize - 1];
      }
      else{
        pstNextEventListElem = (mExternalEventListEnd - 1);
      }

      if(mExternalEventListStart != pstNextEventListElem){
        //the list is not full
        mExternalEventListEnd = pstNextEventListElem;
      }
      mProcessingEvents = true;
      resumeSelfSuspend();
    }
    else{
      DEVLOG_ERROR("External event queue is full, external event dropped!\n");
    }
  } // End critical region
}

void CEventChainExecutionThread::addEventEntry(SEventEntry *paEventToAdd){
  if(0 == *mEventListEnd){
    *mEventListEnd = paEventToAdd;
    TEventEntryPtr* pstNextEventListElem;

    if(mEventListEnd == &mEventList[0]){
      //wrap the ringbuffer
      pstNextEventListElem = &mEventList[cg_nEventChainEventListSize - 1];
    }
    else{
      pstNextEventListElem = (mEventListEnd - 1);
    }

    if(mEventListStart != pstNextEventListElem){
      //the list is not full
      mEventListEnd = pstNextEventListElem;
    }
  }
  else{
    DEVLOG_ERROR("Event queue is full, event dropped!\n");
  }
}

void CEventChainExecutionThread::changeExecutionState(EMGMCommandType paCommand){
  switch (paCommand){
    case cg_nMGM_CMD_Start:
      if(!isAlive()){
        //only start the thread when we are not already running
        start();
      }
      break;
    case cg_nMGM_CMD_Kill:
      clear();
      // fall through
    case cg_nMGM_CMD_Stop:
      setAlive(false); //end thread in both cases
      resumeSelfSuspend();
      break;
    default:
      break;
  }
}

