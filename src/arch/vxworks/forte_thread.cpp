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
#include <time.h>

forte::arch::CThreadBase<TASK_ID, TASK_ID_ERROR>::TThreadHandleType CVxWorksThread::createThread(long paStackSize){
  //TODO: Check if guarding the stack is necessary
  return taskSpawn(0, scmInitialTaskPriority, VX_FP_TASK /* Needed for C++*/, paStackSize,
      (FUNCPTR)threadFunction, (_Vx_usr_arg_t) this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void CVxWorksThread::threadFunction(void *paArguments){
  forte::arch::CThreadBase<TASK_ID, TASK_ID_ERROR>::runThread(static_cast<CVxWorksThread *>(paArguments));
}

CVxWorksThread::CVxWorksThread(long paStackSize) : forte::arch::CThreadBase<TASK_ID, TASK_ID_ERROR>(paStackSize){
}

CVxWorksThread::~CVxWorksThread(){
}

void CVxWorksThread::setDeadline(const CIEC_TIME &paVal){
  mDeadline = paVal;
}

void CVxWorksThread::sleepThread(unsigned int paMilliSeconds){
  struct timespec stReq;
  stReq.tv_sec = paMilliSeconds / 1000;
  stReq.tv_nsec = 1000000 * (paMilliSeconds % 1000);
  nanosleep(&stReq, NULL);
}
