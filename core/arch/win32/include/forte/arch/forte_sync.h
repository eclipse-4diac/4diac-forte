/*******************************************************************************
 * Copyright (c) 2010, 2020 ACIN, Profactor GmbH, fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial
 *documentation Jörg Walter - Windows XP compatibility
 *******************************************************************************/
#pragma once

#include <windows.h>

#define CSyncObject CPCSyncObject // allows that doxygen can generate better documenation

namespace forte::arch {
  /*! \ingroup win32_hal
   * \brief The sync object implementation for the win32 thread interface.
   *
   * In the win32 version a CRITICAL_SECTION is used for the sync object.
   *
   */

  class CPCSyncObject {
    public:
      CPCSyncObject();
      ~CPCSyncObject();

      /*!\brief Lock the resource coming after the lock command
       *
       * This function blocks until it will get the lock for the coming critical section.
       */
      void lock();

      //! Free the resource coming after the lock command
      void unlock();

      CPCSyncObject(const CPCSyncObject &) = delete;
      CPCSyncObject &operator=(const CPCSyncObject &) = delete;

    private:
#if _WIN32_WINNT >= 0x0600
      SRWLOCK mLock;
#else
      CRITICAL_SECTION mLock;
#endif
  };
} // namespace forte::arch
