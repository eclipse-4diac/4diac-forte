/************************************************************************************
 * Copyright (c) 2021, 2023 OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Jörg Walter - initial implementation
 ************************************************************************************/

#ifndef SRC_ARCH_FREERTOS_SOCKHAND_PLUSTCP_H_
#define SRC_ARCH_FREERTOS_SOCKHAND_PLUSTCP_H_

/*
 This is an emulation layer that defines enough of the classic BSD network API
 in terms of FreeRTOS-Plus-TCP so that FORTE can successfully establish and
 listen for TCP/IP connections. FreeRTOS-Plus-TCP is not BSD-compatible out
 of the box, but it is close enough so that some renaming of constants and a
 few helper functions provide just enough of the BSD API. This is useful
 because some targets in the official FreeRTOS distribution only have
 FreeRTOS-plus-TCP bindings and no lwIP support.
*/

#include <unordered_set>
#include <cstdint>

// remove fd_set and related definitions when including sys/types.h
#define fd_set LIBC_fd_set
#include <sys/types.h>
#undef fd_set
#undef FD_ZERO
#undef FD_SET
#undef FD_CLR
#undef FD_ISSET

// base headers
#include <FreeRTOS.h>
#include <FreeRTOS_Sockets.h>
#include <FreeRTOS_IP.h>
#include <FreeRTOS_TCP_IP.h>

// common constants
#define PF_INET FREERTOS_AF_INET
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define IPPROTO_IP -1 // not yet implemented
#define INADDR_ANY 0
#define SOL_SOCKET 0
#define SO_REUSEADDR -1 // not needed
#define IP_ADD_MEMBERSHIP -1 // not yet implemented

//////////////////////////////////////////////////////////////////////////////////////
// trivial function wrappers
//
// The socket file handle in the BSD API is cast from/to a FreeRTOS Socket_t,
// which happens to be the same size as an intptr_t
static inline BaseType_t setsockopt(intptr_t xSocket,
                                    int32_t lLevel,
                                    int32_t lOptionName,
                                    const void * pvOptionValue,
                                    size_t uxOptionLength ) {
  if (lOptionName == -1) return 0;
  return FreeRTOS_setsockopt(reinterpret_cast<Socket_t>(xSocket), lLevel, lOptionName, pvOptionValue, uxOptionLength);
}

static inline BaseType_t connect(intptr_t xClientSocket,
                                 struct freertos_sockaddr * pxAddress,
                                 socklen_t xAddressLength ) {
  return FreeRTOS_connect(reinterpret_cast<Socket_t>(xClientSocket), pxAddress, xAddressLength);
}

static inline BaseType_t bind(intptr_t xClientSocket,
                              struct freertos_sockaddr * pxAddress,
                              socklen_t xAddressLength ) {
  return FreeRTOS_bind(reinterpret_cast<Socket_t>(xClientSocket), pxAddress, xAddressLength);
}



#define htons(x) FreeRTOS_htons(x)
#define htonl(x) FreeRTOS_htonl(x)
#define socket(a,b,c) reinterpret_cast<intptr_t>(FreeRTOS_socket((a),(b),(c)))
#define listen(a,b) FreeRTOS_listen(reinterpret_cast<Socket_t>(a),(b))
#define accept(a,b,c) reinterpret_cast<intptr_t>(FreeRTOS_accept(reinterpret_cast<Socket_t>(a),(b),(c)))
#define recv(a,b,c,d) FreeRTOS_recv(reinterpret_cast<Socket_t>(a),(b),(c),(d))
#define recvfrom(a,b,c,d,e,f) FreeRTOS_recvfrom(reinterpret_cast<Socket_t>(a),(b),(c),(d),(e),(f))
#define send(a,b,c,d) FreeRTOS_send(reinterpret_cast<Socket_t>(a),(b),(c),(d))
#define sendto(a,b,c,d,e,f) FreeRTOS_sendto(reinterpret_cast<Socket_t>(a),(b),(c),(d),(e),(f))
#define close(x) FreeRTOS_closesocket(reinterpret_cast<Socket_t>(x))
#define inet_addr FreeRTOS_inet_addr

//////////////////////////////////////////////////////////////////////////////////////
// redefinition of common data types
#define sockaddr freertos_sockaddr

struct sockaddr_in {
  uint8_t sin_len;
  uint8_t sin_family;
  uint16_t sin_port;
  struct {
    uint32_t s_addr;
  } sin_addr;
  uint8_t sin_zero[0];
};

struct ip_mreq {
  struct {
    uint32_t s_addr;
  } imr_multiaddr, imr_interface;
; };

//////////////////////////////////////////////////////////////////////////////////////
// emulation of select() and fd_set
using fd_set = std::unordered_set<Socket_t>;
#define FD_ZERO(pSet) ((pSet)->clear())
#define FD_SET(fd, pSet) ((pSet)->insert(reinterpret_cast<Socket_t>(fd)))
#define FD_CLR(fd, pSet) ((pSet)->erase(reinterpret_cast<Socket_t>(fd)))
#define FD_ISSET(fd, pSet) ((pSet)->count(reinterpret_cast<Socket_t>(fd)))

static inline int select(intptr_t, fd_set *in, fd_set *out, fd_set *exc, timeval *timeout) {
  auto set = FreeRTOS_CreateSocketSet();
  if (in) for (auto rsock : *in) FreeRTOS_FD_SET(rsock, set, eSELECT_READ);
  if (out) for (auto rsock : *out) FreeRTOS_FD_SET(rsock, set, eSELECT_WRITE);
  if (exc) for (auto rsock : *exc) FreeRTOS_FD_SET(rsock, set, eSELECT_EXCEPT);

  TickType_t to = 0;
  if (timeout) {
    to = timeout->tv_sec * configTICK_RATE_HZ;
    to += timeout->tv_usec * configTICK_RATE_HZ / 1000000;
  }

  auto eventsock = FreeRTOS_select(set, to);

  int ret = 0;
  if (in) {
    bool active = (eventsock && FD_ISSET(eventsock, in));
    ret += active;
    FD_ZERO(in);
    if (active) FD_SET(eventsock, in);
  }

  if (out) {
    bool active = (eventsock && FD_ISSET(eventsock, out));
    ret += active;
    FD_ZERO(out);
    if (active) FD_SET(eventsock, out);
  }

  if (exc) {
    bool active = (eventsock && FD_ISSET(eventsock, exc));
    ret += active;
    FD_ZERO(exc);
    if (active) FD_SET(eventsock, exc);
  }

  FreeRTOS_DeleteSocketSet(set);
  return ret;
};

#endif /* SRC_ARCH_FREERTOS_SOCKHAND_PLUSTCP_H_ */
