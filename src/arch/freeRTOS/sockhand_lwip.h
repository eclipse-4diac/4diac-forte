/************************************************************************************
 * Copyright (c) 2016, 2023 fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 * Jörg Walter - make exchangeable with FreeRTOS-plus-TCP
 ************************************************************************************/

#ifndef SRC_ARCH_FREERTOS_SOCKHAND_LWIP_H_
#define SRC_ARCH_FREERTOS_SOCKHAND_LWIP_H_

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

#undef connect //gets confused with connect function of conn.h and childs
#undef bind    //conflicts with a C++17 symbol used in FORTE

#if !defined(CONFIG_IDF_CMAKE)
inline int connect(int s, const struct sockaddr *name, socklen_t namelen) {
  return lwip_connect(s, name, namelen);
}
#endif

static inline int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
  return lwip_bind(sockfd, addr, addrlen);
}

#endif /* SRC_ARCH_FREERTOS_SOCKHAND_LWIP_H_ */
