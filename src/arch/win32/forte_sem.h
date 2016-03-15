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

#ifndef SRC_ARCH_WIN32_SEMAPHORE_H_
#define SRC_ARCH_WIN32_SEMAPHORE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace forte {
  namespace arch {

    /*!\brief counting semaphore for syncing operation in FORTE
     *
     * The semaphore is initialized with the value given.
     */
    class CWin32Semaphore{
      public:

        explicit CWin32Semaphore(unsigned int paInitialValue  = 0);
        ~CWin32Semaphore();

        void semInc();
        void semWaitIndefinitly();

      private:
        HANDLE mSemaphore;
    };


    typedef CWin32Semaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_POSIX_SEMAPHORE_H_ */
