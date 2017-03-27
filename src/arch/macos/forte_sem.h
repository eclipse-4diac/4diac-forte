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

#ifndef SRC_ARCH_POSIX_SEMAPHORE_H_
#define SRC_ARCH_POSIX_SEMAPHORE_H_

#include <dispatch/dispatch.h>

namespace forte {
  namespace arch {

    /*!\brief counting semaphore for syncing operation in FORTE
     *
     * The semaphore is initialized with the value given.
     */
    class CPThreadSemaphore{
      public:

        explicit CPThreadSemaphore(unsigned int paInitialValue  = 0);
        ~CPThreadSemaphore();

        void semInc();
        void semWaitIndefinitly();

      private:
        dispatch_semaphore_t mSemaphore;
    };


    typedef CPThreadSemaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_POSIX_SEMAPHORE_H_ */
