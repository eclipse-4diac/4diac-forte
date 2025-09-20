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

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define FORTE_SOCKET_TYPE SOCKET
#define FORTE_INVALID_SOCKET INVALID_SOCKET

// these include needs to be last
#include "forte/arch/gensockhand.h"
#include "forte/arch/fdselecthand.h"
#include "forte/arch/win32socketinterf.h"

namespace forte::arch {
  typedef CGenericIPComSocketHandler<CFDSelectHandler, CWin32SocketInterface> CIPComSocketHandler;
}
