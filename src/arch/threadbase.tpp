/*******************************************************************************
 * Copyright (c) 2016, 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "threadbase.h"
#include <criticalregion.h>
#include "../devlog.h"

using namespace forte::arch;

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::CThreadBase(long paStackSize) :
    mThreadHandle(nullHandle), mStackSize(paStackSize), mStack(0), mAlive(false) {

}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::~CThreadBase() {
  end();
  if(0 != mStack ){
    delete[] mStack;
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::start(void){
  if(nullHandle == mThreadHandle){
    mJoinMutex.lock();
    mThreadHandle = createThread(mStackSize);
    if(nullHandle == mThreadHandle){
      DEVLOG_ERROR("Error could not create the thread!");
    }
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::end(void) {
  if(nullHandle != mThreadHandle){
    setAlive(false);
    join();
    ThreadDeletePolicy::deleteThread(mThreadHandle);
    mThreadHandle = nullHandle;
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::join(){
  if(nullHandle != mThreadHandle){
    CCriticalRegion criticalRegion(mJoinMutex);
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::runThread(CThreadBase *paThread) {
	// if pointer is ok
	if (0 != paThread) {
		paThread->setAlive(true);
		paThread->run();
		paThread->setAlive(false);
		paThread->mJoinMutex.unlock();
	} else {
		DEVLOG_ERROR("pThread pointer is 0!");
	}
}
