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

namespace forte::com::impl {

  class TCPListenChannel final : public SocketChannel {
    public:
      explicit TCPListenChannel(ComObserver<ComBuffer> &paObserver) : SocketChannel(paObserver) {
      }

      SOCKET socket(std::string_view paConfigString) override;
      ComResult send(ComBuffer paData) override;
      std::size_t recv() override;

    private:
      SOCKET mConnectionSocket = INVALID_SOCKET;
  };

} // namespace forte::com::impl
