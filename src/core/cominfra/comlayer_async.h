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

#include <extevhan.h>
#include "comlayer.h"
#include "forte_thread.h"
#include "forte_sync.h"
#include "fortelist.h"

namespace forte {
	namespace com_infra {

		struct CComLayerAsync_Data {
			const unsigned int callId;
			void *payload;
			void *result;
		};

		class CComLayerAsync: public CComLayer, protected CThread{
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
			unsigned int currentCallId;

			CSinglyLinkedList<struct CComLayerAsync_Data*> asyncCalls;
			CSinglyLinkedList<struct CComLayerAsync_Data*> asyncResults;

			virtual EComResponse processInterrupt();

			CSyncObject asyncResultsMutex;
		};

	}
}

#endif //FORTE_COMLAYER_ASYNC_H
