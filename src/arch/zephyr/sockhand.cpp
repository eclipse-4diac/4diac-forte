/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "sockhand.h"

void setupMulticastGroup(char* paIPAddr, const char* paMCInterface) {
  // setting up multicast group
  struct in_addr ifaddr;
  ifaddr.s_addr = inet_addr(paMCInterface);
  struct net_if* iface = nullptr;

  if (nullptr == net_if_ipv4_addr_lookup(&ifaddr, &iface)) {
    DEVLOG_WARNING("CBSDSocketInterface: net_if_ipv4_addr_lookup failed: %s - fall back to default if\n", paMCInterface);
    iface = net_if_get_default();
  }
  if (nullptr != iface) {
    DEVLOG_INFO("CBSDSocketInterface: multicast using if: %s\n", iface->config.name);
    struct in_addr addr;
    addr.s_addr = inet_addr(paIPAddr);
    struct net_if_mcast_addr* maddr = net_if_ipv4_maddr_lookup(&addr, &iface);
    if (!maddr) {
      maddr = net_if_ipv4_maddr_add(iface, &addr);
      if (!maddr) {
        DEVLOG_WARNING("CBSDSocketInterface: net_if_ipv4_maddr_add failed\n");
      }
    }
    if (maddr && !net_if_ipv4_maddr_is_joined(maddr)) {
      net_if_ipv4_maddr_join(iface, maddr);
    }
  } else {
    DEVLOG_WARNING("CBSDSocketInterface: net_if_get_default() failed\n");
  }
}

