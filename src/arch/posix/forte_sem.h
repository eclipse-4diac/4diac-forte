/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH, TU Vienna/ACIN,
 *               2020 OFFIS e.V., Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Peter Gsellmann, Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *  Jörg Walter - make timed wait work without busy-loop, switch to binary
 *    semaphore
 *  Alois Zoitl - changed from pthread mutex to CPThreadSyncObject
 *******************************************************************************/

#ifndef SRC_ARCH_POSIX_SEMAPHORE_H_
#define SRC_ARCH_POSIX_SEMAPHORE_H_

#include <pthread.h>
#include "datatype.h"
#include "forte_sync.h"

namespace forte {
  namespace arch {

    /*!\brief binary semaphore for syncing operation in FORTE
     *
     * The semaphore is initialized with the value given.
     */
    class CPThreadSemaphore{
      public:

        explicit CPThreadSemaphore(bool paInitialValue = false);
        ~CPThreadSemaphore();

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
        /* TODO: in C++11, this class should be properly made non-copyable since
         * mutexes cannot be copied after initialisation

        CPThreadSemaphore(const CPThreadSemaphore &) = delete;
        CPThreadSemaphore &operator=(const CPThreadSemaphore &) = delete;
        */
        CPThreadSemaphore(const CPThreadSemaphore &);
        CPThreadSemaphore &operator=(const CPThreadSemaphore &);

        /* Implementation is based on POSIX condition variables instead of POSIX
         * semaphores, because POSIX semaphores cannot safely wait without busy
         * looping. Derived from https://stackoverflow.com/a/57496953 */

        CPThreadSyncObject mMutex;
        pthread_cond_t mCond;

        bool mPosted;

    };

    typedef CPThreadSemaphore CSemaphore;

  } /* namespace arch */
} /* namespace forte */

#endif /* SRC_ARCH_POSIX_SEMAPHORE_H_ */
