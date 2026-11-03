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

      ~TCPListenChannel() override;

      ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) override;
      ComResult send(ComBuffer paData) override;
      ComResult close() override;

    protected:
      SOCKET socket(std::string_view paConfigString) override;
      std::size_t recv() override;

    private:
      SOCKET mConnectionSocket = INVALID_SOCKET;
  };

} // namespace forte::com::impl
