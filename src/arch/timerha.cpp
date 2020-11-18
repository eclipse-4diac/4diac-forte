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

DEFINE_HANDLER(CTimerHandler)

CTimerHandler::CTimerHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    mForteTime(0), mTimedFBList(0){
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
    CCriticalRegion criticalRegion(mSync);
    addTimedFBEntry(paTimerListEntry);
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
          buffer->mTimeOut = mForteTime + buffer->mInterval;  // the next activation time of this FB
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

