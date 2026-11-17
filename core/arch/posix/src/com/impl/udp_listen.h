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

  class UDPListenChannel : public SocketChannel {
    public:
      explicit UDPListenChannel(ComObserver<ComBuffer> &paObserver) : SocketChannel(paObserver) {
      }

      [[nodiscard]] bool supports(std::span<const ComChannelDescriptor> paDescriptors) const override;
      ComResult close() override;

    protected:
      ComResultValue<Socket> socket(std::string_view paConfigString) override;
      ComResultValue<std::size_t> recv() override;

    private:
      bool mConnected = false;
  };

} // namespace forte::com::impl
