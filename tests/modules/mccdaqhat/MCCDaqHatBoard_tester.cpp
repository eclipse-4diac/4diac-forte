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
#include "../../../modules/mccdaqhat/boards/MCC128Board.h"
#include "../../../modules/mccdaqhat/boards/MCC152Board.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  using test::FakeMCCDaqHatApi;

  BOOST_AUTO_TEST_SUITE(MCCDaqHatBoard_Tests)

  BOOST_AUTO_TEST_CASE(mcc128OpenConfiguresModeAndRange) {
    FakeMCCDaqHatApi api;
    MCC128Board board(api, 3);
    BOOST_TEST(board.open(1, 2, TChannelUsage{}).empty());
    const std::vector<std::string> expected{"mcc128Open(3)", "mcc128InputModeWrite(3,1)", "mcc128InputRangeWrite(3,2)"};
    BOOST_TEST(api.mCalls == expected, boost::test_tools::per_element());
  }

  BOOST_AUTO_TEST_CASE(mcc128OpenFailureClosesAndReportsAddress) {
    FakeMCCDaqHatApi api;
    api.mNextResult = -7;
    MCC128Board board(api, 2);
    BOOST_TEST(board.open(0, 0, TChannelUsage{}) == "Address 2: fake error");
    BOOST_TEST(api.mCalls.back() == "mcc128Close(2)");
  }

  BOOST_AUTO_TEST_CASE(mcc128PollReadsOnlyUsedChannels) {
    FakeMCCDaqHatApi api;
    MCC128Board board(api, 0);
    TChannelUsage used{};
    used[1] = true;
    used[6] = true;
    BOOST_TEST(board.open(0, 0, used).empty());
    api.mCalls.clear();
    api.mAnalogCodes[1] = 1000;
    api.mAnalogCodes[6] = 60000;
    BOOST_TEST(board.poll());
    const std::vector<std::string> expected{"mcc128InputReadCode(0,1)", "mcc128InputReadCode(0,6)"};
    BOOST_TEST(api.mCalls == expected, boost::test_tools::per_element());
    BOOST_TEST(board.getCode(1) == 1000);
    BOOST_TEST(board.getCode(6) == 60000);
    BOOST_TEST(board.getCode(0) == 0);
  }

  BOOST_AUTO_TEST_CASE(pollFailsAfterTenConsecutiveErrorsAndRecovers) {
    FakeMCCDaqHatApi api;
    MCC128Board board(api, 0);
    TChannelUsage used{};
    used[0] = true;
    BOOST_TEST(board.open(0, 0, used).empty());
    api.mNextResult = -3;
    for (unsigned int i = 1; i < MCCBoard::scmMaxConsecutiveErrors; ++i) {
      BOOST_TEST(board.poll());
    }
    BOOST_TEST(!board.poll());
    BOOST_TEST(board.getLastError() == "Address 0: fake error");
    api.mNextResult = 0;
    BOOST_TEST(board.poll());
  }

  BOOST_AUTO_TEST_CASE(pollOnClosedBoardDoesNothing) {
    FakeMCCDaqHatApi api;
    MCC128Board board(api, 0);
    BOOST_TEST(board.poll());
    BOOST_TEST(api.mCalls.empty());
  }

  BOOST_AUTO_TEST_CASE(mcc152OpenSetsPullThenOutputsLowThenDirection) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xDE, 0xF0, 0x0F).empty());
    const std::vector<std::string> expected{"mcc152Open(1)",
                                            "mcc152DioReset(1)",
                                            "mcc152DioConfigWritePort(1,2,240)",
                                            "mcc152DioConfigWritePort(1,1,15)",
                                            "mcc152DioOutputWritePort(1,0)",
                                            "mcc152DioConfigWritePort(1,0,222)"};
    BOOST_TEST(api.mCalls == expected, boost::test_tools::per_element());
  }

  BOOST_AUTO_TEST_CASE(mcc152PollCachesInputPort) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 1);
    BOOST_TEST(board.open(0xFF, 0xFF, 0xFF).empty());
    api.mInputPort = 0x05;
    BOOST_TEST(board.poll());
    BOOST_TEST(board.getInputBit(0));
    BOOST_TEST(!board.getInputBit(1));
    BOOST_TEST(board.getInputBit(2));
  }

  BOOST_AUTO_TEST_CASE(mcc152WritesBitAndAnalogCode) {
    FakeMCCDaqHatApi api;
    MCC152Board board(api, 4);
    BOOST_TEST(board.open(0xFE, 0xFF, 0xFF).empty());
    api.mCalls.clear();
    BOOST_TEST(board.writeOutputBit(0, true) == 0);
    BOOST_TEST(board.writeAnalogCode(1, 2048) == 0);
    const std::vector<std::string> expected{"mcc152DioOutputWriteBit(4,0,1)", "mcc152OutputWriteCode(4,1,2048)"};
    BOOST_TEST(api.mCalls == expected, boost::test_tools::per_element());
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::mccdaqhat
