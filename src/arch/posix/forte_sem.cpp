/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Peter Gsellmann, Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *******************************************************************************/

#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "forte_sem.h"
#include "../devlog.h"
#include "../utils/timespec_utils.h"

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

    void CPThreadSemaphore::inc(){
      sem_post(&mSemaphore);
    }

    void CPThreadSemaphore::waitIndefinitly(){
      while((-1 == sem_wait(&mSemaphore)) && (errno == EINTR)); //handle interrupts from signals
    }

    bool CPThreadSemaphore::timedWait(const TForteUInt64 paRelativeTimeout){
      timespec timeoutSpec = { paRelativeTimeout / scmSecondInNanoSeconds, (paRelativeTimeout % scmSecondInNanoSeconds) };
      timespec currentTime = { 0, 0 };
      clock_gettime(CLOCK_MONOTONIC, &currentTime);

      timespec expectedAbsoluteTimeoutTime = {0, 0};
      timespecAdd(&currentTime, &timeoutSpec, &expectedAbsoluteTimeoutTime);

      do{
        if(0 == sem_trywait(&mSemaphore)){
          return true;
        }
        clock_gettime(CLOCK_MONOTONIC, &currentTime);

      } while(timespecLessThan(&currentTime, &expectedAbsoluteTimeoutTime));
      return false;
    }

    bool CPThreadSemaphore::tryNoWait(){
      return (0 == sem_trywait(&mSemaphore));
    }
  } /* namespace arch */
} /* namespace forte */
