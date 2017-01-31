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
      mSemaphore = dispatch_semaphore_create(paInitialValue);

      if(0 == mSemaphore){
        DEVLOG_ERROR("Could not initialize suspend sempaphore: %s\n", strerror(errno));
      }
    }

    CPThreadSemaphore::~CPThreadSemaphore(){
      dispatch_release(mSemaphore);
    }

    void CPThreadSemaphore::semInc(){
      dispatch_semaphore_signal(mSemaphore);
    }

    void CPThreadSemaphore::semWaitIndefinitly(){
      dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_FOREVER);
    }

  } /* namespace arch */
} /* namespace forte */
