/*******************************************************************************
 * Copyright (c) 2012, 2013 ACIN and fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "vxworkstimeha.h"
//#include "../../core/devexec.h"
#include <time.h>
#include <sys/time.h>

#ifndef timerclear
#define timerclear(tvp) ((tvp)->tv_sec = (tvp)->tv_usec = 0)
#endif

#ifndef timersub
#define timersub(a, b, result)                          \
  do {                                                  \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;       \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;    \
    if ((result)->tv_usec < 0) {                        \
      --(result)->tv_sec;                               \
      (result)->tv_usec += 1000000;                     \
    }                                                   \
  } while (0)
#endif

#ifndef timeradd
#define timeradd(a, b, result)                          \
  do {                                                  \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;       \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;    \
    if ((result)->tv_usec >= 1000000)                   \
      {                                                 \
  ++(result)->tv_sec;                                   \
  (result)->tv_usec -= 1000000;                         \
      }                                                 \
  } while (0)
#endif

#ifndef timercmp
#define timercmp(a, b, CMP)                             \
  (((a)->tv_sec == (b)->tv_sec) ?                       \
   ((a)->tv_usec CMP (b)->tv_usec) :                    \
   ((a)->tv_sec CMP (b)->tv_sec))
#endif

void CTimerHandler::createTimerHandler(void){
  if(0 == sm_poFORTETimer)
    sm_poFORTETimer = new CPCTimerHandler();
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
    stOldTime = stNewTime; // in c++ this should work fine
  }
}

void CPCTimerHandler::enableHandler(void){
  start();
}

void CPCTimerHandler::disableHandler(void){
  end();
}

void CPCTimerHandler::setPriority(int ){
  //TODO think on how to handle this.
}

int CPCTimerHandler::getPriority(void) const{
  //TODO think on how to handle this.
  return 1;
}
