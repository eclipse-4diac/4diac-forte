/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
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
        if(RX_OK != rX_SemCreateSemaphore(&semaphoreName[0], mSemaphore, paInitialValue)){
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

    void CrcXSemaphore::semInc(){
      rX_SemPutSemaphore(mSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CrcXSemaphore::semWaitIndefinitly(){
      rX_SemWaitForSemaphore(mSemaphore, RX_INFINITE);
      //FIXME what to dow if retval is not RX_OK?
    }

  } /* namespace arch */
} /* namespace forte */
