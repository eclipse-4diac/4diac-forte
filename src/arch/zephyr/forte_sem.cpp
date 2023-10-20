/************************************************************************************
 Copyright (c) 2023 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"

namespace forte {
  namespace arch {

    CZephyrSemaphore::CZephyrSemaphore(unsigned int paInitialValue) {
      sema = static_cast<struct k_sem*>(k_malloc(sizeof(struct k_sem)));
      k_sem_init(sema, paInitialValue, 1);
    }

    CZephyrSemaphore::~CZephyrSemaphore() {
      k_free(sema);
    }

    void CZephyrSemaphore::inc() {
      k_sem_give(sema);
    }

    void CZephyrSemaphore::waitIndefinitely() {
      k_sem_take(sema, K_FOREVER);
    }

    bool CZephyrSemaphore::timedWait(const TForteUInt64 paRelativeTimeout) {
      return !k_sem_take(sema, K_NSEC(paRelativeTimeout));
    }

    bool CZephyrSemaphore::tryNoWait() {
      return !k_sem_take(sema, K_NO_WAIT);
    }
  } /* namespace arch */
} /* namespace forte */
