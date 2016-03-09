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

    CPThreadSemaphore::CPThreadSemaphore(unsigned int paInitialValue){
      m_hSelfSuspendSemaphore = CreateSemaphore(NULL, paInitialValue, 10, NULL);
      if(0 == m_hSelfSuspendSemaphore){
        DEVLOG_ERROR("Could not initialize suspend semphore: %d\n", GetLastError());
      }
    }

    CPThreadSemaphore::~CPThreadSemaphore(){
      CloseHandle(mSemaphore);
    }

    void CPThreadSemaphore::semInc(){
      ReleaseSemaphore(m_hSelfSuspendSemaphore, 1, 0);
    }

    void CPThreadSemaphore::semWaitIndefinitly(){
      WaitForSingleObject(m_hSelfSuspendSemaphore, INFINITE);
    }

  } /* namespace arch */
} /* namespace forte */
