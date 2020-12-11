/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Ingo Hegny
 *      - initial API and implementation and/or initial documentation
 *   Alois Zoitl - worked on reducing the jitter and overhead of timer handler
 *                 Bug #568902
 *******************************************************************************/
#include "timerha.h"
#include "../core/datatypes/forte_time.h"
#include "../core/devexec.h"
#include "../core/esfb.h"
#include "../core/utils/criticalregion.h"
#include <algorithm>
#include <functional>

DEFINE_HANDLER(CTimerHandler)

CTimerHandler::CTimerHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    mForteTime(0), mTimedFBList(nullptr), mAddFBList(nullptr){
}

CTimerHandler::~CTimerHandler(){
}

void CTimerHandler::registerTimedFB(STimedFBListEntry *paTimerListEntry, const CIEC_TIME &paTimeInterval) {
  //calculate the correct interval based on time-base and timer ticks per seconds
  paTimerListEntry->mInterval = static_cast<TForteUInt32>((paTimeInterval * getTicksPerSecond()) / cgForteTimeBaseUnitsPerSecond);
  // Correct null intervals that can lead to event queue overflow to at least 1 timer tick
  if(0 == paTimerListEntry->mInterval) {
    paTimerListEntry->mInterval = 1;
  }
  // set the first next activation time right here to reduce jitter, see Bug #568902 for details
  paTimerListEntry->mTimeOut = mForteTime + paTimerListEntry->mInterval;
  {
    CCriticalRegion criticalRegion(mAddListSync);
    paTimerListEntry->mNext = mAddFBList;
    mAddFBList = paTimerListEntry;
  }
}

void CTimerHandler::addTimedFBEntry(STimedFBListEntry *paTimerListEntry) {
  paTimerListEntry->mNext = 0;
  if (0 == mTimedFBList) {
    mTimedFBList = paTimerListEntry;
  } else {
    if (mTimedFBList->mTimeOut > paTimerListEntry->mTimeOut) {
      paTimerListEntry->mNext = mTimedFBList;
      mTimedFBList = paTimerListEntry;
    } else {
      STimedFBListEntry *runner = mTimedFBList;
      while (0 != runner->mNext) {
        if (runner->mNext->mTimeOut > paTimerListEntry->mTimeOut) {
          paTimerListEntry->mNext = runner->mNext;
          runner->mNext = paTimerListEntry;
          break;
        }
        runner = runner->mNext;
      }
      runner->mNext = paTimerListEntry;
    }
  }
}

void CTimerHandler::unregisterTimedFB(CEventSourceFB *paTimedFB) {
  CCriticalRegion criticalRegion(mRemoveListSync);
  mRemoveFBList.push_back(paTimedFB);
}

void CTimerHandler::removeTimedFB(CEventSourceFB *paTimedFB) {
  if (0 != mTimedFBList) {
    STimedFBListEntry *buffer = 0;
    if (mTimedFBList->mTimedFB == paTimedFB) {
      buffer = mTimedFBList;
      mTimedFBList = mTimedFBList->mNext;
      buffer->mNext = 0;
      buffer->mTimeOut = 0;
    } else {
      STimedFBListEntry *runner = mTimedFBList;
      while (0 != runner->mNext) {
        if (runner->mNext->mTimedFB == paTimedFB) {
          buffer = runner->mNext;
          runner->mNext = runner->mNext->mNext;
          buffer->mNext = 0;
          buffer->mTimeOut = 0;
          break;
        }
        runner = runner->mNext;
      }
    }
  }
}

void CTimerHandler::nextTick(void) {
  ++mForteTime;
  mDeviceExecution.notifyTime(mForteTime); //notify the device execution that one tick passed by.

  if(!mRemoveFBList.empty()){
    processRemoveList();
  }

  processTimedFBList();

  if(0 != mAddFBList){
    processAddList();
  }
}

void  CTimerHandler::processTimedFBList(){
  while (0 != mTimedFBList) {
    if (mTimedFBList->mTimeOut > mForteTime) {
      break;
    }
    STimedFBListEntry *buffer = mTimedFBList;
    mTimedFBList = buffer->mNext;  //remove buffer from the list
    triggerTimedFB(buffer);
  }
}

void CTimerHandler::triggerTimedFB(STimedFBListEntry *paTimerListEntry){
  mDeviceExecution.startNewEventChain(paTimerListEntry->mTimedFB);

  switch (paTimerListEntry->mType) {
    case e_Periodic:
      paTimerListEntry->mTimeOut = mForteTime + paTimerListEntry->mInterval;  // the next activation time of this FB
      addTimedFBEntry(paTimerListEntry); //re-register the timed FB
      break;
    case e_SingleShot:
      // nothing special is to do up to now, therefore go to default
    default:
      paTimerListEntry->mNext = 0;
      paTimerListEntry->mTimeOut = 0;
      break;
  }
}

void CTimerHandler::processAddList(){
  CCriticalRegion criticalRegion(mAddListSync);
  while(0 != mAddFBList){
    STimedFBListEntry *buffer = mAddFBList;
    mAddFBList = buffer->mNext; //remove buffer from the list
    if(buffer->mTimeOut < mForteTime){
      // the time already passed trigger the fb
      triggerTimedFB(buffer);
    }
    else{
      addTimedFBEntry(buffer);
    }
  }
}

void CTimerHandler::processRemoveList(){
  CCriticalRegion criticalRegion(mRemoveListSync);
  std::for_each(mRemoveFBList.begin(), mRemoveFBList.end(), std::bind1st(std::mem_fun(&CTimerHandler::removeTimedFB), this));
  mRemoveFBList.clear();
}


