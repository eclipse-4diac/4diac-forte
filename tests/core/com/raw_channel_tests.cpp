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

#include "channeltestfixtures.h"
#include "forte_boost_output_support.h"
#include "../../../core/src/com/impl/raw.h"

using namespace forte::literals;
using namespace std::literals::string_literals;

namespace forte::com::test {
  BOOST_AUTO_TEST_SUITE(RAW_CHANNEL)

  BOOST_AUTO_TEST_CASE(OPEN_CLOSE) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST(!channel.isOpen());
    BOOST_TEST(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 1);
    BOOST_TEST(TestBufferChannel::smCloseCalls == 0);
    BOOST_TEST(channel.isOpen());

    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 1);
    BOOST_TEST(TestBufferChannel::smCloseCalls == 1);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);
    BOOST_TEST(channel.open("invalid", {}) == ComResult::Ok);
    BOOST_TEST(channel.open("invalid",
                            std::array{
                                ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                            }) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 1);
    BOOST_TEST(TestBufferChannel::smCloseCalls == 0);
    BOOST_TEST(channel.isOpen());

    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 1);
    BOOST_TEST(TestBufferChannel::smCloseCalls == 1);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(VALID_CONFIG) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST(
        channel.open("4", std::array{
                              ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                          }) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(INVALID_CONFIG) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST(channel.open("4", {}) == ComResult::InvalidId);
    BOOST_TEST(TestBufferChannel::smCreated == 0);
    BOOST_TEST(!channel.isOpen());

    BOOST_TEST(channel.open("not-a-size",
                            std::array{
                                ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                            }) == ComResult::InvalidId);
    BOOST_TEST(TestBufferChannel::smCreated == 0);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(SUPPORTS) {
    TestVariantObserver observer;
    const impl::RawChannel channel(observer);

    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "raw"_STRID, .mConfigString = "4"},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "raw"_STRID, .mConfigString = "invalid"},
    }));
    BOOST_TEST(!channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = ""},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "raw"_STRID, .mConfigString = "4"},
        ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
    }));
    BOOST_TEST(!channel.supports({}));
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_BOUNDS) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 1);
    BOOST_TEST(TestBufferChannel::smLastChannel->configString() == "child");
    BOOST_TEST(TestBufferChannel::smLastMinReceiveSize == 0);
    BOOST_TEST(TestBufferChannel::smLastMaxReceiveSize == CIEC_STRING::scmMaxStringLen);

    BOOST_TEST_REQUIRE(channel.close() == ComResult::Ok);
    BOOST_TEST_REQUIRE(
        channel.open("4", std::array{
                              ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "changed"},
                          }) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smCreated == 1);
    BOOST_TEST(TestBufferChannel::smDestroyed == 0);
    BOOST_TEST(TestBufferChannel::smOpenCalls == 2);
    BOOST_TEST(TestBufferChannel::smLastChannel->configString() == "changed");
    BOOST_TEST(TestBufferChannel::smLastMinReceiveSize == 4);
    BOOST_TEST(TestBufferChannel::smLastMaxReceiveSize == 4);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_BOUNDS_INVALID) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST(channel.setMinReceiveSize(0) == ComResult::Ok);
    BOOST_TEST(channel.setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST(channel.setMinReceiveSize(2) == ComResult::DataTypeError);
    BOOST_TEST(channel.setMaxReceiveSize(0) == ComResult::DataTypeError);
    BOOST_TEST(channel.setMaxReceiveSize(1) == ComResult::Ok);
    BOOST_TEST(channel.setMaxReceiveSize(2) == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_INVALID_DATA) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);
    std::span<CIEC_ANY_VARIANT> noData;
    std::array multipleValues{CIEC_ANY_VARIANT{"one"_STRING}, CIEC_ANY_VARIANT{"two"_STRING}};
    std::array nonStringValue{CIEC_ANY_VARIANT{true_BOOL}};
    std::array stringValue{CIEC_ANY_VARIANT{"hello"_STRING}};

    BOOST_TEST(channel.send({}) == ComResult::DataTypeError);
    BOOST_TEST(channel.send(multipleValues) == ComResult::DataTypeError);
    BOOST_TEST(channel.send(nonStringValue) == ComResult::DataTypeError);
    BOOST_TEST(channel.send(stringValue) == ComResult::NoSocket);
    BOOST_TEST(TestBufferChannel::smSendCalls == 0);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV) {
    TestBufferChannel::reset();
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{"hello"_STRING}};
    BOOST_TEST(channel.send(sendData) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smSendCalls == 1);
    BOOST_TEST(TestBufferChannel::smLastSentData == "hello");

    std::string reply{"reply"};
    const ComBuffer receiveData{reply};
    TestBufferChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestBufferChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<const CIEC_STRING &>(observer.mLastReceived->unwrap()) == "reply");
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV_ASYNC) {
    TestBufferChannel::reset();
    TestBufferChannel::smNextSendResult = ComResult::Async;
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{"hello"_STRING}};
    BOOST_TEST(channel.send(sendData) == ComResult::Async);
    BOOST_TEST(TestBufferChannel::smSendCalls == 1);
    BOOST_TEST(TestBufferChannel::smLastSentData == "hello");

    std::array sendAgainData{CIEC_ANY_VARIANT{"again"_STRING}};
    BOOST_TEST(channel.send(sendAgainData) == ComResult::SendFailed);
    BOOST_TEST(TestBufferChannel::smSendCalls == 1);
    BOOST_TEST(TestBufferChannel::smLastSentData == "hello");

    TestBufferChannel::smLastChannel->completeSend(ComResult::Ok);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mSentCalls == 1);
    BOOST_TEST(observer.mLastSentResult == ComResult::Ok);
    BOOST_TEST(observer.mLastSentChannel == &channel);

    std::string reply{"reply"};
    const ComBuffer receiveData{reply};
    TestBufferChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestBufferChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<const CIEC_STRING &>(observer.mLastReceived->unwrap()) == "reply");
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_CASE(SEND_ASYNC_CLOSE_REOPEN_SEND) {
    TestBufferChannel::reset();
    TestBufferChannel::smNextSendResult = ComResult::Async;
    TestVariantObserver observer;
    impl::RawChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);

    std::array firstSendData{CIEC_ANY_VARIANT{"hello"_STRING}};
    BOOST_TEST(channel.send(firstSendData) == ComResult::Async);
    BOOST_TEST(TestBufferChannel::smSendCalls == 1);
    BOOST_TEST(TestBufferChannel::smLastSentData == "hello");

    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(!channel.isOpen());

    TestBufferChannel::smNextSendResult = ComResult::Ok;
    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-buffer-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);

    std::array secondSendData{CIEC_ANY_VARIANT{"again"_STRING}};
    BOOST_TEST(channel.send(secondSendData) == ComResult::Ok);
    BOOST_TEST(TestBufferChannel::smSendCalls == 2);
    BOOST_TEST(TestBufferChannel::smLastSentData == "again");
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
