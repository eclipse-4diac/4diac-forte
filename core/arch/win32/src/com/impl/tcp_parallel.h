/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
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

#include "net.h"
#include "tcp.h"
#include "../../../../common/src/com/impl/parallel_state.h"

namespace forte::com::impl {

  /**
   * \brief A com channel for TCP connections that supports non-blocking, parallel connection attempts.
   * \note The address resolution is still performed synchronously, so time-sensitive applications should use numeric
   * addresses, a local hosts file, or a local resolver.
   */
  class TCPParallelChannel : public TCPChannel {
    public:
      explicit TCPParallelChannel(ComObserver<ComBuffer> &paObserver) : TCPChannel(paObserver) {
      }

      [[nodiscard]] bool supports(std::span<const ComChannelDescriptor> paDescriptors) const override;

      ComResult close() override;

    protected:
      [[nodiscard]] ParallelConnectionState<Socket> &getConnectionState() {
        return mConnectionState;
      }

      ComResultValue<Socket> socket(std::string_view paConfigString) override;

    private:
      ParallelConnectionState<Socket> mConnectionState;
  };

} // namespace forte::com::impl
