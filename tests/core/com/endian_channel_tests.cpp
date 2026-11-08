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
#include <bit>
#include <string>

#include "channeltestfixtures.h"
#include "forte_boost_output_support.h"
#include "../../../core/src/com/impl/endian.h"

using namespace forte::literals;

namespace forte::com::test {
  BOOST_AUTO_TEST_SUITE(ENDIAN_CHANNEL)

  BOOST_AUTO_TEST_CASE(OPEN_CLOSE) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST(!channel.isOpen());
    BOOST_TEST(channel.open({}, std::array{
                                    ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                                }) == ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smCloseCalls == 0);
    BOOST_TEST(channel.isOpen());

    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smCloseCalls == 1);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open({}, std::array{
                             ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                         }) == ComResult::Ok);
    BOOST_TEST(channel.open("invalid", {}) == ComResult::Ok);
    BOOST_TEST(
        channel.open("invalid", std::array{
                                    ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                                }) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smCloseCalls == 0);

    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smCloseCalls == 1);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(VALID_CONFIG) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    for (const std::string config : {"", "native", "big", "little"}) {
      BOOST_TEST(
          channel.open(config, std::array{
                                   ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                               }) == ComResult::Ok);
      BOOST_TEST(TestChannel::smCreated == 1);
      BOOST_TEST(channel.isOpen());
      BOOST_TEST(channel.close() == ComResult::Ok);
      BOOST_TEST(!channel.isOpen());
    }
  }

  BOOST_AUTO_TEST_CASE(INVALID_CONFIG) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST(channel.open("invalid", {}) == ComResult::InvalidId);
    BOOST_TEST(TestChannel::smCreated == 0);
    BOOST_TEST(!channel.isOpen());

    BOOST_TEST(channel.open("big", {}) == ComResult::InvalidId);
    BOOST_TEST(TestChannel::smCreated == 0);
    BOOST_TEST(!channel.isOpen());

    BOOST_TEST(
        channel.open("invalid", std::array{
                                    ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                                }) == ComResult::InvalidId);
    BOOST_TEST(TestChannel::smCreated == 0);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(SUPPORTS) {
    TestVariantObserver observer;
    const impl::EndianChannel channel(observer);

    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "endian"_STRID, .mConfigString = ""},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "endian"_STRID, .mConfigString = "invalid"},
    }));
    BOOST_TEST(!channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = ""},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "endian"_STRID, .mConfigString = "big"},
        ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
    }));
    BOOST_TEST(!channel.supports({}));
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_BOUNDS) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);
    BOOST_TEST(channel.setMinReceiveSize(3) == ComResult::Ok);
    BOOST_TEST(channel.setMaxReceiveSize(6) == ComResult::Ok);

    BOOST_TEST(channel.open({}, std::array{
                                    ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                                }) == ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 1);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "child");
    BOOST_TEST(TestChannel::smLastMinReceiveSize == 3);
    BOOST_TEST(TestChannel::smLastMaxReceiveSize == 6);

    BOOST_TEST(channel.setMinReceiveSize(4) == ComResult::Ok);
    BOOST_TEST(channel.setMaxReceiveSize(8) == ComResult::Ok);
    BOOST_TEST(TestChannel::smLastChannel->getMinReceiveSize() == 4);
    BOOST_TEST(TestChannel::smLastChannel->getMaxReceiveSize() == 8);

    BOOST_TEST_REQUIRE(channel.close() == ComResult::Ok);
    BOOST_TEST(
        channel.open("big", std::array{
                                ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "changed"},
                            }) == ComResult::Ok);
    BOOST_TEST(TestChannel::smCreated == 1);
    BOOST_TEST(TestChannel::smDestroyed == 0);
    BOOST_TEST(TestChannel::smOpenCalls == 2);
    BOOST_TEST(TestChannel::smLastChannel->configString() == "changed");
    BOOST_TEST(TestChannel::smLastMinReceiveSize == 4);
    BOOST_TEST(TestChannel::smLastMaxReceiveSize == 8);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV_BIG) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open("big", std::array{
                                ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                            }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{0x1234_WORD}};
    BOOST_TEST(channel.send(sendData) == ComResult::Ok);
    BOOST_TEST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(TestChannel::smLastSentValues[0].unwrap())) ==
               (std::endian::native == std::endian::big ? 0x1234 : 0x3412));

    std::array receiveData{CIEC_ANY_VARIANT{CIEC_WORD((std::endian::native == std::endian::big ? 0x1234 : 0x3412))}};
    TestChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(observer.mLastReceived->unwrap())) == 0x1234);
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV_LITTLE) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open("little", std::array{
                                   ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                               }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{0x1234_WORD}};
    BOOST_TEST(channel.send(sendData) == ComResult::Ok);
    BOOST_TEST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(TestChannel::smLastSentValues[0].unwrap())) ==
               (std::endian::native == std::endian::little ? 0x1234 : 0x3412));

    std::array receiveData{CIEC_ANY_VARIANT{CIEC_WORD((std::endian::native == std::endian::little ? 0x1234 : 0x3412))}};
    TestChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(observer.mLastReceived->unwrap())) == 0x1234);
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV_NATIVE) {
    TestChannel::reset();
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST_REQUIRE(
        channel.open("native", std::array{
                                   ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                               }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{0x1234_WORD}};
    BOOST_TEST(channel.send(sendData) == ComResult::Ok);
    BOOST_TEST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(TestChannel::smLastSentValues[0].unwrap())) ==
               0x1234);

    std::array receiveData{CIEC_ANY_VARIANT{0x1234_WORD}};
    TestChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(observer.mLastReceived->unwrap())) == 0x1234);
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV_ASYNC) {
    TestChannel::reset();
    TestChannel::smNextSendResult = ComResult::Async;
    TestVariantObserver observer;
    impl::EndianChannel channel(observer);

    BOOST_TEST(channel.open({}, std::array{
                                    ComChannelDescriptor{.mChannel = "test-channel"_STRID, .mConfigString = "child"},
                                }) == ComResult::Ok);

    std::array sendData{CIEC_ANY_VARIANT{0x1234_WORD}};
    BOOST_TEST(channel.send(sendData) == ComResult::Async);
    BOOST_TEST_REQUIRE(TestChannel::smLastSentValues.size() == 1);
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(TestChannel::smLastSentValues[0].unwrap())) ==
               0x1234);

    TestChannel::smLastChannel->completeSend(ComResult::Ok);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mSentCalls == 1);
    BOOST_TEST(observer.mLastSentResult == ComResult::Ok);
    BOOST_TEST(observer.mLastSentChannel == &channel);

    std::array receiveData{CIEC_ANY_VARIANT{0x5678_WORD}};
    TestChannel::smLastChannel->completeReceive(receiveData);
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(TestChannel::smReceiveCallbacks == 1);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST_REQUIRE(observer.mLastReceived.has_value());
    BOOST_TEST(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(observer.mLastReceived->unwrap())) == 0x5678);
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
