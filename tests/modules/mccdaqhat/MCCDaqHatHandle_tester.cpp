/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "FakeMCCDaqHatApi.h"
#include "../../../modules/mccdaqhat/handles/MCCHandles.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_word.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  using test::FakeMCCDaqHatApi;

  BOOST_AUTO_TEST_SUITE(MCCDaqHatHandle_Tests)

  BOOST_AUTO_TEST_CASE(analogInputReportsChangeOnceAndReadsCode) {
    FakeMCCDaqHatApi api;
    MCC128Board board(api, 0);
    TChannelUsage used{};
    used[2] = true;
    BOOST_TEST(board.open(0, 0, used).empty());
    MCCAnalogInHandle handle(nullptr, 5, board, 2);
    BOOST_TEST(handle.getDeviceIndex() == 5U);
    api.mAnalogCodes[2] = 4711;
    BOOST_TEST(board.poll());
    BOOST_TEST(handle.checkChanged());
    BOOST_TEST(!handle.checkChanged());
    CIEC_WORD value;
    handle.get(value);
    BOOST_TEST(static_cast<TForteWord>(value) == 4711);
  }

  BOOST_AUTO_TEST_CASE(digitalInputFollowsPortBit) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFF, 0xFF, 0xFF).empty());
    MCCDigitalInHandle handle(nullptr, 0, board, 3);
    api.mInputPort = 0x08;
    BOOST_TEST(board.poll());
    BOOST_TEST(handle.checkChanged());
    CIEC_BOOL value;
    handle.get(value);
    BOOST_TEST(static_cast<bool>(value));
  }

  BOOST_AUTO_TEST_CASE(digitalOutputWritesBit) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFE, 0xFF, 0xFF).empty());
    api.mCalls.clear();
    MCCDigitalOutHandle handle(nullptr, 0, board, 0);
    handle.set(CIEC_BOOL(true));
    BOOST_TEST(api.mCalls.back() == "mcc152DioOutputWriteBit(1,0,1)");
    BOOST_TEST(!handle.checkChanged());
  }

  BOOST_AUTO_TEST_CASE(analogOutputClampsTo12Bit) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFF, 0xFF, 0xFF).empty());
    MCCAnalogOutHandle handle(nullptr, 0, board, 1);
    handle.set(CIEC_WORD(5000));
    BOOST_TEST(api.mCalls.back() == "mcc152OutputWriteCode(1,1,4095)");
    CIEC_WORD value;
    handle.get(value);
    BOOST_TEST(static_cast<TForteWord>(value) == 4095);
  }

  BOOST_AUTO_TEST_CASE(digitalOutputReportsFailedWriteOncePerEpisode) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFE, 0xFF, 0xFF).empty());
    MCCDigitalOutHandle handle(nullptr, 0, board, 0);
    BOOST_TEST(!handle.isWriteErrorReported());
    api.mNextResult = -7;
    handle.set(CIEC_BOOL(true));
    BOOST_TEST(handle.isWriteErrorReported());
    handle.set(CIEC_BOOL(false)); // second failure of the same episode stays quiet, the flag stays set
    BOOST_TEST(handle.isWriteErrorReported());
    api.mNextResult = IMCCDaqHatApi::scmSuccess;
    handle.set(CIEC_BOOL(true));
    BOOST_TEST(!handle.isWriteErrorReported());
    api.mNextResult = -7; // a new failure starts a new episode
    handle.set(CIEC_BOOL(false));
    BOOST_TEST(handle.isWriteErrorReported());
  }

  BOOST_AUTO_TEST_CASE(analogOutputReportsFailedWriteOncePerEpisode) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFF, 0xFF, 0xFF).empty());
    MCCAnalogOutHandle handle(nullptr, 0, board, 0);
    api.mNextResult = -7;
    handle.set(CIEC_WORD(100));
    BOOST_TEST(handle.isWriteErrorReported());
    api.mNextResult = IMCCDaqHatApi::scmSuccess;
    handle.set(CIEC_WORD(101));
    BOOST_TEST(!handle.isWriteErrorReported());
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::mccdaqhat
