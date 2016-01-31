/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _SYNC_H_
#define _SYNC_H_

#define WIN32_LEAN_AND_MEAN
 #include <windows.h>

#define CSyncObject CPCSyncObject //allows that doxygen can generate better documenation


/*! \ingroup win32_hal
 * \brief The sync object implementation for the win32 thread interface.
 *
 * In the win32 version a CRITICAL_SECTION is used for the sync object.
 * 
 */
 
class CPCSyncObject{
  private:
  protected:
  //! The win32 CRITICAL_SECTION handle of the operating system.
	 CRITICAL_SECTION m_oMutexHandle;

  public:
    CPCSyncObject();
    ~CPCSyncObject();
  /*!\brief Lock the resource coming after the lock command  
   * 
   * This function blocks until it will get the lock for the coming critical section.
   */  
    void lock(void){
		EnterCriticalSection(&m_oMutexHandle);
      //TODO handle return value
    };
  //!Free the resource coming after the lock command  
    void unlock(void){
		LeaveCriticalSection(&m_oMutexHandle);
      //TODO handle return value
    };
};

#endif /*SYNC_H_*/
