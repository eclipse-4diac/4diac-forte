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
 *******************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"
#include <errno.h>
#include <string.h>

namespace forte {
  namespace arch {

    CNetOSSemaphore::CNetOSSemaphore(unsigned int paInitialValue){
      tx_semaphore_create(&mSemaphore, "hugo", paInitialValue);
    }

    CNetOSSemaphore::~CNetOSSemaphore(){
      tx_semaphore_delete(&mSemaphore);
    }

    void CNetOSSemaphore::inc(){
      tx_semaphore_put(&mSemaphore);
    }

    void CNetOSSemaphore::waitIndefinitely(){
      tx_semaphore_get(&mSemaphore, TX_WAIT_FOREVER);
    }

    bool CNetOSSemaphore::timedWait(TForteUInt64 paRelativeTimeout) {
      //TODO: Handle ticks to nanoseconds
      return (0 == tx_semaphore_get(&mSemaphore, paRelativeTimeout));
    }

    bool tryNoWait() {
      return (0 == tx_semaphore_get(&mSemaphore, TX_NO_WAIT));
    }

  } /* namespace arch */
} /* namespace forte */
