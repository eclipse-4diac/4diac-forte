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

CExternTimerHandler *CExternTimerHandler::sm_poFORTEExtTimer = 0;

const TForteInt32 CExternTimerHandler::csm_nTicksPerSecond = 1000;

extern "C" __declspec(dllexport) 
void __stdcall nextTick(void){
  CExternTimerHandler::externNextTick();
}

extern "C" __declspec(dllexport) 
unsigned int __stdcall getTicksPerSecond(){
  return CExternTimerHandler::getExternTicksPerSecond();
}

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& pa_poDeviceExecution){
  if(!CExternTimerHandler::sm_poFORTEExtTimer){ //creating two timers is not possible
    CExternTimerHandler::sm_poFORTEExtTimer = new CExternTimerHandler(pa_poDeviceExecution);
  }
  return CExternTimerHandler::sm_poFORTEExtTimer;
}

CExternTimerHandler::CExternTimerHandler(CDeviceExecution& pa_poDeviceExecution) : CTimerHandler(pa_poDeviceExecution)  {
}

CExternTimerHandler::~CExternTimerHandler(){
  disableHandler();
  CExternTimerHandler::sm_poFORTEExtTimer = 0;
}

void CExternTimerHandler::externNextTick(){
  if(CExternTimerHandler::sm_poFORTEExtTimer){
    CExternTimerHandler::sm_poFORTEExtTimer->nextTick();
  }
}

void CExternTimerHandler::enableHandler(void){
  //TODO think on hwo to handle this.
}

void CExternTimerHandler::disableHandler(void){
  //TODO think on hwo to handle this.
}

void CExternTimerHandler::setPriority(int pa_nPriority){
  //TODO think on hwo to handle this.
}

int CExternTimerHandler::getPriority(void) const{
  //TODO think on hwo to handle this.
  return 1;
}
