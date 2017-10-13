/*******************************************************************************
  * Copyright (c) 2017 fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Jose Cabral - initial implementation and rework handler infrastructure
  *******************************************************************************/

#include <devexec.h>
#include <timerha.h>


const unsigned int CTimerHandler::handlerIdentifier = 0;

void CDeviceExecution::createHandlers(CDeviceExecution& pa_DeviceExecution){

  pa_DeviceExecution.mRegisteredEventHandlers[0].m_poHandler = CTimerHandler::createTimerHandler(pa_DeviceExecution);
}
