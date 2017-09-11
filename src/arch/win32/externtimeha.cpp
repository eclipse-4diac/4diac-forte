/*******************************************************************************
 * Copyright (c) 2011, 2013 Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

CTimerHandler* CTimerHandler::createTimerHandler(){
  if(!CExternTimerHandler::sm_poFORTEExtTimer){ //creating two timers is not possible
    CExternTimerHandler::sm_poFORTEExtTimer = new CExternTimerHandler();
  }
  return CExternTimerHandler::sm_poFORTEExtTimer;
}

CExternTimerHandler::CExternTimerHandler(){
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
