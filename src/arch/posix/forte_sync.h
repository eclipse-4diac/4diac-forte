/*******************************************************************************
 * Copyright (c) 2005, 2020 ACIN, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Jörg Walter - make objects non-copyable
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

#include <pthread.h>

namespace forte {
  namespace arch {
    //forward declaration of CPThreadSemaphore so that we can use it in friend
    class CPThreadSemaphore;
  }
}


/*! \ingroup posix_hal
 * \brief The sync object implementation for the posix thread interface.
 *
 * In the posix version a mutex is used for the sync object.
 * 
 */
 
class CPThreadSyncObject{
  public:
    CPThreadSyncObject();
    ~CPThreadSyncObject();

    /*!\brief Lock the resource coming after the lock command
     *
     * This function blocks until it will get the lock for the coming critical section.
     */
    void lock(void){
      pthread_mutex_lock(&mMutex);
      //TODO handle return value
    }

    //!Free the resource coming after the lock command
    void unlock(void){
      pthread_mutex_unlock(&mMutex);
      //TODO handle return value
    }


  private:
    //! Accessor method to the mutex allowing platform specific code to use this sync object class.
    pthread_mutex_t *getPosixMutex(){
      return &mMutex;
    }

    // prevent copies, since pthread_mutex_t may not be copied
    CPThreadSyncObject(const CPThreadSyncObject &); /* = delete; */
    CPThreadSyncObject &operator=(const CPThreadSyncObject &); /* = delete; */

    //! The posix thread mutex handle of the operating system.
    pthread_mutex_t mMutex;

    friend class forte::arch::CPThreadSemaphore;
};

typedef CPThreadSyncObject CSyncObject; //allows that doxygen can generate better documenation

#endif /*FORTE_SYNC_H_*/
