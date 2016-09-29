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

#include "forte_sem.h"
#include "../devlog.h"

namespace forte {
  namespace arch {

    CrcXSemaphore::CrcXSemaphore(unsigned int paInitialValue){
      if(RX_OK != rX_SemCreateSemaphore(0, mSemaphore, paInitialValue)){
        DEVLOG_ERROR("Could not initialize semaphore\n");
      }
    }

    CrcXSemaphore::~CrcXSemaphore(){
      rX_SemDeleteSemaphore(mSemaphore);
    }

    void CrcXSemaphore::semInc(){
      rX_SemPutSemaphore(mSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CrcXSemaphore::semWaitIndefinitly(){
      rX_SemGetSemaphore(mSemaphore);
      //FIXME what to dow if retval is not RX_OK?
    }

  } /* namespace arch */
} /* namespace forte */
