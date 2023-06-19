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
#include "devlog.h"
#include "../core/devexec.h"
#include <criticalregion.h>
#include <commfb.h>

DEFINE_HANDLER(CModbusHandler);

CModbusHandler::TCallbackDescriptor CModbusHandler::m_nCallbackDescCount = 0;

CModbusHandler::CModbusHandler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution)  {
}

CModbusHandler::~CModbusHandler() = default;

CModbusHandler::TCallbackDescriptor CModbusHandler::addComCallback(forte::com_infra::CComLayer* pa_pComCallback){
  CCriticalRegion criticalRegion(m_oSync);
  m_nCallbackDescCount++;
  TComContainer stNewNode = { m_nCallbackDescCount, pa_pComCallback };
  m_lstComCallbacks.push_back(stNewNode);

  return m_nCallbackDescCount;
}

CModbusHandler::TCallbackList::iterator CModbusHandler::findComCallbackUnlocked(TCallbackDescriptor pa_nCallbackDesc) {
  return std::find_if(
           m_lstComCallbacks.begin(),
           m_lstComCallbacks.end(),
           [pa_nCallbackDesc](const TComContainer &cc) {
             return cc.m_nCallbackDesc == pa_nCallbackDesc;
           }
         );
}

void CModbusHandler::removeComCallback(CModbusHandler::TCallbackDescriptor pa_nCallbackDesc){
  CCriticalRegion criticalRegion(m_oSync);

  auto itDelCb = findComCallbackUnlocked(pa_nCallbackDesc);
  if (itDelCb != m_lstComCallbacks.end()) {
    m_lstComCallbacks.erase(itDelCb);
  }
}

void CModbusHandler::executeComCallback(CModbusHandler::TCallbackDescriptor pa_nCallbackDesc){
  m_oSync.lock();
  auto itCallback = findComCallbackUnlocked(pa_nCallbackDesc);
  if (itCallback != m_lstComCallbacks.end()) {
    forte::com_infra::CComLayer *comLayer = itCallback->m_pCallback;
    m_oSync.unlock();
    if(forte::com_infra::e_Nothing != comLayer->recvData(nullptr,0)){
      startNewEventChain(comLayer->getCommFB());
    }
  } else {
    m_oSync.unlock();
  }
}
