/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"
#include <errno.h>
#include <string.h>

namespace forte {
  namespace arch {

    CWin32Semaphore::CWin32Semaphore(unsigned int paInitialValue){
      mSemaphore = CreateSemaphore(NULL, paInitialValue, 10, NULL);
      if(0 == mSemaphore){
        DEVLOG_ERROR("Could not initialize suspend semaphore: %d\n", GetLastError());
      }
    }

    CWin32Semaphore::~CWin32Semaphore(){
      CloseHandle(mSemaphore);
    }

    void CWin32Semaphore::semInc(){
      ReleaseSemaphore(mSemaphore, 1, 0);
    }

    void CWin32Semaphore::semWaitIndefinitly(){
      WaitForSingleObject(mSemaphore, INFINITE);
    }

  } /* namespace arch */
} /* namespace forte */
