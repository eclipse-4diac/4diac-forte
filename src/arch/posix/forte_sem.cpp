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
      if(-1 == sem_init(&mSemaphore, 0, paInitialValue)){
        DEVLOG_ERROR("Could not initialize suspend sempaphore: %s\n", strerror(errno));
      }
    }

    CPThreadSemaphore::~CPThreadSemaphore(){
      sem_destroy(&mSemaphore);
    }

    void CPThreadSemaphore::semInc(){
      sem_post(&mSemaphore);
    }

    void CPThreadSemaphore::semWaitIndefinitly(){
      int n;
      do{
        n = sem_wait(&mSemaphore);
      } while((-1 == n) && (errno == EINTR)); //handle interrupts from signals
    }

  } /* namespace arch */
} /* namespace forte */
