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

#include <winsock2.h>

#include "forte/com/buffer.h"
#include "forte/com/channel.h"

namespace forte::com::impl {

  class SocketChannel : public ComChannel<ComBuffer> {
    public:
      ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) override;
      ComResult send(ComBuffer paData) override;
      ComResult poll() override;
      ComResult close() override;

      ComResult setMaxReceiveSize(std::size_t paSize) override;

      ~SocketChannel() override;

    protected:
      explicit SocketChannel(ComObserver<ComBuffer> &paObserver) : ComChannel(paObserver), mSocket(INVALID_SOCKET) {
      }

      virtual SOCKET socket(std::string_view paConfigString) = 0;
      virtual std::size_t recv();

      [[nodiscard]] SOCKET getSocket() const {
        return mSocket;
      }

      ComBuffer &getBuffer() {
        return mBuffer;
      }

    private:
      SOCKET mSocket;
      ComBuffer mBuffer;
  };

} // namespace forte::com::impl
