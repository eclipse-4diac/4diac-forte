/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte_thread.h"

#include <fortenew.h>
#include "../devlog.h"

#define FORTE_TASK_NAME       "FORTE_TASK"
#define FORTE_TASK_TOKEN      TSK_TOK_1
#define FORTE_TASK_PRIORITY   TSK_PRIO_15

UINT CrcXThread::smTaskInstance = 0;
RX_TASK_TOKEN CrcXThread::smTaskToken = FORTE_TASK_TOKEN;

forte::arch::CThreadBase<RX_HANDLE>::TThreadHandleType CrcXThread::createThread(long paStackSize){
  TThreadHandleType retVal = 0;
  if (0 != mStack){
    RX_RESULT retVal;
    retVal = rX_SysCreateTask(FORTE_TASK_NAME, threadFunction, (void*)this, mStack, paStackSize / 4, RX_TASK_AUTO_START,
        0, FORTE_TASK_PRIORITY, CrcXThread::smTaskToken, CrcXThread::smTaskInstance, (void (*) (void*))NULL);
    if (RX_OK == retVal){
      CrcXThread::smTaskInstance++;
      CrcXThread::smTaskToken++;
      if(RX_OK != rX_SysIdentifyTask(FORTE_TASK_NAME, CrcXThread::smTaskInstance - 1, &retVal, 0, 0)){
        DEVLOG_ERROR("Task was created but couldn't be identified. Memory leaks might happen\n");
      }
    }else{
      DEVLOG_ERROR("Couldn't create a task\n");
    }
  }
  return retVal;
}

void CrcXThread::threadFunction(void *arguments){
  CThreadBase::runThread(static_cast<CrcXThread *>(arguments));
}

CrcXThread::CrcXThread(long paStackSize) : CThreadBase(paStackSize),  mStack(0){
  if(paStackSize < (300 * 4)){ // If m_nStackSize == 0, the minimum is also set.
    paStackSize = 300 * 4;
  }

  mStack = new char[paStackSize];
  if (0 == mStack){
    DEVLOG_ERROR("Not enough memory to allocate %l bytes for creating a new thread\n", mStackSize);
  }
}

CrcXThread::~CrcXThread(){
}

void CrcXThread::setDeadline(const CIEC_TIME &){
  //mDeadline = paVal;
}

void CrcXThread::sleepThread(unsigned int paMilliSeconds){
  rX_SysSleepTask(paMilliSeconds * 1000 / rX_SysGetSystemCycletime());
}

void CrcXThread::deleteThread(RX_HANDLE paThreadHandle){
  rX_SysDeleteTask(paThreadHandle, 0);
}
