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
    CThread(), mSuspendSemaphore(false), mProcessingEvents(false){
  clear();
}

CEventChainExecutionThread::~CEventChainExecutionThread() = default;

void CEventChainExecutionThread::run(){
  while(isAlive()){ //thread is allowed to execute
    mainRun();
  }
}

void CEventChainExecutionThread::mainRun(){
  if(externalEventOccured()){
    transferExternalEvents();
  }
  if(mEventListEnd == mEventListStart && mEventListEnd->mFB == nullptr){
    mProcessingEvents = false;
    selfSuspend();
    mProcessingEvents = true; //set this flag here to true as well in case the suspend just went through and processing was not finished
  }
  else{
    if(nullptr != mEventListStart->mFB){
      mEventListStart->mFB->receiveInputEvent(mEventListStart->mPortId, this);
    }
    mEventListStart->mFB = nullptr;
    if(mEventListEnd == mEventListStart){ 
      return;//let's suspend next time call mainrun to reduce duplicate code
    }

    if(mEventListStart == &mEventList[0]){
      //wrap the ringbuffer
      mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];
    }
    else{
      mEventListStart--;
    }
  }
}

void CEventChainExecutionThread::clear(){
  for(ssize_t i = 0; i < cg_nEventChainEventListSize; i++){
    mEventList[i].mFB = nullptr;
  }
  mEventListEnd = mEventListStart = &mEventList[cg_nEventChainEventListSize - 1];

  {
    CCriticalRegion criticalRegion(mExternalEventListSync);
    for(ssize_t i = 0; i < cg_nEventChainExternalEventListSize; i++){
      mExternalEventList[i].mFB = nullptr;
    }
    mExternalEventListEnd = mExternalEventListStart = &mExternalEventList[cg_nEventChainExternalEventListSize - 1];
  }
}

void CEventChainExecutionThread::transferExternalEvents(){
  CCriticalRegion criticalRegion(mExternalEventListSync);
  //this while is built in a way that it checks also if we got here by accident
  while(mExternalEventListStart != mExternalEventListEnd){
      if(mExternalEventListStart->mFB != nullptr){
        //add only valid entries
        addEventEntry(*mExternalEventListStart);
        mExternalEventListStart->mFB = nullptr;
      }
      if(mExternalEventListStart == &mExternalEventList[0]){
        //wrap the ringbuffer
        mExternalEventListStart = &mExternalEventList[cg_nEventChainExternalEventListSize - 1];
      }
      else{
        mExternalEventListStart--;
      }
  }
  //the queue is full and pop out last one
  if(mExternalEventListStart->mFB != nullptr){
      addEventEntry(*mExternalEventListStart);
      mExternalEventListStart->mFB = nullptr;
  }
}

void CEventChainExecutionThread::startEventChain(TEventEntry paEventToAdd){
  FORTE_TRACE("CEventChainExecutionThread::startEventChain\n");
  {
    CCriticalRegion criticalRegion(mExternalEventListSync);
    if(mExternalEventListEnd->mFB == nullptr){
      *mExternalEventListEnd = paEventToAdd;
      TEventEntryPtr pstNextEventListElem;

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

void CEventChainExecutionThread::addEventEntry(TEventEntry paEventToAdd){
  if(mEventListEnd->mFB == nullptr){
    *mEventListEnd = paEventToAdd;
    TEventEntryPtr pstNextEventListElem;

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
    case EMGMCommandType::Start:
      if(!isAlive()){
        //only start the thread when we are not already running
        start();
      }
      break;
    case EMGMCommandType::Kill:
      clear();
      [[fallthrough]];
    case EMGMCommandType::Stop:
      setAlive(false); //end thread in both cases
      resumeSelfSuspend();
      break;
    default:
      break;
  }
}

