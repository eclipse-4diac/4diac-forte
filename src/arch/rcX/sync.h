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
#ifndef _SYNC_H_
#define _SYNC_H_

#include <rX_Includes.h>

/*! \ingroup rcX_hal
 * \brief The sync object implementation for the rcX operating system.
 *
 * In the rcX version a mutex is used for the sync object.
 * 
 */

class CrcXSyncObject{
  public:
    CrcXSyncObject();
    ~CrcXSyncObject();

    /*!\brief Lock the resource coming after the lock command
     *
     * This function blocks until it will get the lock for the coming critical section.
     */
    void lock(void){
      rX_MtxLockMutex(mMutexHandle, RX_INFINITE);
      //TODO handle return value
    }

    //!Free the resource coming after the lock command
    void unlock(void){
      rX_MtxUnlockMutex(mMutexHandle);
      //TODO handle return value
    }

  private:
    RX_HANDLE mMutexHandle;
};

typedef CrcXSyncObject CSyncObject; //allows that doxygen can generate better documentation

#endif /*SYNC_H_*/
