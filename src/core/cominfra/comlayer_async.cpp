/*******************************************************************************
  * Copyright (c) 2017 fortiss GmbH.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Stefan Profanter - initial implementation
  *******************************************************************************/

#include "comlayer_async.h"
#include "devlog.h"

forte::com_infra::CComLayerAsync::CComLayerAsync(forte::com_infra::CComLayer *pa_poUpperLayer, forte::com_infra::CBaseCommFB *pa_poComFB) :
    CComLayer(pa_poUpperLayer, pa_poComFB), currentCallId(0), asyncCalls(), asyncResults(), asyncResultsMutex() {

}

forte::com_infra::CComLayerAsync::~CComLayerAsync() {
  for (CSinglyLinkedList<CComLayerAsync_Data*>::Iterator iter = asyncCalls.begin(); iter != asyncCalls.end(); ++iter) {
    forte_free((*iter));
  }
  asyncCalls.clearAll();
  for (CSinglyLinkedList<CComLayerAsync_Data*>::Iterator iter = asyncResults.begin(); iter != asyncResults.end(); ++iter) {
    forte_free((*iter));
  }
  asyncResults.clearAll();
  setAlive(false);
  mSuspendSemaphore.semInc();
}

void forte::com_infra::CComLayerAsync::run() {
  while(isAlive()){

    mSuspendSemaphore.semWaitIndefinitly();

    while (isAlive() && !asyncCalls.isEmpty()) {
      CSinglyLinkedList<struct CComLayerAsync_Data*>::Iterator iter = asyncCalls.begin();
      (*iter)->result = handleAsyncCall((*iter)->callId, (*iter)->payload);
      asyncResultsMutex.lock();
      asyncResults.push_back((*iter));
      asyncResultsMutex.unlock();
      handleAsyncEvent();
      asyncCalls.pop_front();
    }
  }
}

unsigned int forte::com_infra::CComLayerAsync::callAsync(void *payload) {
  if (!isAlive()) {
    DEVLOG_ERROR("start() has to be called before callAsync\n");
    return 0;
  }

  currentCallId++;
  if (currentCallId == 0)
    // handle overflow. Do not return 0
    currentCallId = 1;

  CComLayerAsync_Data *data = static_cast<CComLayerAsync_Data*>(forte_malloc(sizeof(CComLayerAsync_Data)));

  // cast away const
  *(unsigned int *)&data->callId = currentCallId;
  data->payload = payload;

  asyncCalls.push_back(data);
  mSuspendSemaphore.semInc();

  return currentCallId;
}

forte::com_infra::EComResponse forte::com_infra::CComLayerAsync::processInterrupt() {
  if (asyncResults.isEmpty())
    return processInterruptChild();

  asyncResultsMutex.lock();
  while (isAlive() && !asyncResults.isEmpty()) {
    CSinglyLinkedList<CComLayerAsync_Data*>::Iterator iter = asyncResults.begin();
    handleAsyncCallResult((*iter)->callId, (*iter)->payload, (*iter)->result);
    forte_free((*iter));
    asyncResults.pop_front();
  }
  asyncResultsMutex.unlock();

  return processInterruptChild();
}

forte::com_infra::EComResponse forte::com_infra::CComLayerAsync::processInterruptChild() {
  return e_ProcessDataOk;
}
