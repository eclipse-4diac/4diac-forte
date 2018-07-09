/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SOCKHAND_H_
#define SOCKHAND_H_

#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
//these include needs to be last
#include "../fdselecthand.h"
#include "win32socketinterf.h"
#include "../gensockhand.h"

typedef CGenericIPComSocketHandler<CFDSelectHandler, CWin32SocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
