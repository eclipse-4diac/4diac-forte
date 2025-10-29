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

#pragma once

#include <string_view>
#include <ws2tcpip.h>

namespace forte::com::impl::net {

  SOCKET open(std::string_view paConfigString, const ADDRINFOA &paHints);

  int setNonBlocking(SOCKET paSocket);

} // namespace forte::com::impl::net
