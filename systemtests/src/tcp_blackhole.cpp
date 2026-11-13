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
 ******************************************************************************/

#include <cerrno>
#include <chrono>
#include <fstream>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

#include "../../core/arch/posix/src/com/impl/socket.h"
#include "../../core/arch/posix/src/com/impl/net.h"
#include "forte/com/buffer.h"
#include "forte/com/factory.h"
#include "forte/stringid.h"
#include "forte/util/devlog.h"

using namespace forte::literals;

namespace forte::com::impl::systemtests {
  namespace {
    bool tcpAbortOnOverflowIsDisabled() {
      std::ifstream sysctlFile("/proc/sys/net/ipv4/tcp_abort_on_overflow");
      int setting{};
      if (!(sysctlFile >> setting)) {
        DEVLOG_ERROR(
            "tcp_blackhole requires tcp_abort_on_overflow=0 (cannot read /proc/sys/net/ipv4/tcp_abort_on_overflow)\n");
        return false;
      }
      if (setting != 0) {
        DEVLOG_ERROR("tcp_blackhole requires tcp_abort_on_overflow=0 (found %d)\n", setting);
        return false;
      }
      return true;
    }

    bool fillBacklog(const sockaddr_in &paAddress, std::vector<Socket> &paQueuedClients, Socket &paPendingProbe) {
      for (std::size_t attemptNumber = 0; attemptNumber < 16 && !paPendingProbe; ++attemptNumber) {
        Socket attempt(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        if (!attempt || attempt.setNonBlocking() != 0) {
          DEVLOG_ERROR("tcp_blackhole failed to create a nonblocking backlog probe socket\n");
          return false;
        }

        if (::connect(attempt.get(), reinterpret_cast<const sockaddr *>(&paAddress), sizeof(paAddress)) == 0) {
          paQueuedClients.emplace_back(std::move(attempt));
          continue;
        }
        if (errno != EINPROGRESS) {
          continue;
        }

        pollfd descriptor{attempt.get(), POLLOUT, 0};
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
        int pollResult{};
        do {
          const auto remaining =
              std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now())
                  .count();
          pollResult = ::poll(&descriptor, 1, static_cast<int>(remaining > 0 ? remaining : 0));
        } while (pollResult < 0 && errno == EINTR && std::chrono::steady_clock::now() < deadline);

        if (pollResult == 0) {
          paPendingProbe = std::move(attempt);
          continue;
        }
        if (pollResult < 0) {
          DEVLOG_ERROR("tcp_blackhole failed while polling a backlog probe socket\n");
          return false;
        }

        int socketError{};
        socklen_t errorSize = sizeof(socketError);
        if (getsockopt(attempt.get(), SOL_SOCKET, SO_ERROR, &socketError, &errorSize) != 0) {
          DEVLOG_ERROR("tcp_blackhole failed to inspect a backlog probe socket\n");
          return false;
        }
        if (socketError == 0) {
          paQueuedClients.emplace_back(std::move(attempt));
        }
      }

      if (paQueuedClients.empty() || !paPendingProbe) {
        DEVLOG_ERROR("tcp_blackhole could not saturate the TCP listen backlog; tcp_abort_on_overflow must be 0\n");
        return false;
      }
      return true;
    }

    class TCPBlackholeChannel final : public SocketChannel {
      public:
        explicit TCPBlackholeChannel(ComObserver<ComBuffer> &paObserver) : SocketChannel(paObserver) {
        }

        ComResult open(const std::string_view paConfigString,
                       const std::span<const ComChannelDescriptor> paDescriptors) override {
          const ComResult result = SocketChannel::open(paConfigString, paDescriptors);
          if (result != ComResult::Ok && !isOpen()) {
            mPendingProbe.reset();
            mQueuedClients.clear();
          }
          return result;
        }

        ComResult send(ComBuffer) override {
          return isOpen() ? ComResult::WouldBlock : ComResult::NoSocket;
        }

        ComResult poll() override {
          return isOpen() ? ComResult::WouldBlock : ComResult::NoSocket;
        }

        ComResult close() override {
          mPendingProbe.reset();
          mQueuedClients.clear();
          return isOpen() ? SocketChannel::close() : ComResult::Ok;
        }

        [[nodiscard]] bool supports(const std::span<const ComChannelDescriptor> paDescriptors) const override {
          return supportsChannel("tcp_blackhole"_STRID, paDescriptors);
        }

      protected:
        ComResultValue<Socket> socket(const std::string_view paConfigString) override {
          if (!tcpAbortOnOverflowIsDisabled()) {
            return {.mResult = ComResult::Unknown};
          }

          const std::size_t separator = paConfigString.rfind(':');
          if (separator == std::string_view::npos || separator == 0 || separator + 1 == paConfigString.size()) {
            DEVLOG_ERROR("tcp_blackhole requires an IPv4 address and port\n");
            return {.mResult = ComResult::InvalidId};
          }

          addrinfo hints{};
          hints.ai_family = AF_INET;
          hints.ai_socktype = SOCK_STREAM;
          hints.ai_protocol = IPPROTO_TCP;
          auto [resolveResult, addressesRaw] = net::resolve(paConfigString, hints);
          if (resolveResult != ComResult::Ok) {
            DEVLOG_ERROR("tcp_blackhole could not resolve address (result %d)\n", static_cast<int>(resolveResult));
            return {.mResult = ComResult::InvalidId};
          }
          std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> addresses(addressesRaw, freeaddrinfo);

          Socket listener(::socket(addresses->ai_family, addresses->ai_socktype, addresses->ai_protocol));
          if (!listener) {
            DEVLOG_ERROR("tcp_blackhole failed to create its listen socket\n");
            return {.mResult = ComResult::Unknown};
          }
          constexpr int reuseAddress = 1;
          if (setsockopt(listener.get(), SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) != 0 ||
              ::bind(listener.get(), addresses->ai_addr, addresses->ai_addrlen) != 0 ||
              ::listen(listener.get(), 1) != 0) {
            DEVLOG_ERROR("tcp_blackhole failed to bind or listen on the configured address\n");
            return {.mResult = ComResult::Unknown};
          }

          sockaddr_in boundAddress{};
          socklen_t boundAddressLength = sizeof(boundAddress);
          if (getsockname(listener.get(), reinterpret_cast<sockaddr *>(&boundAddress), &boundAddressLength) != 0) {
            DEVLOG_ERROR("tcp_blackhole failed to inspect its listen socket\n");
            return {.mResult = ComResult::Unknown};
          }

          std::vector<Socket> queuedClients;
          Socket pendingProbe;
          if (!fillBacklog(boundAddress, queuedClients, pendingProbe)) {
            return {.mResult = ComResult::Unknown};
          }

          mQueuedClients = std::move(queuedClients);
          mPendingProbe = std::move(pendingProbe);
          return {.mValue = std::move(listener)};
        }

      private:
        std::vector<Socket> mQueuedClients;
        Socket mPendingProbe;
    };

    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TCPBlackholeChannel> entry("tcp_blackhole"_STRID);
  } // namespace
} // namespace forte::com::impl::systemtests
