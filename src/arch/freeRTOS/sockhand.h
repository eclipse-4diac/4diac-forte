/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_FREERTOS_SOCKHAND_H_
#define SRC_ARCH_FREERTOS_SOCKHAND_H_

#define LWIP_COMPAT_SOCKETS 1

#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/igmp.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"

#include <string.h>

#include "devlog.h"

#undef connect
//
inline int connect(int s, const struct sockaddr *name, socklen_t namelen){
return lwip_connect(s, name, namelen);
}

//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"

typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* SRC_ARCH_FREERTOS_SOCKHAND_H_ */

