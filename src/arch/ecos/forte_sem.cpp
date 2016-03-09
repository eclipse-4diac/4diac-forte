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

    CEcosSemaphore::CEcosSemaphore(unsigned int paInitialValue){
      cyg_semaphore_init(&mSemaphore, paInitialValue);
    }

    CEcosSemaphore::~CEcosSemaphore(){
      cyg_semaphore_destroy(&mSemaphore);
    }

    void CEcosSemaphore::semInc(){
      cyg_semaphore_post(&mSemaphore);
    }

    void CEcosSemaphore::semWaitIndefinitly(){
      cyg_semaphore_wait(&mSemaphore);
    }

  } /* namespace arch */
} /* namespace forte */
