/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SOCKHAND_H_
#define SOCKHAND_H_

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define FORTE_SOCKET_TYPE SOCKET
#define FORTE_INVALID_SOCKET INVALID_SOCKET

//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "win32socketinterf.h"


typedef CGenericIPComSocketHandler<CFDSelectHandler, CWin32SocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
