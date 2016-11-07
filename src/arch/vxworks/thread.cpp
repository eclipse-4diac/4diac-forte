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

#include "thread.h"
#include "../devlog.h"
#include <criticalregion.h>

#define FORTE_TASK_PRIORITY   25

void CVxWorksThread::start(void){
  //TODO: Check if guarding the stack is necessary
  mThreadID = taskSpawn(0, FORTE_TASK_PRIORITY, VX_NO_STACK_FILL | VX_FP_TASK /* Needed for C++*/, mStackSize,
      (FUNCPTR)threadFunction, (_Vx_usr_arg_t) this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  if(TASK_ID_ERROR == mThreadID){
    //TODO: manage error
  }
}

void CVxWorksThread::threadFunction(void *arguments){
  // Get pointer to CThread object out of void pointer
  CVxWorksThread *pThread = static_cast<CVxWorksThread *>(arguments);

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

CVxWorksThread::CVxWorksThread(long pa_nStackSize) :
      mThreadID(TASK_ID_ERROR), mStackSize(pa_nStackSize){
}

CVxWorksThread::~CVxWorksThread(){
  if(0 != mThreadID){
    end();
  }
}

void CVxWorksThread::setDeadline(const CIEC_TIME &pa_roVal){
  mDeadline = pa_roVal;
}

void CVxWorksThread::join(void){
  if(0 != mThreadID){
    CCriticalRegion criticalRegion(mJoinMutex);
    taskDelete(mThreadID);
  }
}
