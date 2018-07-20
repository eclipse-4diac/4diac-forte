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
#include <devlog.h>
#include <criticalregion.h>

forte::com_infra::CComLayerAsync::CComLayerAsync(forte::com_infra::CComLayer *pa_poUpperLayer, forte::com_infra::CBaseCommFB *pa_poComFB) :
    CComLayer(pa_poUpperLayer, pa_poComFB), mCurrentCallId(0){

}

forte::com_infra::CComLayerAsync::~CComLayerAsync() {
  setAlive(false);
  mSuspendSemaphore.inc();
}

void forte::com_infra::CComLayerAsync::run() {
  while(isAlive()){

    mSuspendSemaphore.waitIndefinitely();

    while (isAlive() && !mAsyncCalls.isEmpty()) {
      SAsyncData first(*mAsyncCalls.begin());
      first.mResult = handleAsyncCall(first.mCallId, first.mPayload);
      {
        CCriticalRegion criticalRegion(mAsyncResultsMutex);
        mAsyncResults.push_back(first);
      }
      handleAsyncEvent();
      mAsyncCalls.pop_front();
    }
  }
}

unsigned int forte::com_infra::CComLayerAsync::callAsync(void *payload) {
  if (!isAlive()) {
    DEVLOG_ERROR("start() has to be called before callAsync\n");
    return 0;
  }

  mCurrentCallId++;
  if (mCurrentCallId == 0){
    // handle overflow. Do not return 0
    mCurrentCallId = 1;
  }
  mAsyncCalls.push_back(SAsyncData(mCurrentCallId, payload, 0));
  mSuspendSemaphore.inc();
  return mCurrentCallId;
}

forte::com_infra::EComResponse forte::com_infra::CComLayerAsync::processInterrupt() {
  if (mAsyncResults.isEmpty())
    return processInterruptChild();

  CCriticalRegion criticalRegion(mAsyncResultsMutex);
  while (isAlive() && !mAsyncResults.isEmpty()) {
    const SAsyncData &value = *(mAsyncResults.begin());
    handleAsyncCallResult(value.mCallId, value.mPayload, value.mResult);
    mAsyncResults.pop_front();
  }
  return processInterruptChild();
}

forte::com_infra::EComResponse forte::com_infra::CComLayerAsync::processInterruptChild() {
  return e_ProcessDataOk;
}
