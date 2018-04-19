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

#ifndef SRC_ARCH_RCX_SEMAPHORE_H_
#define SRC_ARCH_RCX_SEMAPHORE_H_

#include "vxWorks.h"
#include "semLib.h"

namespace forte {
  namespace arch {

    class CVxWorksSemaphore{
      public:
        explicit CVxWorksSemaphore(unsigned int paInitialValue  = 0);
        ~CVxWorksSemaphore();

        void semInc();
        void semWaitIndefinitly();

      private:
        SEM_ID m_oCountSemaphore;
    };


    typedef CVxWorksSemaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_RCX_SEMAPHORE_H_ */
