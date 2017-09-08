/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortenew.h>
#include "pctimeha.h"
#include "../../core/devexec.h"
#include <time.h>
#include <sys/time.h>

CTimerHandler* CTimerHandler::createTimerHandler(){
  return new CPCTimerHandler();
}

CPCTimerHandler::CPCTimerHandler(){
}

CPCTimerHandler::~CPCTimerHandler(){
  disableHandler();
}

void CPCTimerHandler::run(){
  struct timespec stReq;
  stReq.tv_sec = 0;
  stReq.tv_nsec = (1000000 / getTicksPerSecond()) * 1000;
  
  struct timeval stOldTime;
  struct timeval stNewTime;
  struct timeval stReqTime;
  // Timer interval is 1ms
  stReqTime.tv_sec = 0;
  stReqTime.tv_usec = (1000 / getTicksPerSecond()) * 1000;
  struct timeval stDiffTime;
  struct timeval stRemainingTime;
  timerclear(&stRemainingTime);

  gettimeofday(&stOldTime, 0);
  while(isAlive()){

    nanosleep(&stReq, NULL);

    gettimeofday(&stNewTime, 0);

    timersub(&stNewTime, &stOldTime, &stDiffTime);

    timeradd(&stRemainingTime, &stDiffTime, &stRemainingTime);

    while(!timercmp(&stRemainingTime, &stReqTime, <)){
      nextTick();
      timersub(&stRemainingTime, &stReqTime, &stRemainingTime);
    }
    stOldTime = stNewTime;  // in c++ this should work fine
  } 
}

void CPCTimerHandler::enableHandler(void){
  start();
}

void CPCTimerHandler::disableHandler(void){
  end(); 
}

void CPCTimerHandler::setPriority(int ){
  //TODO think on hwo to handle this.
}

int CPCTimerHandler::getPriority(void) const {
  //TODO think on hwo to handle this.
  return 1;
}
