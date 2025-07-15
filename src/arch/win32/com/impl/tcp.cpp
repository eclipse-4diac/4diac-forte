/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "tcp.h"
#include "net.h"

#include "core/stringdict.h"
#include "core/com/factory.h"

USE_STRING_ID(tcp)

using namespace forte::com;
using namespace forte::com::impl;

namespace {
  [[maybe_unused]] ComChannelEntryImpl<ComBuffer, TCPChannel> entry(STRID(tcp));
}

SOCKET TCPChannel::socket(const std::string_view paConfigString) {
  ADDRINFOEXA hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  return net::open(paConfigString, hints);
}
