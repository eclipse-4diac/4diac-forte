/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../forte_architecture.h"
#include <sockhand.h>

bool CForteArchitecture::mInitialized = false;

bool CForteArchitecture::initialize() {
  if (!mInitialized) {
    // Windows Socket Startupcode
    WORD wVersionRequested;
    WSADATA wsaData;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    WSAStartup(wVersionRequested, &wsaData);
    mInitialized = true;
  }
  return true;
}

void CForteArchitecture::deinitialize() {
  if (mInitialized) {
    WSACleanup();
    mInitialized = false;
  }
}
