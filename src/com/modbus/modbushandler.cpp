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
#include "modbushandler.h"
#include "devlog.h"
#include "../core/devexec.h"
#include <criticalregion.h>
#include <commfb.h>

DEFINE_HANDLER(CModbusHandler);

CModbusHandler::TCallbackDescriptor CModbusHandler::m_nCallbackDescCount = 0;

CModbusHandler::CModbusHandler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution)  {
}

CModbusHandler::~CModbusHandler(){

}

CModbusHandler::TCallbackDescriptor CModbusHandler::addComCallback(forte::com_infra::CComLayer* pa_pComCallback){
  CCriticalRegion criticalRegion(m_oSync);
  m_nCallbackDescCount++;
  TComContainer stNewNode = { m_nCallbackDescCount, pa_pComCallback };
  m_lstComCallbacks.pushBack(stNewNode);

  return m_nCallbackDescCount;
}

void CModbusHandler::removeComCallback(CModbusHandler::TCallbackDescriptor pa_nCallbackDesc){
  CCriticalRegion criticalRegion(m_oSync);

  TCallbackList::Iterator itRunner(m_lstComCallbacks.begin());

  if(itRunner->m_nCallbackDesc == pa_nCallbackDesc){
    m_lstComCallbacks.popFront();
  }
  else{
    TCallbackList::Iterator itLastPos(itRunner);
    TCallbackList::Iterator itEnd(m_lstComCallbacks.end());
    ++itRunner;
    while(itRunner != itEnd){
      if(itRunner->m_nCallbackDesc == pa_nCallbackDesc){
        m_lstComCallbacks.eraseAfter(itLastPos);
        break;
      }
      itLastPos = itRunner;
      ++itRunner;
    }
  }
}

void CModbusHandler::executeComCallback(CModbusHandler::TCallbackDescriptor pa_nCallbackDesc){
  m_oSync.lock();
  TCallbackList::Iterator itEnd(m_lstComCallbacks.end());
  TCallbackList::Iterator itCurrent(m_lstComCallbacks.begin());
  for(TCallbackList::Iterator itCallback = m_lstComCallbacks.begin(); itCallback != itEnd; ++itCallback){
    itCurrent = itCallback;

    if(itCurrent->m_nCallbackDesc == pa_nCallbackDesc){
      forte::com_infra::CComLayer *comLayer = itCurrent->m_pCallback;
      m_oSync.unlock();
      if(forte::com_infra::e_Nothing != comLayer->recvData(0,0)){
        startNewEventChain(comLayer->getCommFB());
      }
      m_oSync.lock();
      break;
    }
  }
  m_oSync.unlock();
}
