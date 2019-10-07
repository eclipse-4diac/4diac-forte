/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_FREERTOS_SEMAPHORE_H_
#define SRC_ARCH_FREERTOS_SEMAPHORE_H_

#include <FreeRTOS.h>
#include <semphr.h>
#include "../datatype.h"

namespace forte {
  namespace arch {

    /*!\brief counting semaphore for syncing operation in FORTE
     *
     * The semaphore is initialized with the value given.
     */
    class CFreeRTOSSemaphore {
      public:

        explicit CFreeRTOSSemaphore(unsigned int paInitialValue = 0);
        ~CFreeRTOSSemaphore();

        void inc();
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
        SemaphoreHandle_t mSemaphore;
    };

    typedef CFreeRTOSSemaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_POSIX_SEMAPHORE_H_ */
