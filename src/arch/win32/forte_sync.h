/*******************************************************************************
 * Copyright (c) 2010, 2020 ACIN, Profactor GmbH, fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *  Jörg Walter - Windows XP compatibility
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

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
  public:
    CPCSyncObject();
    ~CPCSyncObject();

    /*!\brief Lock the resource coming after the lock command
     *
     * This function blocks until it will get the lock for the coming critical section.
     */
    void lock(void);

    //!Free the resource coming after the lock command
    void unlock(void);

  private:
    CPCSyncObject(const CPCSyncObject &); /* = delete; */
    CPCSyncObject &operator=(const CPCSyncObject &); /* = delete; */

#ifdef WINDOWS_XP_COMPAT
    CRITICAL_SECTION mLock;
#else
    SRWLOCK mLock;
#endif
};

#endif /*FORTE_SYNC_H_*/
