/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "forte_boost_output_support.h"

#include "../../core/src/cominfra/ipcomlayer.h"
#include "forte/cominfra/commfb.h"
#include "forte/datatypes/forte_string.h"
#include "../../../stdfblib/system/src/EMB_RES.h"

using namespace forte::literals;

namespace forte::com_infra::test {
  namespace {
    iec61499::system::EMB_RES resource({}, resource);

    const auto scmDataInputTypes = std::array{"BOOL"_STRID, "WSTRING"_STRID};
    const auto scmDataOutputTypes = std::array{"BOOL"_STRID, "WSTRING"_STRID, "STRING"_STRID};

    // a client comm FB that is never executed: the test itself plays the role of the event chain
    class CIPLayerTestCommFB : public CCommFB {
      public:
        CIPLayerTestCommFB() : CCommFB({}, resource, e_Client) {
          SFBInterfaceSpec &interface(getGenInterfaceSpec());
          interface.mEINames = {};
          interface.mEONames = {};
          interface.mDINames = scmDataInputTypes;
          interface.mDONames = scmDataOutputTypes;
          setupFBInterface();
        }

      protected:
        void createGenOutputData() override {
          mGenDOs = std::unique_ptr<CIEC_ANY *[]>(new CIEC_ANY *[] { new CIEC_STRING() });
        }

        size_t getGenEOOffset() override {
          return 0;
        }
    };

    // the layer above the IP layer, records what the IP layer hands up
    class CRecordingLayer : public CComLayer {
      public:
        explicit CRecordingLayer(CBaseCommFB *paComFB) : CComLayer(nullptr, paComFB) {
        }

        EComResponse sendData(void *, unsigned int) override {
          return e_ProcessDataOk;
        }

        EComResponse recvData(const void *paData, unsigned int paSize) override {
          const auto *data = static_cast<const char *>(paData);
          mReceived.append(data, paSize);
          mChunkSizes.push_back(paSize);
          return e_ProcessDataOk;
        }

        std::string mReceived;
        std::vector<unsigned int> mChunkSizes;

      private:
        EComResponse openConnection(char *) override {
          return e_InitOk;
        }

        void closeConnection() override {
        }
    };

    // an IP layer connected to one end of a socket pair, without the socket handler thread
    class CConnectedIPComLayer : public CIPComLayer {
      public:
        CConnectedIPComLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB, int paSocket) :
            CIPComLayer(paUpperLayer, paComFB) {
          mSocketID = paSocket;
          mConnectionState = e_Connected;
        }

        ~CConnectedIPComLayer() override {
          // the socket pair belongs to the test and is not registered with the socket handler, so neither the top
          // layer nor closeConnection() may close it
          mSocketID = arch::CIPComSocketHandler::scmInvalidSocketDescriptor;
          mTopLayer->setBottomLayer(nullptr);
        }

        // what the socket handler does when select reports the socket
        EComResponse socketReadable() {
          return recvData(&mSocketID, 0);
        }
    };

    int pendingBytes(int paSocket) {
      int pending = 0;
      BOOST_REQUIRE_EQUAL(0, ioctl(paSocket, FIONREAD, &pending));
      return pending;
    }

    struct SocketPair {
        SocketPair() {
          BOOST_REQUIRE_EQUAL(0, socketpair(AF_UNIX, SOCK_STREAM, 0, mFds.data()));
        }
        ~SocketPair() {
          close(mFds[0]);
          close(mFds[1]);
        }
        std::array<int, 2> mFds{};
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(IPComLayer_test)

  // A frame larger than the receive buffer fills the buffer while the rest of the frame waits in the socket. The
  // socket handler reports the socket again before the event chain has emptied the buffer. The layer must leave the
  // data in the socket and keep the received data for the event chain, instead of waiting for the buffer to empty.
  BOOST_AUTO_TEST_CASE(frameLargerThanReceiveBuffer) {
    constexpr unsigned int bufferSize = cgIPLayerRecvBufferSize;
    const unsigned int frameSize = 2 * bufferSize + 100;
    std::string frame(frameSize, '\0');
    for (unsigned int i = 0; i < frameSize; ++i) {
      frame[i] = static_cast<char>(i % 251);
    }

    SocketPair sockets;
    const int peer = sockets.mFds[1];
    int sendBufferSize = static_cast<int>(2 * frameSize);
    setsockopt(peer, SOL_SOCKET, SO_SNDBUF, &sendBufferSize, sizeof(sendBufferSize));
    // MSG_DONTWAIT so that a too small socket buffer fails the test instead of blocking it
    BOOST_REQUIRE_EQUAL(static_cast<ssize_t>(frameSize), send(peer, frame.data(), frameSize, MSG_DONTWAIT));

    CIPLayerTestCommFB commFB;
    CRecordingLayer topLayer(&commFB);
    CConnectedIPComLayer ipLayer(&topLayer, &commFB, sockets.mFds[0]);

    // fill the receive buffer
    for (int i = 0; i < 10 && pendingBytes(sockets.mFds[0]) > static_cast<int>(frameSize - bufferSize); ++i) {
      BOOST_TEST(e_ProcessDataOk == ipLayer.socketReadable());
    }
    BOOST_REQUIRE_EQUAL(static_cast<int>(frameSize - bufferSize), pendingBytes(sockets.mFds[0]));

    // the socket is reported again while the buffer is still full: nothing is read and nothing is lost
    BOOST_TEST(e_Nothing == ipLayer.socketReadable());
    BOOST_TEST(static_cast<int>(frameSize - bufferSize) == pendingBytes(sockets.mFds[0]));
    BOOST_TEST(topLayer.mChunkSizes.empty());

    // the event chain empties the buffer
    BOOST_TEST(e_ProcessDataOk == ipLayer.processInterrupt());
    BOOST_REQUIRE_EQUAL(1U, topLayer.mChunkSizes.size());
    BOOST_TEST(bufferSize == topLayer.mChunkSizes[0]);

    // a second interrupt for the same data does not hand it up again
    BOOST_TEST(e_Nothing == ipLayer.processInterrupt());
    BOOST_TEST(1U == topLayer.mChunkSizes.size());

    // the rest of the frame follows
    for (int i = 0; i < 10 && pendingBytes(sockets.mFds[0]) > 0; ++i) {
      BOOST_TEST(e_ProcessDataOk == ipLayer.socketReadable());
      BOOST_TEST(e_ProcessDataOk == ipLayer.processInterrupt());
    }
    BOOST_TEST(0 == pendingBytes(sockets.mFds[0]));
    BOOST_TEST(frameSize == topLayer.mReceived.size());
    BOOST_TEST((frame == topLayer.mReceived));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com_infra::test
