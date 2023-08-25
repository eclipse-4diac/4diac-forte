/*******************************************************************************
 * Copyright (c) 2011, 2013 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerhard Ebenhofer, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "externtimeha.h"
#include "../../core/devexec.h"
#include <windows.h>

CExternTimerHandler *CExternTimerHandler::smFORTEExtTimer = 0;

const TForteInt32 CExternTimerHandler::csmTicksPerSecond = 1000;

extern "C" __declspec(dllexport) 
void __stdcall nextTick(void){
  CExternTimerHandler::externNextTick();
}

extern "C" __declspec(dllexport) 
unsigned int __stdcall getTicksPerSecond(){
  return CExternTimerHandler::getExternTicksPerSecond();
}

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& paDeviceExecution){
  if(!CExternTimerHandler::smFORTEExtTimer){ //creating two timers is not possible
    CExternTimerHandler::smFORTEExtTimer = new CExternTimerHandler(paDeviceExecution);
  }
  return CExternTimerHandler::smFORTEExtTimer;
}

CExternTimerHandler::CExternTimerHandler(CDeviceExecution& paDeviceExecution) : CTimerHandler(paDeviceExecution)  {
}

CExternTimerHandler::~CExternTimerHandler(){
  disableHandler();
  CExternTimerHandler::smFORTEExtTimer = 0;
}

void CExternTimerHandler::externNextTick(){
  if(CExternTimerHandler::smFORTEExtTimer){
    CExternTimerHandler::smFORTEExtTimer->nextTick();
  }
}

void CExternTimerHandler::enableHandler(){
  //TODO think on hwo to handle this.
}

void CExternTimerHandler::disableHandler(){
  //TODO think on hwo to handle this.
}

void CExternTimerHandler::setPriority(int paPriority){
  //TODO think on hwo to handle this.
}

int CExternTimerHandler::getPriority() const{
  //TODO think on hwo to handle this.
  return 1;
}
