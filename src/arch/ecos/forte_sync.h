/*******************************************************************************
 * Copyright (c) 2006, 2007 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

#include <cyg/kernel/kapi.h>

#define CSyncObject CECOSSyncObject //allows that doxygen can generate better documenation

/*! \ingroup ECOS-HAL
 * \brief The sync object implementation for eCos.
 *
 * In the eCos version a mutex is used for the sync object.
 */
class CECOSSyncObject{
  private:
  protected:
  //! The posix thread mutex handle of the operating system.
    cyg_mutex_t m_oMutexHandle;
  public:
    CECOSSyncObject();
    ~CECOSSyncObject();
  /*!\brief Lock the resource coming after the lock command  
   * 
   * This function blocks until it will get the lock for the coming critical section.
   */  
    void lock(void){
      cyg_mutex_lock(&m_oMutexHandle);
    };
  //!Freee the resource coming after the lock command  
    void unlock(void){
      cyg_mutex_unlock(&m_oMutexHandle);
    };
};

#endif /*FORTE_SYNC_H_*/
