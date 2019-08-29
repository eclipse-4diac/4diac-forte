/*******************************************************************************
 * Copyright (c) 2016, 2017, 2018 fortiss GmbH, TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - updates due to changes in CPThreadSemaphore
 *******************************************************************************/
#include "threadbase.h"
#include <criticalregion.h>
#include "../devlog.h"

using namespace forte::arch;

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::CThreadBase(long paStackSize) :
    mStack(0), mThreadHandle(nullHandle), mStackSize(paStackSize), mAlive(false) {

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
  CCriticalRegion criticalRegion(mThreadMutex);
  if(nullHandle == mThreadHandle){
    mThreadHandle = createThread(mStackSize);
    if(nullHandle == mThreadHandle){
      DEVLOG_ERROR("Error could not create the thread!\n");
      mJoinSem.inc();
    }
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::end(void) {
  CCriticalRegion criticalRegion(mThreadMutex);
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
    mJoinSem.waitIndefinitely();
    mJoinSem.inc(); //allow many joins
  }
}

template <typename TThreadHandle, TThreadHandle nullHandle, typename ThreadDeletePolicy>
void CThreadBase<TThreadHandle, nullHandle, ThreadDeletePolicy>::runThread(CThreadBase *paThread) {
  // if pointer is ok
  if (0 != paThread) {
    paThread->setAlive(true);
    paThread->run();
    paThread->setAlive(false);
    paThread->mJoinSem.inc();
  } else {
    DEVLOG_ERROR("pThread pointer is 0!");
  }
}
