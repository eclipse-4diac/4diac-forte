/*******************************************************************************
 * Copyright (c) 2021 SYSGO GmbH
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
 *  Thomas Wagner - copy from "arch/posix": CLOCK_MONOTONIC was replaced by
 *                  CLOCK_REALTIME, which is required by PikeOS Posix.
 *******************************************************************************/

#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "forte_sem.h"
#include "arch/devlog.h"
#include "arch/utils/timespec_utils.h"

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

    void CPThreadSemaphore::waitIndefinitely(){
      while((-1 == sem_wait(&mSemaphore)) && (errno == EINTR)); //handle interrupts from signals
    }

    bool CPThreadSemaphore::timedWait(const TForteUInt64 paRelativeTimeout){
      timespec timeoutSpec = { static_cast<time_t>(paRelativeTimeout / scmSecondInNanoSeconds), static_cast<time_t>(paRelativeTimeout % scmSecondInNanoSeconds) };
      timespec currentTime = { 0, 0 };
      clock_gettime(CLOCK_REALTIME, &currentTime);

      timespec expectedAbsoluteTimeoutTime = {0, 0};
      timespecAdd(&currentTime, &timeoutSpec, &expectedAbsoluteTimeoutTime);

      do{
        if(0 == sem_trywait(&mSemaphore)){
          return true;
        }
        clock_gettime(CLOCK_REALTIME, &currentTime);

      } while(timespecLessThan(&currentTime, &expectedAbsoluteTimeoutTime));
      return false;
    }

    bool CPThreadSemaphore::tryNoWait(){
      return (0 == sem_trywait(&mSemaphore));
    }
  } /* namespace arch */
} /* namespace forte */
