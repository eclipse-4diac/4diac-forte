/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "timerha.h"
#include "../core/datatypes/forte_time.h"
#include "../core/devexec.h"
#include "../core/esfb.h"
#include "../core/utils/criticalregion.h"

DEFINE_HANDLER(CTimerHandler)

CTimerHandler::CTimerHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    mForteTime(0), mTimedFBList(0){
}

CTimerHandler::~CTimerHandler(){
}

void CTimerHandler::registerTimedFB(STimedFBListEntry *paTimerListEntry, const CIEC_TIME &paTimeInterval) {
  //calculate the correct interval based on time-base and timer ticks per seconds
  paTimerListEntry->mInterval = static_cast<TForteUInt32>((paTimeInterval * getTicksPerSecond()) / cgForteTimeBaseUnitsPerSecond);
  {
    CCriticalRegion criticalRegion(mSync);
    addTimedFBEntry(paTimerListEntry);
  }
}

void CTimerHandler::addTimedFBEntry(STimedFBListEntry *paTimerListEntry) {
  paTimerListEntry->mTimeOut = mForteTime + paTimerListEntry->mInterval; // the next activation time of this FB
  paTimerListEntry->mNext = 0;

  // Correct null intervals that can lead to event queue overflow to 10 ms
  if(paTimerListEntry->mInterval == 0) {
    paTimerListEntry->mTimeOut += (getTicksPerSecond() > 100) ? getTicksPerSecond() / 100 : 1;
  }
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
  CCriticalRegion criticalRegion(mSync);
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
  if(0 != mTimedFBList){
    //only check the list if there are entries in the list
    CCriticalRegion criticalRegion(mSync);
    while (0 != mTimedFBList) {
      if (mTimedFBList->mTimeOut > mForteTime) {
        break;
      }
      mDeviceExecution.startNewEventChain(mTimedFBList->mTimedFB);
      STimedFBListEntry *buffer = mTimedFBList;
      mTimedFBList = mTimedFBList->mNext;

      switch (buffer->mType) {
        case e_Periodic:
          addTimedFBEntry(buffer); //re-register the timed FB
          break;
        case e_SingleShot:
          // nothing special is to do up to now
        default:
          buffer->mNext = 0;
          buffer->mTimeOut = 0;
          break;
      }
    }
  }
}

