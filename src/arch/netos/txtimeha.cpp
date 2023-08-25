/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <fortenew.h>
#include "txtimeha.h"
#include "../../core/devexec.h"

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& paDeviceExecution){
  return new CTXTimerHandler(paDeviceExecution);
}

CTXTimerHandler::CTXTimerHandler(CDeviceExecution& paDeviceExecution) : CTimerHandler(paDeviceExecution)  {
// setup the handler for recieving the timer calls  
  //TODO handle ticks per second correctly here
  UINT status = tx_timer_create(&mTimer, "FORTE timer", &timerHandlerFunc, (ULONG) this, 1, 1, TX_NO_ACTIVATE);
  if (status == TX_SUCCESS)
    DEVLOG_DEBUG("Timer created\n");
  else
    DEVLOG_DEBUG("Error creating timer\n");
  
  //TODO handle retval
}

CTXTimerHandler::~CTXTimerHandler(){
  disableHandler();
  tx_timer_delete(&mTimer);
}

void CTXTimerHandler::enableHandler(){
  UINT status = tx_timer_activate(&mTimer);
  if (status == TX_SUCCESS)
    DEVLOG_DEBUG("Timer activated\n");
  else
    DEVLOG_DEBUG("Error activating timer\n");
  
  //TODO handle retval
}

void CTXTimerHandler::disableHandler(){
  tx_timer_deactivate(&mTimer);
  //TODO handle retval
}

void CTXTimerHandler::setPriority(int paPriority){
}

int CTXTimerHandler::getPriority() const {
  return 0;
}
