/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
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

    void CEcosSemaphore::inc(){
      cyg_semaphore_post(&mSemaphore);
    }

    void CEcosSemaphore::waitIndefinitly(){
      cyg_semaphore_wait(&mSemaphore);
    }

    bool CEcosSemaphore::timedWait(TForteUInt64 paRelativeTimeout){
      cyg_tick_count_t absWaitTime = cyg_current_time() + paRelativeTimeout * CYGNUM_HAL_RTC_NUMERATOR / CYGNUM_HAL_RTC_DENOMINATOR;
      return (0 != cyg_semaphore_timed_wait(&mSemaphore, absWaitTime));
    }

    bool CEcosSemaphore::tryNoWait(){
      return (0 != cyg_semaphore_trywait());
    }

  } /* namespace arch */
} /* namespace forte */
