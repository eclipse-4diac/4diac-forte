/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../forte_architecture.h"
#include <sockhand.h>

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
