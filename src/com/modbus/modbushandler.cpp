/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <algorithm>
#include "modbushandler.h"
#include "modbuslayer.h"
#include "devlog.h"
#include "../core/devexec.h"
#include <commfb.h>

DEFINE_HANDLER(CModbusHandler);


CModbusHandler::CModbusHandler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution)  {
}

CModbusHandler::~CModbusHandler() = default;

void CModbusHandler::executeComCallback(forte::com_infra::CModbusComLayer* pa_pComCallback){
  if(forte::com_infra::e_Nothing != pa_pComCallback->recvData(0,0)){
    startNewEventChain(pa_pComCallback->getCommFB());
  }
}
