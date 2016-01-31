/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN
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

#include <pthread.h>

#define CSyncObject CPCSyncObject //allows that doxygen can generate better documenation


/*! \ingroup posix_hal
 * \brief The sync object implementation for the posix thread interface.
 *
 * In the posix version a mutex is used for the sync object.
 * 
 */
 
class CPCSyncObject{
  private:
  protected:
  //! The posix thread mutex handle of the operating system.
    pthread_mutex_t m_oMutexHandle;
  public:
    CPCSyncObject();
    ~CPCSyncObject();
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

#endif /*SYNC_H_*/
