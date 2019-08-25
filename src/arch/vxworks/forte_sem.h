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

#ifndef SRC_ARCH_RCX_SEMAPHORE_H_
#define SRC_ARCH_RCX_SEMAPHORE_H_

#include "vxWorks.h"
#include "semLib.h"
#include "datatype.h"

namespace forte {
  namespace arch {

    class CVxWorksSemaphore{
      public:
        explicit CVxWorksSemaphore(unsigned int paInitialValue  = 0);
        ~CVxWorksSemaphore();

        /** @brief Unlocks (increments) the semaphore
         *
         */
        void inc();
        /** @brief Waits until the semaphore can be locked
         *
         */
        void waitIndefinitely();

        /** @brief Checks if the semaphore is locked, and waits the specified amount of time if it is locked
         *
         * @param paRelativeTimeout - The relative time span to wait in nanoseconds
         * @return true - semaphore has become available before the timeout, false - semaphore was not available before timeout
         */
        bool timedWait(const TForteUInt64 paRelativeTimeout);

        /** @brief Tries to immediately get the semaphore, if it is available
         *
         * @return true - semaphore was available, false - semaphore was not available
         */
        bool tryNoWait();

      private:
        SEM_ID mSemaphore;
    };


    typedef CVxWorksSemaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_RCX_SEMAPHORE_H_ */
