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
#include <iostream>
#include "vxworkstimeha.h"
#include "../../core/devexec.h"
#include <time.h>


void CTimerHandler::createTimerHandler(void){
  if(0 == sm_poFORTETimer)
    sm_poFORTETimer = new CPCTimerHandler();
}

CPCTimerHandler::CPCTimerHandler(){
}

CPCTimerHandler::~CPCTimerHandler(){
  std::cout << "~CPCTimerHandler()" << std::endl;
  disableHandler();
}

void CPCTimerHandler::run(){
  struct timespec stReq;
  stReq.tv_sec = 0;
  stReq.tv_nsec = (1000000 / getTicksPerSecond()) * 1000;

  while(isAlive()){
     nanosleep(&stReq, NULL);
     nextTick();
   }
}

void CPCTimerHandler::enableHandler(void){
  start();
}

void CPCTimerHandler::disableHandler(void){
  end();
}

void CPCTimerHandler::setPriority(int pa_nPriority){
  //TODO think on hwo to handle this.
}

int CPCTimerHandler::getPriority(void) const{
  //TODO think on hwo to handle this.
  return 1;
}
