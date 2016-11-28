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
#include "../core/devexec.h"
#include "../core/esfb.h"
#include <criticalregion.h>

CTimerHandler *CTimerHandler::sm_poFORTETimer = 0;

void CTimerHandler::registerTimedFB(STimedFBListEntry *pa_pstTimerListEntry, const CIEC_TIME &pa_roTimeInterval) {
  //calculate the correct interval based on time-base and timer ticks per seconds
  pa_pstTimerListEntry->m_nInterval = static_cast<TForteUInt32>((pa_roTimeInterval * getTicksPerSecond()) / FORTE_TIME_BASE_UNITS_PER_SECOND);
  {
    CCriticalRegion criticalRegion(m_oSync);
    addTimedFBEntry(pa_pstTimerListEntry);
  }
}

void CTimerHandler::addTimedFBEntry(STimedFBListEntry *pa_pstTimerListEntry) {
  pa_pstTimerListEntry->m_stTimeOut = m_stForteTime;
  pa_pstTimerListEntry->m_stTimeOut += pa_pstTimerListEntry->m_nInterval; // the next activationtime of this FB
  pa_pstTimerListEntry->m_pstNext = 0;

	// Correct null intervals that can lead to event queue overflow to 10 ms
	if (pa_pstTimerListEntry->m_nInterval == 0)
		pa_pstTimerListEntry->m_stTimeOut += getTicksPerSecond() > 100 ? getTicksPerSecond() / 100 : 1;

  if (0 == m_pstTimedFBList) {
    m_pstTimedFBList = pa_pstTimerListEntry;
  } else {
    if (m_pstTimedFBList->m_stTimeOut > pa_pstTimerListEntry->m_stTimeOut) {
      pa_pstTimerListEntry->m_pstNext = m_pstTimedFBList;
      m_pstTimedFBList = pa_pstTimerListEntry;
    } else {
      STimedFBListEntry *runner = m_pstTimedFBList;
      while (0 != runner->m_pstNext) {
        if (runner->m_pstNext->m_stTimeOut > pa_pstTimerListEntry->m_stTimeOut) {
          pa_pstTimerListEntry->m_pstNext = runner->m_pstNext;
          runner->m_pstNext = pa_pstTimerListEntry;
          break;
        }
        runner = runner->m_pstNext;
      }
      runner->m_pstNext = pa_pstTimerListEntry;
    }
  }
}

void CTimerHandler::unregisterTimedFB(CEventSourceFB *pa_poTimedFB) {
  CCriticalRegion criticalRegion(m_oSync);
  if (0 != m_pstTimedFBList) {
    STimedFBListEntry *buffer = 0;
    if (m_pstTimedFBList->m_poTimedFB == pa_poTimedFB) {
      buffer = m_pstTimedFBList;
      m_pstTimedFBList = m_pstTimedFBList->m_pstNext;
      buffer->m_pstNext = 0;
      buffer->m_stTimeOut.m_nLowerValue = 0;
      buffer->m_stTimeOut.m_nUpperValue = 0;
    } else {
      STimedFBListEntry *runner = m_pstTimedFBList;
      while (0 != runner->m_pstNext) {
        if (runner->m_pstNext->m_poTimedFB == pa_poTimedFB) {
          buffer = runner->m_pstNext;
          runner->m_pstNext = runner->m_pstNext->m_pstNext;
          buffer->m_pstNext = 0;
          buffer->m_stTimeOut.m_nLowerValue = 0;
          buffer->m_stTimeOut.m_nUpperValue = 0;
          break;
        }
        runner = runner->m_pstNext;
      }
    }
  }
}

void CTimerHandler::nextTick(void) {
  ++m_stForteTime;
  getDeviceExecution()->notifyTime(m_stForteTime); //notify the device execution that one tick passed by.
  {
    CCriticalRegion criticalRegion(m_oSync);
    while (0 != m_pstTimedFBList) {
      if (m_pstTimedFBList->m_stTimeOut > m_stForteTime) {
        break;
      }
      getDeviceExecution()->startNewEventChain(m_pstTimedFBList->m_poTimedFB);
      STimedFBListEntry *buffer = m_pstTimedFBList;
      m_pstTimedFBList = m_pstTimedFBList->m_pstNext;

      switch (buffer->m_eType) {
        case e_Periodic:
          addTimedFBEntry(buffer); //reregister the timed FB
          break;
        case e_SingleShot:
          // nothing special is to do up to now
        default:
          buffer->m_pstNext = 0;
          buffer->m_stTimeOut.m_nLowerValue = 0;
          buffer->m_stTimeOut.m_nUpperValue = 0;
          break;
      }
    }
  }
}

