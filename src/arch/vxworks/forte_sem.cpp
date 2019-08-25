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
 *  Martin Melik-Merkumians - changes to binary semaphore
 *******************************************************************************/

#include "forte_sem.h"
#include <sysLib.h>

namespace forte {
  namespace arch {

    CVxWorksSemaphore::CVxWorksSemaphore(unsigned int paInitialValue){
      mSemaphore = semBCreate(SEM_Q_FIFO, paInitialValue > 0 ? SEM_FULL : SEM_EMPTY);
      if(SEM_ID_NULL == mSemaphore){
        //TODO: check return value for out_of_memory error
      }
    }

    CVxWorksSemaphore::~CVxWorksSemaphore(){
      semDelete(mSemaphore);
    }

    void CVxWorksSemaphore::inc(){
      semGive(mSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CVxWorksSemaphore::waitIndefinitely(){
      semTake(mSemaphore, WAIT_FOREVER);
      //FIXME what to dow if retval is not RX_OK?
    }

    bool CVxWorksSemaphore::timedWait(TForteUInt64 paRelativeTimeout){
      //TODO: nanoseconds to timer ticks
      return (OK == semTake(mSemaphore, static_cast<_Vx_ticks_t>(
          (static_cast<double>(paRelativeTimeout) / 1E9) *
          sysClkRateGet())));
    }

    bool CVxWorksSemaphore::tryNoWait(){
      return (OK == semTake(mSemaphore, NO_WAIT));
    }

  } /* namespace arch */
} /* namespace forte */
