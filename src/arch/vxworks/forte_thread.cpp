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
  struct timespec stReq = {paMilliSeconds / 1000, 1000000 * (paMilliSeconds % 1000)};
  nanosleep(&stReq, NULL);
}
