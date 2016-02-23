/*******************************************************************************
 * Copyright (c) 2005 - 2016 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic, Martin Melik Merkiumians - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include <fortealloc.h>
#include "thread.h"
#include "../devlog.h"
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <criticalregion.h>

void CPosixThread::start(void){
  // Get new Thread
  if(0 == m_stThreadID){

    //If the stackSize is not 0 we have to set it
    if(m_nStackSize){
      pthread_attr_t stAttr;

      if(pthread_attr_init(&stAttr)){
        DEVLOG_ERROR("Error could not get the default thread attributes! %s\n", strerror(errno));
        return;
      }
  #ifdef __CYGWIN__
      if (pthread_attr_setstacksize (&stAttr, m_nStackSize)){
        DEVLOG_ERROR("Error could not set the stacksize for the thread! %s\n", strerror(errno));
        return;
      }
  #else
      //if (pthread_attr_setstacksize(&stAttr, m_nStackSize)) {
      if(pthread_attr_setstack(&stAttr, m_pacStack, m_nStackSize)){
        DEVLOG_ERROR("Error could not set the stacksize for the thread! %s\n", strerror(errno));
        return;
      }
  #endif
      if(pthread_create(&m_stThreadID, &stAttr, threadFunction, this)){
        DEVLOG_ERROR("Error could not create the thread! %s\n", strerror(errno));
        return;
      }
      if(pthread_attr_destroy(&stAttr)){
        DEVLOG_ERROR("Error could not free the thread attributes! %s\n", strerror(errno));
        return;
      }
    }
    else{
      if(pthread_create(&m_stThreadID, NULL, threadFunction, this)){
        DEVLOG_ERROR("Error could not create the thread! %s\n", strerror(errno));
        return;
      }
    }
  }
  //wait till the thread is up and running
  do{
    //pthread_yield();
    sleep(1);
  }while(!isAlive());
}

void * CPosixThread::threadFunction(void *arguments){
  // Get pointer to CThread object out of void pointer
  CPosixThread *pThread = (CPosixThread *) arguments;

  // if pointer is ok
  if(0 != pThread){
    CCriticalRegion criticalRegion(pThread->mJoinMutex);
    pThread->setAlive(true);
    pThread->run();
    pThread->setAlive(false);
    pThread->m_stThreadID = 0;
  }
  else{
    DEVLOG_ERROR("pThread pointer is 0!");
  }
  return 0;
}

CPosixThread::CPosixThread(long pa_nStackSize) :
      m_stThreadID(0), m_nStackSize(pa_nStackSize), m_pacStack(0){

  if(0 != m_nStackSize){
    m_pacStack = new char[m_nStackSize];
  }

  if(-1 == sem_init(&m_stSuspendSemaphore, 0, 0)){
    DEVLOG_ERROR("Could not initialize suspend sempaphore: %s\n", strerror(errno));
  }
}

CPosixThread::~CPosixThread(){
  if(0 != m_stThreadID){
    end();
  }
  if(0 != m_nStackSize){
    delete[] m_pacStack;
  }
  sem_destroy(&m_stSuspendSemaphore);
}

void CPosixThread::setDeadline(const CIEC_TIME &pa_roVal){
  m_oDeadline = pa_roVal;
  //under the posix pthread implemention currently it makes no sense to set any priority.
  //It will not be considered.
}

void CPosixThread::resumeSelfSuspend(void){
  sem_post(&m_stSuspendSemaphore);
}

void CPosixThread::join(void){
  if(0 != m_stThreadID){
    CCriticalRegion criticalRegion(mJoinMutex);
  }
}

void CPosixThread::selfSuspend(void) {
  int n;
  do{
    n = sem_wait(&m_stSuspendSemaphore);
  }while( (-1 == n) && (errno == EINTR) );  //handle interrupts from signals
}
