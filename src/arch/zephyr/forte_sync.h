/************************************************************************************
 Copyright (c) 2023 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef _SYNC_H_
#define _SYNC_H_

#include <zephyr/kernel.h>

#define CSyncObject CZephyrSyncObject //allows that doxygen can generate better documenation

/*! \ingroup ZEPHYR-HAL
 * \brief The sync object implementation for Zephyr.
 *
 * In the Zephyr version we use a binary semaphore for the sync object
 * 
 * Note: For Mutex implementation,  uncomment the xSemaphoreCreateMutex() function in sync.cpp
 */

class CZephyrSyncObject {
  public:
    CZephyrSyncObject();
    ~CZephyrSyncObject();

    /*!\brief Lock the resource coming after the lock command
     *
     * This function blocks until it will get the lock for the coming critical section.
     */
    void lock() {
      k_mutex_lock(mutex, K_FOREVER);
    }

    //!Free the resource coming after the lock command
    void unlock() {
      k_mutex_unlock(mutex);
    }

  private:
    struct k_mutex* mutex{ 0 };
};

#endif /*SYNC_H_*/
