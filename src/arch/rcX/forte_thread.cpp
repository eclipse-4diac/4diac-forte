/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte_thread.h"

#include <fortenew.h>
#include "../devlog.h"
#include <criticalregion.h>

#define FORTE_TASK_NAME       "FORTE_TASK"
#define FORTE_TASK_TOKEN      TSK_TOK_25
#define FORTE_TASK_PRIORITY   TSK_PRIO_25

UINT CrcXThread::smTaskInstance = 0;

void CrcXThread::start(void){
  if (0 != m_pacStack){
    if (RX_OK == rX_SysCreateTask(FORTE_TASK_NAME, threadFunction, (void*)this, m_pacStack, mStackSize / 4, RX_TASK_AUTO_START,
        0, FORTE_TASK_PRIORITY, FORTE_TASK_TOKEN, CrcXThread::smTaskInstance, (void (*) (void*))NULL)){
      CrcXThread::smTaskInstance++;
      if(RX_OK != rX_SysIdentifyTask(FORTE_TASK_NAME, CrcXThread::smTaskInstance - 1, &mThreadID, 0, 0)){
        DEVLOG_ERROR("Task was created but couldn't be identified. Memory leaks might happen\n");
      }
    }else{
      DEVLOG_ERROR("Couldn't create a task\n");
    }
  }
}

void CrcXThread::threadFunction(void *arguments){
  // Get pointer to CThread object out of void pointer
  CrcXThread *pThread = static_cast<CrcXThread *>(arguments);

  // if pointer is ok
  if(0 != pThread){
    CCriticalRegion criticalRegion(pThread->mJoinMutex);
    pThread->setAlive(true);
    pThread->run();
    pThread->setAlive(false);
  }
  else{
    DEVLOG_ERROR("pThread pointer is 0!");
  }
}

CrcXThread::CrcXThread(long pa_nStackSize) :
      mThreadID(0), mStackSize(pa_nStackSize), m_pacStack(0){
	if(mStackSize < (128 * 4)){ // If m_nStackSize == 0, the minimum is also set.
		mStackSize = 128 * 4;
	}

	m_pacStack = forte_malloc(mStackSize);
	if (0 == m_pacStack){
	  DEVLOG_ERROR("Not enough memory to allocate %l bytes for creating a new thread\n", mStackSize);
	}
}

CrcXThread::~CrcXThread(){
  if(0 != mThreadID){
    end();
  }
  rX_SysDeleteTask(mThreadID, 0);
  forte_free(m_pacStack);
}

void CrcXThread::setDeadline(const CIEC_TIME &pa_roVal){
  mDeadline = pa_roVal;
}

void CrcXThread::join(void){
  if(0 != mThreadID){
    CCriticalRegion criticalRegion(mJoinMutex);
  }
}
