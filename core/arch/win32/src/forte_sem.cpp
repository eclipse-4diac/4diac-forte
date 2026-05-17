/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Peter Gsellmann, Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *******************************************************************************/

#include "forte/arch/forte_sem.h"
#include "forte/util/devlog.h"
#include <errno.h>
#include <string.h>

namespace forte::arch {
  CWin32Semaphore::CWin32Semaphore(bool paInitialValue) {
    mEvent = CreateEvent(nullptr, FALSE, paInitialValue ? TRUE : FALSE, nullptr);
    if (0 == mEvent) {
      DEVLOG_ERROR("Could not initialize suspend semaphore: %d\n", GetLastError());
    }
  }

  CWin32Semaphore::~CWin32Semaphore() {
    CloseHandle(mEvent);
  }

  void CWin32Semaphore::inc() {
    SetEvent(mEvent);
  }

  void CWin32Semaphore::waitIndefinitely() {
    WaitForSingleObject(mEvent, INFINITE);
  }

  bool CWin32Semaphore::timedWait(TForteUInt64 paRelativeTimeout) {
    return (0 == WaitForSingleObject(mEvent, static_cast<DWORD>(paRelativeTimeout / 1000000 /* 1E6*/)));
  }

  bool CWin32Semaphore::tryNoWait() {
    return (0 == WaitForSingleObject(mEvent, 0));
  }
} // namespace forte::arch
