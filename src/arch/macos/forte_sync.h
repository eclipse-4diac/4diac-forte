/*******************************************************************************
 * Copyright (c) 2005 - 2010, 2016 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic - initial API and implementation and/or initial
 *                             documentation
 *  Alois Zoitl  - taken this file from the Posix version and adapted it for
 *                 Mac OS
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

#include <pthread.h>

/*! \ingroup mac_hal
 * \brief The sync object implementation for the posix thread interface on Mac Os.
 *
 * In the Mac OS version a mutex is used for the sync object.
 */
 
class CMacOsSyncObject{
  private:
  protected:
  //! The posix thread mutex handle of the operating system.
    pthread_mutex_t m_oMutexHandle;
  public:
    CMacOsSyncObject();
    ~CMacOsSyncObject();
  /*!\brief Lock the resource coming after the lock command  
   * 
   * This function blocks until it will get the lock for the coming critical section.
   */  
    void lock(void){
      pthread_mutex_lock(&m_oMutexHandle);
      //TODO handle return value
    };
  //!Freee the resource coming after the lock command  
    void unlock(void){
      pthread_mutex_unlock(&m_oMutexHandle);
      //TODO handle return value
    };
};

typedef CMacOsSyncObject CSyncObject;  //allows that doxygen can generate better documenation


#endif /*FORTE_SYNC_H_*/
