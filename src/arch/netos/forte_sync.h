/*******************************************************************************
 * Copyright (c) 2006 - 2010 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

#include <threadx/tx_api.h>

#define CSyncObject CTXSyncObject //allows that doxygen can generate better documenation

/*! \ingroup tnetos_hal 
 * \brief The sync object implementation for NET+OS.
 *
 * In the NET+OS version a mutex is used for the sync object.
 * 
 * @author AZ
 */
class CTXSyncObject{
  private:
  protected:
    TX_MUTEX m_stMutex;
  public:
    CTXSyncObject();
    ~CTXSyncObject();
  /*!\brief Lock the resource coming after the lock command  
   * 
   * This function blocks until it will get the lock for the coming critical section.
   */  
    void lock(void){
      tx_mutex_get(&m_stMutex, TX_WAIT_FOREVER);
      //TODO handle return value
    };
  //!Freee the resource coming after the lock command  
    void unlock(void){
      tx_mutex_put(&m_stMutex);
      //TODO handle return value
    };
};

#endif /*FORTE_SYNC_H_*/
