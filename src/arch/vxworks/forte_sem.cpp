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
//#include "../devlog.h"

namespace forte {
  namespace arch {

    CVxWorksSemaphore::CVxWorksSemaphore(unsigned int paInitialValue){
      m_oCountSemaphore = semCCreate(SEM_Q_FIFO, paInitialValue);
      if(SEM_ID_NULL == m_oCountSemaphore){
        //TODO: check return value for out_of_memory error
      }
    }

    CVxWorksSemaphore::~CVxWorksSemaphore(){
      semDelete(m_oCountSemaphore);
    }

    void CVxWorksSemaphore::semInc(){
      semGive(m_oCountSemaphore);
      //FIXME what to dow if retval is not RX_OK?
      //for the current use in 4diac an overflow of the sem counter can
      //be ignored if afterwards the value is not zero
    }

    void CVxWorksSemaphore::semWaitIndefinitly(){
      semTake(m_oCountSemaphore, WAIT_FOREVER);
      //FIXME what to dow if retval is not RX_OK?
    }

  } /* namespace arch */
} /* namespace forte */
