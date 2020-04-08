/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Peter Gsellmann, Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *******************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"
#include <errno.h>
#include <string.h>

namespace forte {
  namespace arch {

    CWin32Semaphore::CWin32Semaphore(unsigned int paInitialValue){
      mSemaphore = CreateSemaphore(NULL, (paInitialValue > 1 ? 1 : 0), 1, NULL);
      if(0 == mSemaphore){
        DEVLOG_ERROR("Could not initialize suspend semaphore: %d\n", GetLastError());
      }
    }

    CWin32Semaphore::~CWin32Semaphore(){
      CloseHandle(mSemaphore);
    }

    void CWin32Semaphore::inc(){
      ReleaseSemaphore(mSemaphore, 1, 0);
    }

    void CWin32Semaphore::waitIndefinitely(){
      WaitForSingleObject(mSemaphore, INFINITE);
    }

    bool CWin32Semaphore::timedWait(TForteUInt64 paRelativeTimeout){
      return (0 == WaitForSingleObject(mSemaphore, static_cast<DWORD>(paRelativeTimeout / 1000000 /* 1E6*/)));
    }

    bool CWin32Semaphore::tryNoWait(){
      return (0 == WaitForSingleObject(mSemaphore, 0));
    }

  } /* namespace arch */
} /* namespace forte */
