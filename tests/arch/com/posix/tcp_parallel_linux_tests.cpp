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

#include <boost/test/unit_test.hpp>

#include <cerrno>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "forte/com/factory.h"
#include "forte_boost_output_support.h"
#include "com/impl/net.h"
#include "com/impl/tcp_parallel.h"
#include "socket_test_helpers.h"

namespace forte::com::test {
  namespace {
    class TestTCPParallelChannel final : public impl::TCPParallelChannel {
      public:
        using impl::TCPParallelChannel::getConnectionState;

        explicit TestTCPParallelChannel(ComObserver<ComBuffer> &paObserver) : TCPParallelChannel(paObserver) {
        }
    };

    struct BacklogFill {
        std::vector<Socket> mQueuedClients;
        Socket mPendingProbe;
    };

    struct TCPParallelLinuxFixture {
        TCPParallelLinuxFixture() :
            mListener(createLoopbackListener(mAddress)),
            mConfig("127.0.0.1:" + std::to_string(ntohs(mAddress.sin_port))) {
          BOOST_TEST_REQUIRE(static_cast<bool>(mListener));
        }

        BufferObserver mObserver;
        sockaddr_in mAddress{};
        Socket mListener;
        std::string mConfig;

        static bool tcpAbortOnOverflowIsDisabled() {
          std::ifstream sysctlFile("/proc/sys/net/ipv4/tcp_abort_on_overflow");
          int setting{};
          if (!(sysctlFile >> setting)) {
            BOOST_TEST_MESSAGE("tcp_blackhole requires tcp_abort_on_overflow=0 (cannot read "
                               "/proc/sys/net/ipv4/tcp_abort_on_overflow)");
            return false;
          }
          if (setting != 0) {
            BOOST_TEST_MESSAGE("tcp_blackhole requires tcp_abort_on_overflow=0 (found " << setting << ")");
            return false;
          }
          return true;
        }

        BacklogFill fillBacklogUntilPending() const {
          BacklogFill backlog;
          for (std::size_t attemptNumber = 0; attemptNumber < 16 && !backlog.mPendingProbe; ++attemptNumber) {
            Socket attempt(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
            BOOST_TEST_REQUIRE(static_cast<bool>(attempt));
            BOOST_TEST_REQUIRE(attempt.setNonBlocking() == 0);
            BOOST_TEST_REQUIRE(attempt.setNoSigPipe() == 0);

            if (::connect(attempt.get(), reinterpret_cast<const sockaddr *>(&mAddress), sizeof(mAddress)) == 0) {
              backlog.mQueuedClients.emplace_back(std::move(attempt));
              continue;
            }
            if (errno != EINPROGRESS) {
              continue;
            }

            if (!waitForSocketEvent(attempt, SocketConnectCompletionEvent,
                                    Clock::now() + std::chrono::milliseconds(250))) {
              backlog.mPendingProbe = std::move(attempt);
              continue;
            }

            int socketError = 0;
            socklen_t errorSize = sizeof(socketError);
            if (getsockopt(attempt.get(), SOL_SOCKET, SO_ERROR, &socketError, &errorSize) == 0 && socketError == 0) {
              backlog.mQueuedClients.emplace_back(std::move(attempt));
            }
          }
          BOOST_TEST_REQUIRE(!backlog.mQueuedClients.empty());
          BOOST_TEST_REQUIRE(static_cast<bool>(backlog.mPendingProbe));
          return backlog;
        }

        void acceptAvailable(const sockaddr_in &paChannelAddress,
                             std::vector<Socket> &paAcceptedPeers,
                             Socket &paChannelPeer) const {
          for (;;) {
            sockaddr_in peerAddress{};
            socklen_t peerAddressLength = sizeof(peerAddress);
            Socket accepted(::accept(mListener.get(), reinterpret_cast<sockaddr *>(&peerAddress), &peerAddressLength));
            if (!accepted) {
              if (errno == EINTR) {
                continue;
              }
              BOOST_TEST((errno == EAGAIN || errno == EWOULDBLOCK));
              break;
            }
            if (peerAddress.sin_port == paChannelAddress.sin_port) {
              paChannelPeer = std::move(accepted);
            } else {
              paAcceptedPeers.emplace_back(std::move(accepted));
            }
          }
        }

        static std::uint64_t socketCookie(const Socket &paSocket) {
          std::uint64_t cookie{};
          socklen_t cookieLength = sizeof(cookie);
          BOOST_TEST_REQUIRE(getsockopt(paSocket.get(), SOL_SOCKET, SO_COOKIE, &cookie, &cookieLength) == 0);
          return cookie;
        }

        ComResult waitForChannelCompletion(TestTCPParallelChannel &paChannel,
                                           const sockaddr_in &paChannelAddress,
                                           std::vector<Socket> &paAcceptedPeers,
                                           Socket &paChannelPeer) const {
          const auto completionDeadline = Clock::now() + std::chrono::seconds(12);
          auto result = ComResult::InProgress;
          while (result == ComResult::InProgress && Clock::now() < completionDeadline) {
            if (waitForSocketEvent(mListener, SocketReadEvent, Clock::now() + std::chrono::milliseconds(100))) {
              acceptAvailable(paChannelAddress, paAcceptedPeers, paChannelPeer);
            }
            result = paChannel.open(mConfig, std::span<ComChannelDescriptor>{});
          }
          return result;
        }

        void waitForChannelPeer(const sockaddr_in &paChannelAddress,
                                std::vector<Socket> &paAcceptedPeers,
                                Socket &paChannelPeer) const {
          const auto peerDeadline = Clock::now() + std::chrono::seconds(12);
          while (!paChannelPeer && Clock::now() < peerDeadline) {
            if (waitForSocketEvent(mListener, SocketReadEvent, peerDeadline)) {
              acceptAvailable(paChannelAddress, paAcceptedPeers, paChannelPeer);
            }
          }
        }
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(TCP_PARALLEL_CHANNEL_LINUX, TCPParallelLinuxFixture)

  /**
   * \brief  This test saturates the backlog of a real TCP listen socket to emulate a pending connection.
   */

  BOOST_AUTO_TEST_CASE(BACKLOG_SATURATION_REUSES_PENDING_CONNECTION,
                       *boost::unit_test::precondition([](boost::unit_test::test_unit_id) {
                         return TCPParallelLinuxFixture::tcpAbortOnOverflowIsDisabled();
                       })) {
    auto [queuedClients, pendingProbe] = fillBacklogUntilPending();

    TestTCPParallelChannel channel(mObserver);
    const ComResult firstResult = channel.open(mConfig, std::span<ComChannelDescriptor>{});
    BOOST_TEST_REQUIRE(firstResult == ComResult::InProgress);
    auto &state = channel.getConnectionState();
    BOOST_TEST_REQUIRE(state.candidateCount() == 1U);
    BOOST_TEST_REQUIRE(state.hasPendingCandidates());
    const std::uint64_t candidateCookie = socketCookie(state.candidateAt(0));

    const ComResult secondResult = channel.open(mConfig, std::span<ComChannelDescriptor>{});
    BOOST_TEST_REQUIRE(secondResult == ComResult::InProgress);
    BOOST_TEST_REQUIRE(state.candidateCount() == 1U);
    const std::uint64_t retriedCandidateCookie = socketCookie(state.candidateAt(0));
    BOOST_TEST(retriedCandidateCookie == candidateCookie);

    sockaddr_in channelAddress{};
    socklen_t channelAddressLength = sizeof(channelAddress);
    BOOST_TEST_REQUIRE(getsockname(state.candidateAt(0).get(), reinterpret_cast<sockaddr *>(&channelAddress),
                                   &channelAddressLength) == 0);

    std::vector<Socket> acceptedPeers;
    Socket channelPeer;

    BOOST_TEST(pendingProbe.close() == 0);
    BOOST_TEST_REQUIRE(waitForChannelCompletion(channel, channelAddress, acceptedPeers, channelPeer) == ComResult::Ok);

    waitForChannelPeer(channelAddress, acceptedPeers, channelPeer);
    BOOST_TEST_REQUIRE(static_cast<bool>(channelPeer));

    std::string request{"ping"};
    std::string received;
    BOOST_TEST_REQUIRE(channel.send(ComBuffer(request)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(channelPeer, request.size(), received));
    BOOST_TEST(received == request);

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(PENDING_FIRST_ADDRESS_DOES_NOT_BLOCK_REACHABLE_SECOND,
                       *boost::unit_test::precondition([](boost::unit_test::test_unit_id) {
                         return TCPParallelLinuxFixture::tcpAbortOnOverflowIsDisabled();
                       })) {
    auto [queuedClients, pendingProbe] = fillBacklogUntilPending();

    sockaddr_in secondAddress{};
    const Socket secondListener = createLoopbackListener(secondAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(secondListener));

    AddressInfo firstInfo = makeAddressInfo(mAddress);
    AddressInfo secondInfo = makeAddressInfo(secondAddress);
    firstInfo.ai_next = &secondInfo;

    auto state = impl::net::openParallel(&firstInfo, false);
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);
    const NativeSocket pendingHandle = state.candidateAt(0).get();
    const NativeSocket successHandle = state.candidateAt(1).get();
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(0)));
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(1)));

    BOOST_TEST_REQUIRE(
        waitForSocketEvent(successHandle, SocketConnectCompletionEvent, Clock::now() + gcReadinessTimeout));
    pollfd pendingDescriptor{.fd = pendingHandle, .events = SocketConnectCompletionEvent, .revents = 0};
    BOOST_TEST(::poll(&pendingDescriptor, 1, 0) == 0);

    auto [result, winner] = impl::net::pollParallel(state);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST_REQUIRE(static_cast<bool>(winner));
    BOOST_TEST(winner.get() == successHandle);
    BOOST_TEST(nativeSocketIsClosed(pendingHandle));

    BOOST_TEST_REQUIRE(waitForSocketEvent(secondListener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted = acceptLoopbackConnection(secondListener);
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    constexpr std::string_view request = "ping";
    std::string received;
    BOOST_TEST_REQUIRE(sendBytes(winner, request));
    BOOST_TEST_REQUIRE(receiveBytes(accepted, request.size(), received));
    BOOST_TEST(received == request);

    constexpr std::string_view response = "pong";
    received.clear();
    BOOST_TEST_REQUIRE(sendBytes(accepted, response));
    BOOST_TEST_REQUIRE(receiveBytes(winner, response.size(), received));
    BOOST_TEST(received == response);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
