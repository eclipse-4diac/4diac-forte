/*******************************************************************************
 * Copyright (c) 2016, 2020 fortiss GmbH, TU Vienna/ACIN, OFFIS e.V.
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
 *  Jörg Walter - make timed wait work withoout busy-loop, switch to binary
 *    semaphore
 *
 *******************************************************************************/

#include <cerrno>
#include <cstring>
#include <ctime>
#include <cassert>

#include "forte_sem.h"
#include "../devlog.h"
#include "../utils/timespec_utils.h"

namespace forte {
  namespace arch {

    CPThreadSemaphore::CPThreadSemaphore(bool paInitialValue)
      : mPosted(paInitialValue)
    {
      pthread_condattr_t condAttr;

      if (pthread_condattr_init(&condAttr) != 0) {
        DEVLOG_ERROR("Could not initialize cv attributes\n");
      }
      if (pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC) != 0) {
        DEVLOG_ERROR("Could not set cv clock\n");
      }
      if (pthread_cond_init(&mCond, &condAttr) != 0) {
        DEVLOG_ERROR("Could not initialize condition variable\n");
      }
      pthread_condattr_destroy(&condAttr);

      if (pthread_mutex_init(&mMutex, NULL) != 0) {
        DEVLOG_ERROR("Could not initialize mutex\n");
      }
    }

    
    CPThreadSemaphore::~CPThreadSemaphore(){
      pthread_cond_destroy(&mCond);
      pthread_mutex_destroy(&mMutex);
    }


    void CPThreadSemaphore::inc(){
      pthread_mutex_lock(&mMutex);
      mPosted = true;
      pthread_cond_signal(&mCond);
      pthread_mutex_unlock(&mMutex);
    }


    void CPThreadSemaphore::waitIndefinitely(){
      pthread_mutex_lock(&mMutex);
      while (!mPosted) {
        pthread_cond_wait(&mCond, &mMutex);
      }
      mPosted = false;
      pthread_mutex_unlock(&mMutex);
    }


    bool CPThreadSemaphore::timedWait(const TForteUInt64 paRelativeTimeout){
      pthread_mutex_lock(&mMutex);

      if (mPosted) {
        mPosted = false;
        pthread_mutex_unlock(&mMutex);
        return true;
      }

      timespec timeoutSpec = {
        static_cast<time_t>(paRelativeTimeout / 1000000000ULL),
        static_cast<time_t>(paRelativeTimeout % 1000000000ULL)
      };

      timespec currentTime = { 0, 0 };
      clock_gettime(CLOCK_MONOTONIC, &currentTime);

      timespec expectedAbsoluteTimeoutTime = { 0, 0 };
      timespecAdd(&currentTime, &timeoutSpec, &expectedAbsoluteTimeoutTime);

      int rc = 0;
      while (!mPosted && rc == 0) {
        rc = pthread_cond_timedwait(&mCond, &mMutex, &expectedAbsoluteTimeoutTime);
      }

      if (rc != 0 && rc != ETIMEDOUT) {
        DEVLOG_ERROR("Unexpected error during condition variable wait: %i\n", rc);
      }

      assert(!(rc == 0 && !mPosted)
             && (bool)"should have been posted when waiting successfully");

      bool success = (mPosted && rc == 0);
      if (success) {
        mPosted = false;
      }

      pthread_mutex_unlock(&mMutex);
      return success;
    }


    bool CPThreadSemaphore::tryNoWait(){
      pthread_mutex_lock(&mMutex);

      bool success = mPosted;
      mPosted = false;

      pthread_mutex_unlock(&mMutex);
      return success;
    }
  } /* namespace arch */
} /* namespace forte */
