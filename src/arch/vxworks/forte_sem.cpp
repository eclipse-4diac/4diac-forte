/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *******************************************************************************/

#include "forte_sem.h"
//#include "../devlog.h"

namespace forte {
  namespace arch {

    CVxWorksSemaphore::CVxWorksSemaphore(unsigned int paInitialValue){
      mCountSemaphore = semCCreate(SEM_Q_FIFO, paInitialValue);
      if(SEM_ID_NULL == mCountSemaphore){
        //TODO: check return value for out_of_memory error
      }
    }

    CVxWorksSemaphore::~CVxWorksSemaphore(){
      semDelete(mCountSemaphore);
    }

    void CVxWorksSemaphore::inc(){
      semGive(mCountSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CVxWorksSemaphore::waitIndefinitely(){
      semTake(mCountSemaphore, WAIT_FOREVER);
      //FIXME what to dow if retval is not RX_OK?
    }

    bool CVxWorksSemaphore::timedWait(TForteUInt64 paRelativeTimeout){
      //TODO: nanoseconds to timer ticks
      return (OK == semTake(mCountSemaphore, paRelativeTimeout));
    }

    bool CVxWorksSemaphore::tryNoWait(){
      return (OK == semTake(mCountSemaphore, NO_WAIT));
    }

  } /* namespace arch */
} /* namespace forte */
