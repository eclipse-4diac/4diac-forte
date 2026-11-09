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

#include "socket.h"

#include "../../../../common/src/com/impl/parallel_state.h"

#include <string_view>
#include <netdb.h>

namespace forte::com::impl::net {

  ComResult mapSocketError(int paError, ComResult paFallback) noexcept;
  ComResult mapResolverError(int paResolverError, int paSystemError, ComResult paFallback) noexcept;

  ComResultValue<addrinfo *> resolve(std::string_view paConfigString, const addrinfo &paHints);

  ComResultValue<Socket> open(const addrinfo *paAddresses, bool paPassive);

  ComResultValue<Socket> open(std::string_view paConfigString, const addrinfo &paHints);

  ParallelConnectionState<Socket> openParallel(const addrinfo *paAddresses, bool paPassive);

  ComResultValue<ParallelConnectionState<Socket>> openParallel(std::string_view paConfigString,
                                                               const addrinfo &paHints);

  ComResultValue<Socket> pollParallel(ParallelConnectionState<Socket> &paState);

} // namespace forte::com::impl::net
