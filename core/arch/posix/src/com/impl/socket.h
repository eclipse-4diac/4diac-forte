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

#include <utility>
#include <sys/types.h>
#include <sys/socket.h>

#include "forte/com/buffer.h"
#include "forte/com/channel.h"

namespace forte::com::impl {

#ifdef MSG_NOSIGNAL
  inline constexpr int SocketSendFlags = MSG_NOSIGNAL;
#else
  inline constexpr int SocketSendFlags = 0;
#endif

  class Socket {
    public:
      Socket() noexcept = default;
      explicit Socket(const int paSocket) noexcept : mSocket(paSocket) {
      }

      ~Socket() {
        reset();
      }

      Socket(const Socket &) = delete;
      Socket &operator=(const Socket &) = delete;

      Socket(Socket &&paOther) noexcept : mSocket(std::exchange(paOther.mSocket, -1)) {
      }

      Socket &operator=(Socket &&paOther) noexcept {
        if (this != &paOther) {
          reset(std::exchange(paOther.mSocket, -1));
        }
        return *this;
      }

      [[nodiscard]] explicit operator bool() const noexcept {
        return mSocket >= 0;
      }

      [[nodiscard]] int get() const noexcept {
        return mSocket;
      }

      [[nodiscard]] int release() noexcept {
        return std::exchange(mSocket, -1);
      }

      int close() noexcept;
      int setNoSigPipe() const;
      int setNonBlocking() const;
      void reset(int paSocket = -1) noexcept;

    private:
      int mSocket{-1};
  };

  class SocketChannel : public ComChannel<ComBuffer> {
    public:
      ComResult open(std::string_view paConfigString, std::span<const ComChannelDescriptor> paDescriptors) override;
      ComResult send(ComBuffer paData) override;
      ComResult poll() override;
      ComResult close() override;

      [[nodiscard]] bool isOpen() const override {
        return static_cast<bool>(mSocket);
      }

      ComResult setMaxReceiveSize(std::size_t paSize) override;

      ~SocketChannel() override;

    protected:
      explicit SocketChannel(ComObserver<ComBuffer> &paObserver) : ComChannel(paObserver) {
      }

      virtual ComResultValue<Socket> socket(std::string_view paConfigString) = 0;
      virtual ComResultValue<std::size_t> recv();

      [[nodiscard]] const Socket &getSocket() const {
        return mSocket;
      }

      ComResult setSocket(Socket paSocket);

      ComBuffer &getBuffer() {
        return mBuffer;
      }

    private:
      Socket mSocket;
      ComBuffer mBuffer;
  };

} // namespace forte::com::impl
