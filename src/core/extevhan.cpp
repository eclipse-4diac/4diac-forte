/*******************************************************************************
  * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "extevhan.h"
#include "../resource.h"
#include "../device.h"
#include <devlog.h>

CExternalEventHandler::CExternalEventHandler(CDeviceExecution& paDeviceExecution) : mDeviceExecution(paDeviceExecution)
{
}

bool CExternalEventHandler::isAllowed(){
  return mDeviceExecution.extEvHandlerIsAllowed(getIdentifier());
}

void CExternalEventHandler::startNewEventChain(CEventSourceFB *paECStartFB){
  if(isAllowed()){
    FORTE_TRACE("Starting EC\n");
    mDeviceExecution.startNewEventChain(paECStartFB);
  }
  else{
    //TODO: handle this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DEVLOG_DEBUG("Starting EC NOT ALLOWED !!!!!!!!!!!!!!!1\n");
  }
}

