/*******************************************************************************
 * Copyright (c) 2010 - 2014 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "thread.h"
#include "../devlog.h"

DWORD WINAPI CWin32Thread::threadFunction(LPVOID arguments){
  // Get pointer to CThread object out of void pointer
  CWin32Thread *pThread = (CWin32Thread *) arguments;

  // if pointer is ok
  if(0 != pThread){
    pThread->m_bAlive = true;
    pThread->run();
    pThread->m_bAlive = false;
    pThread->m_nThreadHandle = 0;
  }
  else{
    DEVLOG_ERROR("pThread pointer is 0!");
  }
  return 0;
}

CWin32Thread::CWin32Thread(long pa_nStackSize) :
    m_bAlive(false), m_nThreadHandle(0), m_nStackSize(pa_nStackSize), m_nThreadID(~0x0ul){

  m_hSelfSuspendSemaphore = CreateSemaphore(NULL, 0, 10, NULL);

  if(0 == m_hSelfSuspendSemaphore){
    DEVLOG_ERROR("Could not initialize suspend semphore: %d\n", GetLastError());
  }
}

CWin32Thread::~CWin32Thread(){
  if(0 != m_nThreadHandle){
    end();
  }
  CloseHandle(m_hSelfSuspendSemaphore);
}

bool CWin32Thread::isAlive(void) const{
  return m_bAlive;
}

void CWin32Thread::setDeadline(const CIEC_TIME &pa_roVal){
  m_oDeadline = pa_roVal;
//under the win32 currently it makes no sense to set any priority.
//It will not be considered.
}

const CIEC_TIME &CWin32Thread::getDeadline(void) const{
  return m_oDeadline;
}

void CWin32Thread::start(void){
  if(0 == m_nThreadHandle){

    m_nThreadHandle = CreateThread(0, // default security attributes
      m_nStackSize, // use default stack size
      threadFunction, // thread function name
      this, // argument to thread function
      0, // use default creation flags
      &m_nThreadID); // returns the thread identifier

    if(0 == m_nThreadHandle){
      DEVLOG_ERROR("Error could not create the thread!");
    }
  }
}

void CWin32Thread::selfSuspend(void){
  WaitForSingleObject(m_hSelfSuspendSemaphore, INFINITE);
}

void CWin32Thread::resumeSelfSuspend(void){
  ReleaseSemaphore(m_hSelfSuspendSemaphore, 1, 0);
}

void CWin32Thread::end(void){
  m_bAlive = false;
  resumeSelfSuspend();
  join();
}

void CWin32Thread::join(void){
  //check if the thread is still running and that we are not calling join from inside the thread
  if((0 != m_nThreadHandle) && (GetCurrentThreadId() != m_nThreadID)){
    WaitForSingleObject(m_nThreadHandle, INFINITE);
  }
}
