/*******************************************************************************
 * Copyright (c) 2010 - 2017 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include "forte_thread.h"
#include "../devlog.h"

DWORD WINAPI CWin32Thread::threadFunction(LPVOID arguments){
  CThreadBase::runThread(static_cast<CWin32Thread *>(arguments));
  return 0;
}

CWin32Thread::CWin32Thread(long paStackSize) : CThreadBase(paStackSize){
}

CWin32Thread::~CWin32Thread(){
}

void CWin32Thread::setDeadline(const CIEC_TIME &paVal){
  mDeadline = paVal;
  //under the win32 currently it makes no sense to set any priority.
  //It will not be considered.
}

forte::arch::CThreadBase<HANDLE>::TThreadHandleType CWin32Thread::createThread(long paStackSize){
    DWORD threadID;
    return CreateThread(0, // default security attributes
      paStackSize, // use default stack size
      threadFunction, // thread function name
      this, // argument to thread function
      0, // use default creation flags
      &threadID); // returns the thread identifier
}

void CWin32Thread::sleepThread(unsigned int paMilliSeconds){
  Sleep(paMilliSeconds);
}
