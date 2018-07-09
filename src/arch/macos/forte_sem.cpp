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
#include <time.h>
#include <sys/time.h>

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

    void CPThreadSemaphore::inc(){
      dispatch_semaphore_signal(mSemaphore);
    }

    void CPThreadSemaphore::waitIndefinitely(){
      dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_FOREVER);
    }

    bool CPThreadSemaphore::timedWait(const TForteUInt64 paRelativeTimeout){
      struct timespec now = {0, 0};
      clock_gettime(CLOCK_MONOTONIC, &now);
      dispatch_time_t timeout = dispatch_walltime(&now, paRelativeTimeout)
      return (0 == dispatch_semaphore_wait(mSemaphore, timeout));
    }

    bool CPThreadSemaphore::tryNoWait(){
      return (0 == dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_NOW));
    }

  } /* namespace arch */
} /* namespace forte */
