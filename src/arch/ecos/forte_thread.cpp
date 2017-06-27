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

TCECOSThreadPtr CECOSThread::sm_aoThreadList[CECOSThread::scm_nThreadListSize] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
CSyncObject CECOSThread::sm_oThreadListLock;

bool CECOSThread::create(void){
 // Get new Thread 
  cyg_thread_create(scm_nThreadListSize + 2, //lowest priority in ecos 
                    threadFunction, (cyg_addrword_t)(this), 
                    "Test Name", m_cStack, 
                    m_nStackSize, 
                    &m_stHandle, 
                    &m_stThread);

//setup the signals for the join implementation
  return true;
}

void CECOSThread::threadFunction(cyg_addrword_t data){
 // Get pointer to CThread object out of void pointer
  CECOSThread *pThread = static_cast<CECOSThread *>(data);

  // if pointer is ok
  if (pThread){
    CCriticalRegion criticalRegion(pThread->m_stResLock);
    pThread->setAlive(true);
    pThread->run();
    pThread->setAlive(false);
  }
  cyg_thread_exit();
}

CECOSThread::CECOSThread(long pa_nStackSize){
  m_nStackSize = pa_nStackSize;
  m_cStack = new unsigned char[m_nStackSize];
}

CECOSThread::~CECOSThread(){
  end();
  delete[] m_cStack;
}

void CECOSThread::setDeadline(const CIEC_TIME &pa_roVal){
  int i, ii;
  CCriticalRegion criticalRegion(sm_oThreadListLock);
  DEVLOG_DEBUG(">>>>Thread: Set Deadline: %lu\n", pa_roVal.operator TValueType ());
  mDeadLine = pa_roVal;
  //first of all check if this thread is already in the list and remove it from the list
  for(i = 0; i < scm_nThreadListSize; i++){
    if(0 == sm_aoThreadList[i])
      break;
    else
      if(this == sm_aoThreadList[i]){
        for(ii = i; ii < scm_nThreadListSize - 1; ii++){
          if(0 == sm_aoThreadList[ii + 1])
            break;
          sm_aoThreadList[ii + 1]->setPriority(ii + 2);
          sm_aoThreadList[ii] = sm_aoThreadList[ii + 1];  
        }
        break;
      }  
  }

  if(0 == mDeadLine)
    setPriority(scm_nThreadListSize + 2); // use the lowest user priority
  else{
    for(i = 0; i < scm_nThreadListSize; i++){
      if(0 == sm_aoThreadList[i]){
        sm_aoThreadList[i] = this;
        setPriority( i + 2);
        break;
      }  
      else
        if(mDeadLine < sm_aoThreadList[i]->getDeadline()){
          CECOSThread *poRBuf, *poSBuf= sm_aoThreadList[i];
          sm_aoThreadList[i] =  this;
          setPriority( i + 2);
          for(ii = i + 1; ii < scm_nThreadListSize; ii++){
            poSBuf->setPriority(ii + 2);
            poRBuf = sm_aoThreadList[ii];
            sm_aoThreadList[ii] = poSBuf;
            if(0 == poRBuf)
              break;
            poSBuf = poRBuf;  
          }  
          break;
        }
    }      
  }  
}

void CECOSThread::sleepThread(unsigned int pa_miliSeconds){
  usleep(1000 * pa_miliSeconds);
}
  
void CECOSThread::join(void){
  if(isAlive()){
    {CCriticalRegion criticalRegion(m_stResLock); }
    cyg_thread_delete(m_stHandle);
  }   
}
