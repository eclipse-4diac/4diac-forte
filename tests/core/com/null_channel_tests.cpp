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
#include "../../../core/src/com/impl/null.h"

using namespace forte::literals;

namespace forte::com::test {
  BOOST_AUTO_TEST_SUITE(NULL_CHANNEL)

  BOOST_AUTO_TEST_CASE(OPEN_CLOSE) {
    TestBufferObserver observer;
    impl::NullChannel channel(observer);

    BOOST_TEST(!channel.isOpen());
    BOOST_TEST(channel.open({}, {}) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN) {
    TestBufferObserver observer;
    impl::NullChannel channel(observer);

    BOOST_TEST_REQUIRE(channel.open({}, {}) == ComResult::Ok);
    BOOST_TEST(channel.open("invalid", {}) == ComResult::Ok);
    BOOST_TEST(channel.open({}, std::array{
                                    ComChannelDescriptor{.mChannel = "test-buffer"_STRID, .mConfigString = "child"},
                                }) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(channel.close() == ComResult::Ok);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(INVALID_CONFIG) {
    TestBufferObserver observer;
    impl::NullChannel channel(observer);

    BOOST_TEST(channel.open("invalid", {}) == ComResult::InvalidId);
    BOOST_TEST(!channel.isOpen());
    BOOST_TEST(channel.open({}, std::array{
                                    ComChannelDescriptor{.mChannel = "test-buffer"_STRID, .mConfigString = "child"},
                                }) == ComResult::InvalidId);
    BOOST_TEST(!channel.isOpen());
  }

  BOOST_AUTO_TEST_CASE(SUPPORTS) {
    TestBufferObserver observer;
    const impl::NullChannel channel(observer);

    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "null"_STRID, .mConfigString = ""},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "null"_STRID, .mConfigString = "invalid"},
    }));
    BOOST_TEST(!channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "test-buffer"_STRID, .mConfigString = ""},
    }));
    BOOST_TEST(channel.supports(std::array{
        ComChannelDescriptor{.mChannel = "null"_STRID, .mConfigString = ""},
        ComChannelDescriptor{.mChannel = "test-buffer"_STRID, .mConfigString = "child"},
    }));
    BOOST_TEST(!channel.supports({}));
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_BOUNDS_INVALID) {
    TestBufferObserver observer;
    impl::NullChannel channel(observer);

    BOOST_TEST(channel.setMinReceiveSize(0) == ComResult::Ok);
    BOOST_TEST(channel.setMinReceiveSize(1) == ComResult::Overflow);
    BOOST_TEST(channel.setMaxReceiveSize(0) == ComResult::Ok);
    BOOST_TEST(channel.setMaxReceiveSize(1) == ComResult::Ok);
    BOOST_TEST(channel.setMaxReceiveSize(2) == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_RECV) {
    TestBufferObserver observer;
    impl::NullChannel channel(observer);

    BOOST_TEST_REQUIRE(channel.open({}, {}) == ComResult::Ok);

    std::string sendData{"hello"};
    BOOST_TEST(channel.send(ComBuffer(sendData)) == ComResult::Ok);

    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceiveCalls == 1);
    BOOST_TEST(observer.mLastReceiveSize == 0);
    BOOST_TEST(observer.mLastReceiveChannel == &channel);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
