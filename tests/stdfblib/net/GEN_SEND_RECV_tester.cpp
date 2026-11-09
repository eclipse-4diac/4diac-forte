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

#include <boost/test/unit_test.hpp>

#include <array>
#include <string>
#include <string_view>

#include "../../core/fbtests/fbtestfixture.h"
#include "../../core/com/channeltestfixtures.h"
#include "forte_boost_output_support.h"
#include "forte/iec61499/net/GEN_SEND_RECV_fbt.h"

using namespace forte::literals;

namespace forte::com::test {
  namespace {
    struct GEN_SEND_RECV_TestFixture : forte::test::CFBTestFixtureBase {
        static constexpr TEventID scmEventINITID = iec61499::net::GEN_SEND_RECV::scmEventINITID;
        static constexpr TEventID scmEventINITOID = iec61499::net::GEN_SEND_RECV::scmEventINITOID;
        static constexpr TEventID scmEventREQID = iec61499::net::GEN_SEND_RECV::scmEventREQID;
        static constexpr TEventID scmEventCNFID = iec61499::net::GEN_SEND_RECV::scmEventCNFID;
        static constexpr TEventID scmEventRSPID = iec61499::net::GEN_SEND_RECV::scmEventRSPID;
        static constexpr TEventID IND = iec61499::net::GEN_SEND_RECV::scmEventINDID;

        GEN_SEND_RECV_TestFixture() : CFBTestFixtureBase("iec61499::net::SEND_RECV_1_1"_STRID) {
          TestChannel::reset();
          TestBufferChannel::reset();
          setInputData({&mQI, &mID, &mSD1});
          setOutputData({&mQO, &mSTATUS, &mRD1});
          setup();
        }

        void initChannel(const std::string_view paId, const ComResult paResult) {
          mQI = true_BOOL;
          mID.assign(paId.data(), paId.size());
          triggerEvent(scmEventINITID);
          expectInitResult(paResult);
        }

        void closeChannel() {
          mQI = false_BOOL;
          triggerEvent(scmEventINITID);
          expectInitResult(ComResult::Terminated);
        }

        void expectStatus(const ComResult paResult) {
          BOOST_TEST(mQO == (paResult == ComResult::Ok));
          BOOST_TEST(mSTATUS.getValue() == getComResultString(paResult).data());
        }

        void expectInitResult(const ComResult paResult) {
          BOOST_TEST(checkForSingleOutputEventOccurence(scmEventINITOID));
          expectStatus(paResult);
        }

        CIEC_BOOL mQI;
        CIEC_WSTRING mID;
        CIEC_ANY_VARIANT mSD1;
        CIEC_BOOL mQO;
        CIEC_WSTRING mSTATUS;
        CIEC_ANY_VARIANT mRD1;
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(GEN_SEND_RECV_Tests)

  BOOST_FIXTURE_TEST_CASE(INIT, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::Ok;

    initChannel("test-channel[root]", ComResult::Ok);

    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smLastMinReceiveSize == 1);
    BOOST_TEST(TestChannel::smLastMaxReceiveSize == 1);

    closeChannel();

    BOOST_TEST(TestChannel::smCloseCalls == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
  }

  BOOST_FIXTURE_TEST_CASE(RECEIVE_SIZE_FAILURE_PREVENTS_OPEN, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::Ok;
    TestChannel::smSetMaxReceiveSizeResult = ComResult::Overflow;

    initChannel("test-channel[root]", ComResult::Overflow);

    BOOST_TEST(TestChannel::smOpenCalls == 0);
    BOOST_TEST(TestChannel::smCloseCalls == 0);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);

    closeChannel();

    BOOST_TEST(TestChannel::smCloseCalls == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
  }

  BOOST_FIXTURE_TEST_CASE(OPEN_IN_PROGRESS_REUSE, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::InProgress;
    initChannel("test-channel[root]", ComResult::InProgress);
    BOOST_REQUIRE(TestChannel::smLastChannel != nullptr);
    const int identity = TestChannel::smLastChannel->identity();

    initChannel("test-channel[changed]", ComResult::InProgress);
    BOOST_TEST(TestChannel::smLastChannel->identity() == identity);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "changed");
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smOpenCalls == 2);

    initChannel("", ComResult::InvalidId);
    BOOST_REQUIRE(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 2);

    initChannel("test-channel[after-invalid]", ComResult::InProgress);
    BOOST_TEST(TestChannel::smLastChannel->identity() == identity);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "after-invalid");
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smOpenCalls == 3);

    closeChannel();
    BOOST_TEST(TestChannel::smDestroyed == 0);
  }

  BOOST_FIXTURE_TEST_CASE(REPEATED_OPEN_IGNORES_CHANGED_ID_UNTIL_CLOSED, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::Ok;
    initChannel("test-channel[root]", ComResult::Ok);
    const int identity = TestChannel::smLastChannel->identity();

    initChannel("test-channel[changed]", ComResult::Ok);
    initChannel("missing-channel[config]", ComResult::Ok);
    BOOST_TEST(TestChannel::smLastChannel->identity() == identity);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "root");
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smCloseCalls == 0);

    closeChannel();
    TestChannel::smNextOpenResult = ComResult::Ok;
    initChannel("test-alternate-channel[fresh]", ComResult::Ok);
    BOOST_TEST(TestChannel::smLastChannel->identity() != identity);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "fresh");
    BOOST_TEST(TestChannel::smCreated == 2);
    BOOST_TEST(TestChannel::smDestroyed == 1);
    BOOST_TEST(TestChannel::smOpenCalls == 2);
    BOOST_TEST(TestChannel::smCloseCalls == 1);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(OPEN_FAILURE_RETAINS_CHANNEL_UNTIL_CLOSED, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::InProgress;
    initChannel("test-channel[root]", ComResult::InProgress);

    TestChannel::smNextOpenResult = ComResult::Overflow;
    initChannel("test-alternate-channel[fresh]", ComResult::Overflow);
    BOOST_TEST(TestChannel::smCreated == 2);
    BOOST_TEST(TestChannel::smDestroyed == 1);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "fresh");

    TestChannel::smNextOpenResult = ComResult::InProgress;
    initChannel("test-alternate-channel[changed]", ComResult::InProgress);
    BOOST_TEST(TestChannel::smCreated == 2);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "changed");

    closeChannel();
    BOOST_TEST(TestChannel::smCloseCalls == 1);
    BOOST_TEST(TestChannel::smDestroyed == 1);
  }

  BOOST_FIXTURE_TEST_CASE(FACTORY_FAILURE_DISCARDS_CHANNELS, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextOpenResult = ComResult::Ok;
    initChannel("test-channel[root]", ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);

    closeChannel();
    initChannel("missing-channel[config]", ComResult::InvalidId);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 1);

    initChannel("endian[big].missing-channel[child]", ComResult::InvalidId);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(REUSE_CHANNEL_CHAIN, GEN_SEND_RECV_TestFixture) {
    TestBufferChannel::smNextOpenResult = ComResult::Overflow;
    const std::string id = "raw[4].test-buffer-channel[child]";

    initChannel(id, ComResult::Overflow);
    initChannel(id, ComResult::Overflow);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 2);

    initChannel("raw[8].test-buffer-channel[changed]", ComResult::Overflow);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 3);
    BOOST_TEST(TestBufferChannel::smLastChannel->configString() == "changed");
    BOOST_TEST(TestBufferChannel::smLastMaxReceiveSize == 8);
    BOOST_TEST(TestBufferChannel::smLastMinReceiveSize == 8);

    closeChannel();
    initChannel("raw[8].test-buffer-channel[changed]", ComResult::Overflow);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smLastChannel->configString() == "changed");
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(SYNCHRONOUS_SEND_SUCCESS, GEN_SEND_RECV_TestFixture) {
    initChannel("test-channel[root]", ComResult::Ok);
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::Ok);
    BOOST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(TestChannel::smLastSentValues[0].unwrap())) ==
               0x1234);
    BOOST_TEST(TestChannel::smSendCalls == 1);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(SYNCHRONOUS_SEND_ERROR, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextSendResult = ComResult::Overflow;
    initChannel("test-channel[root]", ComResult::Ok);
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::Overflow);
    BOOST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(TestChannel::smLastSentValues[0].unwrap())) ==
               0x1234);
    BOOST_TEST(TestChannel::smSendCalls == 1);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(ASYNCHRONOUS_SEND_SUCCESS, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextSendResult = ComResult::Async;
    initChannel("test-channel[root]", ComResult::Ok);
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(eventChainEmpty());
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smSentCallbacks == 0);
    BOOST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(TestChannel::smLastSentValues[0].unwrap())) ==
               0x1234);
    BOOST_TEST(TestChannel::smSendCalls == 1);

    TestChannel::smLastChannel->completeSend(ComResult::Ok);
    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smSentCallbacks == 1);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(ASYNCHRONOUS_SEND_ERROR, GEN_SEND_RECV_TestFixture) {
    TestChannel::smNextSendResult = ComResult::Async;
    initChannel("test-channel[root]", ComResult::Ok);
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(eventChainEmpty());
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smSentCallbacks == 0);
    BOOST_TEST(TestChannel::smSendCalls == 1);

    TestChannel::smLastChannel->completeSend(ComResult::Overflow);
    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::Overflow);
    BOOST_TEST(TestChannel::smSentCallbacks == 1);
    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(INHIBITED_SEND, GEN_SEND_RECV_TestFixture) {
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::Inhibited);
    BOOST_TEST(TestChannel::smOpenCalls == 0);
    BOOST_TEST(TestChannel::smSendCalls == 0);
  }

  BOOST_FIXTURE_TEST_CASE(SEND_WITHOUT_CHANNEL, GEN_SEND_RECV_TestFixture) {
    mQI = true_BOOL;
    mSD1 = 0x1234_INT;

    triggerEvent(scmEventREQID);

    BOOST_TEST(checkForSingleOutputEventOccurence(scmEventCNFID));
    expectStatus(ComResult::NoSocket);
    BOOST_TEST(TestChannel::smOpenCalls == 0);
    BOOST_TEST(TestChannel::smSendCalls == 0);
  }

  BOOST_FIXTURE_TEST_CASE(RECEIVE, GEN_SEND_RECV_TestFixture) {
    initChannel("test-channel[root]", ComResult::Ok);

    std::array receiveData{CIEC_ANY_VARIANT{0x5678_INT}};
    TestChannel::smLastChannel->completeReceive(receiveData);
    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smLastReceiveResult == ComResult::Ok);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 1);
    BOOST_TEST(TestChannel::smPollCalls == 1);
    BOOST_TEST(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(mRD1.unwrap())) == 0x5678);

    std::array mismatchedData{CIEC_ANY_VARIANT{0x1234_INT}, CIEC_ANY_VARIANT{0x2345_INT}};
    TestChannel::smLastChannel->completeReceive(mismatchedData);
    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::DataTypeError);
    BOOST_TEST(TestChannel::smLastReceiveResult == ComResult::DataTypeError);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 2);
    BOOST_TEST(TestChannel::smPollCalls == 2);
    BOOST_TEST(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(mRD1.unwrap())) == 0x5678);

    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smLastReceiveResult == ComResult::DataTypeError);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 2);
    BOOST_TEST(TestChannel::smPollCalls == 3);

    closeChannel();
  }

  BOOST_FIXTURE_TEST_CASE(INHIBITED_POLL, GEN_SEND_RECV_TestFixture) {
    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::Inhibited);
    BOOST_TEST(TestChannel::smOpenCalls == 0);
    BOOST_TEST(TestChannel::smPollCalls == 0);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 0);
  }

  BOOST_FIXTURE_TEST_CASE(POLL_WITHOUT_CHANNEL, GEN_SEND_RECV_TestFixture) {
    mQI = true_BOOL;

    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::NoSocket);
    BOOST_TEST(TestChannel::smOpenCalls == 0);
    BOOST_TEST(TestChannel::smPollCalls == 0);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 0);
  }

  BOOST_FIXTURE_TEST_CASE(POLL_WITHOUT_DATA, GEN_SEND_RECV_TestFixture) {
    initChannel("test-channel[root]", ComResult::Ok);

    triggerEvent(scmEventRSPID);

    BOOST_TEST(checkForSingleOutputEventOccurence(IND));
    expectStatus(ComResult::Ok);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smPollCalls == 1);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 0);
    closeChannel();
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
