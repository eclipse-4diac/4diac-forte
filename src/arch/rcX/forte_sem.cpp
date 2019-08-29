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

#include "rcXUtilities.h"
#include "forte_sem.h"
#include "../devlog.h"
#include <fortenew.h>

namespace forte {
  namespace arch {

    CrcXSemaphore::CrcXSemaphore(unsigned int paInitialValue){
      mSemaphore = forte_malloc(RX_SEMAPHORE_SIZE);
      if(0 == mSemaphore){
        DEVLOG_ERROR("Not enough memory to allocate %i bytes for creating a new semaphore\n", RX_SEMAPHORE_SIZE);
      }else{
        char semaphoreName[8];
        getRandomString(&semaphoreName[0], 7);
        if(RX_OK != rX_SemCreateSemaphore(&semaphoreName[0], mSemaphore, (paInitialValue > 0 ? 1 : 0))){
          DEVLOG_ERROR("Could not initialize semaphore\n");
        }
      }
    }

    CrcXSemaphore::~CrcXSemaphore(){
      if(0 != mSemaphore){
        rX_SemDeleteSemaphore(mSemaphore);
        forte_free(mSemaphore);
      }
    }

    void CrcXSemaphore::inc(){
      rX_SemClearSemaphoreCount(mSemaphore); //Resets count to zero
      rX_SemPutSemaphore(mSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CrcXSemaphore::waitIndefinitely(){
      rX_SemWaitForSemaphore(mSemaphore, RX_INFINITE);
      //FIXME what to dow if retval is not RX_OK?
    }

    bool CrcXSemaphore::timedWait(const TForteUInt64 paRelativeTimeout){
      //TODO: Conversion from nanoseconds into timer ticks
      return (0 == srX_SemWaitForSemaphore(mSemaphore, paRelativeTimeout));
    }

    bool CrcXSemaphore::tryNoWait(){
      return (0 == srX_SemWaitForSemaphore(mSemaphore, RX_FINITE));
    }


  } /* namespace arch */
} /* namespace forte */
