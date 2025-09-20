/*******************************************************************************
 * Copyright (c) 2010, 2011, 2017 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/arch/forte_sync.h"

namespace forte::arch {

  CPCSyncObject::CPCSyncObject() : mLock(SRWLOCK_INIT) {
  }

  CPCSyncObject::~CPCSyncObject() {
    // we don't need to do anything here
  }

  void CPCSyncObject::lock() {
    AcquireSRWLockExclusive(&mLock);
  }

  //! Free the resource coming after the lock command
  void CPCSyncObject::unlock() {
    ReleaseSRWLockExclusive(&mLock);
  }
} // namespace forte::arch
