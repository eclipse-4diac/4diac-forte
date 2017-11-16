/*******************************************************************************
 * Copyright (c) 2006 - 2016 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include <fortenew.h>
#include <criticalregion.h>
#include "forte_thread.h"
#include <unistd.h>

TCECOSThreadPtr CECOSThread::smThreadList[CECOSThread::scmThreadListSize] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
CSyncObject CECOSThread::smThreadListLock;

forte::arch::CThreadBase<cyg_handle_t>::TThreadHandleType CECOSThread::createThread(long paStackSize){
  TThreadHandleType handle = 0;
  cyg_thread_create(scmThreadListSize + 2, //lowest priority in ecos
                    threadFunction, (cyg_addrword_t)(this), 
                    "Test Name", mStack, paStackSize, &handle, &mThread);

  cyg_thread_resume(handle);
  return handle;
}

void CECOSThread::threadFunction(cyg_addrword_t data){
  CThreadBase::runThread(static_cast<CECOSThread *>(data));
}

CECOSThread::CECOSThread(long paStackSize) : CThreadBase(paStackSize){
  mStack = new char[paStackSize];
}

CECOSThread::~CECOSThread(){
}

void CECOSThread::setDeadline(const CIEC_TIME &paVal){
  int i, ii;
  CCriticalRegion criticalRegion(sm_oThreadListLock);
  DEVLOG_DEBUG(">>>>Thread: Set Deadline: %lu\n", paVal.operator TValueType ());
  mDeadLine = paVal;
  //first of all check if this thread is already in the list and remove it from the list
  for(i = 0; i < scmThreadListSize; i++){
    if(0 == smThreadList[i])
      break;
    else
      if(this == smThreadList[i]){
        for(ii = i; ii < scmThreadListSize - 1; ii++){
          if(0 == smThreadList[ii + 1])
            break;
          smThreadList[ii + 1]->setPriority(ii + 2);
          smThreadList[ii] = sm_aoThreadList[ii + 1];
        }
        break;
      }  
  }

  if(0 == mDeadLine)
    setPriority(scmThreadListSize + 2); // use the lowest user priority
  else{
    for(i = 0; i < scmThreadListSize; i++){
      if(0 == smThreadList[i]){
        smThreadList[i] = this;
        setPriority( i + 2);
        break;
      }  
      else
        if(mDeadLine < smThreadList[i]->getDeadline()){
          CECOSThread *poRBuf, *poSBuf= smThreadList[i];
          smThreadList[i] =  this;
          setPriority( i + 2);
          for(ii = i + 1; ii < scmThreadListSize; ii++){
            poSBuf->setPriority(ii + 2);
            poRBuf = smThreadList[ii];
            smThreadList[ii] = poSBuf;
            if(0 == poRBuf)
              break;
            poSBuf = poRBuf;  
          }  
          break;
        }
    }      
  }  
}

void CECOSThread::sleepThread(unsigned int paMilliSeconds){
  usleep(1000 * paMilliSeconds);
}
  
void CECOSThread::deleteThread(cyg_handle_t paThreadHandle){
  cyg_thread_delete(paThreadHandle);
}
