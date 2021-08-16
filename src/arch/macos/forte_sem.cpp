/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *  Martin Melik-Merkumians - Changes to binary semaphore
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
      mSemaphore = dispatch_semaphore_create(0); //Creates binary semaphore according to developer help

      if(0 == paInitialValue){ // If inital value is zero, semaphore will be claimed
        dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_NOW);
      }

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
      return (0 == dispatch_semaphore_wait(mSemaphore, dispatch_time(DISPATCH_TIME_NOW, static_cast<int64_t>(paRelativeTimeout))));
    }

    bool CPThreadSemaphore::tryNoWait(){
      return (0 == dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_NOW));
    }

  } /* namespace arch */
} /* namespace forte */
