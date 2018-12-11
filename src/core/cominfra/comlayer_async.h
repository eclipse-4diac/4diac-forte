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


#ifndef FORTE_COMLAYER_ASYNC_H
#define FORTE_COMLAYER_ASYNC_H

#include "comlayer.h"
#include "../extevhan.h"
#include "../fortelist.h"
#include <forte_thread.h>
#include <forte_sync.h>
#include <forte_sem.h>

namespace forte {
  namespace com_infra {

    class CComLayerAsync : public CComLayer, protected CThread {
      public:
        virtual ~CComLayerAsync();

      protected:
        CComLayerAsync(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB);
        virtual void run();

        unsigned int callAsync(void *payload);

        virtual void* handleAsyncCall(const unsigned int callId, void *payload) = 0;

        virtual void handleAsyncCallResult(const unsigned int callId, void *payload, void *result) = 0;

        virtual void handleAsyncEvent() = 0;

        virtual EComResponse processInterruptChild();

      private:
        virtual EComResponse processInterrupt();

        class SAsyncData {
          public:
            const unsigned int mCallId;
            void *mPayload;
            void *mResult;

            SAsyncData(unsigned int paCallId, void *paPayload, void *paResult) :
                mCallId(paCallId), mPayload(paPayload), mResult(paResult) {
            }
        };

        typedef CSinglyLinkedList<SAsyncData> TAsynchDataList;
        TAsynchDataList mAsyncCalls;
        TAsynchDataList mAsyncResults;
        unsigned int mCurrentCallId;

        CSyncObject mAsyncResultsMutex;
        CSemaphore mSuspendSemaphore;
    };

  }
}

#endif //FORTE_COMLAYER_ASYNC_H
