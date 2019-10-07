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
#ifndef _SYNC_H_
#define _SYNC_H_

#include <FreeRTOS.h>
#include <semphr.h>

#define CSyncObject CFreeRTOSSyncObject //allows that doxygen can generate better documenation

/*! \ingroup FREERTOS-HAL
 * \brief The sync object implementation for FreeRTOS.
 *
 * In the FreeRTOS version we use a binary semaphore for the sync object
 * 
 * Note: For Mutex implementation,  uncomment the xSemaphoreCreateMutex() function in sync.cpp
 */

class CFreeRTOSSyncObject {
  public:
    CFreeRTOSSyncObject();
    ~CFreeRTOSSyncObject();

    /*!\brief Lock the resource coming after the lock command
     *
     * This function blocks until it will get the lock for the coming critical section.
     */
    void lock(void) {
      xSemaphoreTake(mMutexHandle, portMAX_DELAY);
    }

    //!Free the resource coming after the lock command
    void unlock(void) {
      xSemaphoreGive(mMutexHandle);
    }

  private:

    //! The posix thread mutex handle of the operating system.
    SemaphoreHandle_t mMutexHandle;
};

#endif /*SYNC_H_*/
