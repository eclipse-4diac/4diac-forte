/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"
#include <errno.h>
#include <string.h>

namespace forte {
  namespace arch {

    CFreeRTOSSemaphore::CFreeRTOSSemaphore(unsigned int paInitialValue) {
      mSemaphore = xSemaphoreCreateBinary();
    }

    CFreeRTOSSemaphore::~CFreeRTOSSemaphore() {
      vSemaphoreDelete(mSemaphore);
    }

    void CFreeRTOSSemaphore::inc() {
      xSemaphoreGive(mSemaphore);
    }

    void CFreeRTOSSemaphore::waitIndefinitely() {
      if(mSemaphore != NULL) {
        xSemaphoreTake(mSemaphore, portMAX_DELAY);
      }
    }

    bool CFreeRTOSSemaphore::timedWait(const TForteUInt64 paRelativeTimeout) {
      if(mSemaphore != NULL) {
        return xSemaphoreTake(mSemaphore, pdMS_TO_TICKS(paRelativeTimeout / 1000000));
      }
      return false;
    }

    bool CFreeRTOSSemaphore::tryNoWait() {
      if(mSemaphore != NULL) {
        return xSemaphoreTake(mSemaphore, 0);
      }
      return false;
    }
  } /* namespace arch */
} /* namespace forte */
