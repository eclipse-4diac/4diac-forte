/*******************************************************************************
 * Copyright (c) 2010, 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "pctimeha.h"
#include "../../core/devexec.h"
#include <windows.h>

const TForteInt32 CPCTimerHandler::csmTicksPerSecond = 1000;

CPCTimerHandler::CPCTimerHandler(CDeviceExecution& paDeviceExecution) : CTimerHandler(paDeviceExecution)  {
}

CPCTimerHandler::~CPCTimerHandler(){
  disableHandler();
}

void CPCTimerHandler::run(){
  DWORD stReq = 0; // in us
  LARGE_INTEGER stReqTimeVal = { 0, 0 };

  LARGE_INTEGER stFrequenzy = { 0, 0 }; // clocks per second
  LARGE_INTEGER stElapsed1 = { 0, 0 };
  LARGE_INTEGER stElapsed2 = { 0, 0 };
  LARGE_INTEGER stError = { 0, 0 };
  LARGE_INTEGER stISum = { 0, 0 };
  LARGE_INTEGER stWaittime = { 0, 0 };

  QueryPerformanceFrequency(&stFrequenzy);
  QueryPerformanceCounter(&stElapsed1);

  stReqTimeVal.QuadPart = stFrequenzy.QuadPart / CPCTimerHandler::csmTicksPerSecond;
  //stReqTimeVal = 1000000 / CPCTimerHandler::csmTicksPerSecond;

  while(isAlive()){
    stWaittime.QuadPart *= 1000; // calculate in ms, not sec
    stReq = static_cast<DWORD>(stWaittime.QuadPart / stFrequenzy.QuadPart); // calculate elapsed time in ms

    // dont give up the thread if we still have to do some ticks(some os have 10ms minimum threadtime)
    if(stReq > 0){
      Sleep(stReq);
    }
    nextTick();

    // sample new time
    stElapsed2.QuadPart = stElapsed1.QuadPart;
    QueryPerformanceCounter(&stElapsed1);

    // calulate controler error: Diff - Setupvalue = Error
    stError.QuadPart = stElapsed1.QuadPart - stElapsed2.QuadPart;
    stError.QuadPart = stReqTimeVal.QuadPart - stError.QuadPart;

    // I-controller
    stISum.QuadPart += stError.QuadPart;

    // calculate new Output
    stWaittime.QuadPart = stReqTimeVal.QuadPart + stISum.QuadPart;
    if(stWaittime.QuadPart < 0){
      stWaittime.QuadPart = 0;
    }
  }
}

void CPCTimerHandler::enableHandler(){
  start();
}

void CPCTimerHandler::disableHandler(){
  end();
}

void CPCTimerHandler::setPriority(int) {
  //TODO think on how to handle this.
}

int CPCTimerHandler::getPriority() const{
  //TODO think on how to handle this.
  return 1;
}
