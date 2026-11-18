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

#include "../../../modules/mccdaqhat/mccdaqhat_config.h"

#include <limits>

namespace forte::eclipse4diac::io::mccdaqhat {

  BOOST_AUTO_TEST_SUITE(MCCDaqHatConfig_Tests)

  BOOST_AUTO_TEST_CASE(toCodeRoundsAndClamps) {
    BOOST_TEST(toCode(-3.0, 65535) == 0);
    BOOST_TEST(toCode(0.4, 65535) == 0);
    BOOST_TEST(toCode(1234.6, 65535) == 1235);
    BOOST_TEST(toCode(70000.0, 65535) == 65535);
    BOOST_TEST(toCode(5000.0, 4095) == 4095);
  }

  BOOST_AUTO_TEST_CASE(toCodeMapsNaNToZero) {
    BOOST_TEST(toCode(std::numeric_limits<double>::quiet_NaN(), 65535) == 0);
  }

  BOOST_AUTO_TEST_CASE(addressMustBeWithinJumperRange) {
    BOOST_TEST(validateAddress(0).empty());
    BOOST_TEST(validateAddress(7).empty());
    BOOST_TEST(validateAddress(8) == "Address 8 is out of range 0..7");
  }

  BOOST_AUTO_TEST_CASE(mcc128RejectsInvalidModeAndRange) {
    const TChannelUsage none{};
    BOOST_TEST(validateMCC128Config(2, 0, none) == "InputMode 2 is invalid (0 = single-ended, 1 = differential)");
    BOOST_TEST(validateMCC128Config(0, 4, none) == "InputRange 4 is invalid (0..3)");
    BOOST_TEST(validateMCC128Config(1, 3, none).empty());
  }

  BOOST_AUTO_TEST_CASE(mcc128DifferentialAllowsOnlyFirstFourChannels) {
    TChannelUsage used{};
    used[3] = true;
    BOOST_TEST(validateMCC128Config(1, 0, used).empty());
    used[4] = true;
    BOOST_TEST(validateMCC128Config(1, 0, used) == "AI_4 is not available in differential mode");
    BOOST_TEST(validateMCC128Config(0, 0, used).empty());
  }

  BOOST_AUTO_TEST_CASE(mcc152DirectionFollowsMapping) {
    TChannelUsage inputs{};
    TChannelUsage outputs{};
    outputs[0] = true;
    outputs[5] = true;
    inputs[1] = true;
    uint8_t mask = 0;
    BOOST_TEST(computeMCC152DirectionMask(inputs, outputs, mask).empty());
    BOOST_TEST(static_cast<unsigned int>(mask) == 0xDEU); // bits 0 and 5 are outputs
  }

  BOOST_AUTO_TEST_CASE(mcc152RejectsBitMappedAsInputAndOutput) {
    TChannelUsage inputs{};
    TChannelUsage outputs{};
    inputs[2] = true;
    outputs[2] = true;
    uint8_t mask = 0;
    BOOST_TEST(computeMCC152DirectionMask(inputs, outputs, mask) == "Bit 2 is mapped as DI_2 and DO_2");
  }

  BOOST_AUTO_TEST_CASE(mcc152AnalogOutputIsClampedTo12Bit) {
    bool clamped = true;
    BOOST_TEST(clampMCC152AoCode(4095, clamped) == 4095);
    BOOST_TEST(!clamped);
    BOOST_TEST(clampMCC152AoCode(4096, clamped) == 4095);
    BOOST_TEST(clamped);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::mccdaqhat
