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

#include "../../core/fbcontainermock.h"
#include "../../../modules/mccdaqhat/types/MCCIOController.h"
#include "../../../modules/mccdaqhat/types/MCC128.h"
#include "../../../modules/mccdaqhat/types/MCC152.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  BOOST_AUTO_TEST_SUITE(MCCDaqHatFB_Tests)

  BOOST_AUTO_TEST_CASE(controllerDefaultsTo25Hz) {
    FORTE_MCCIOController fb("MCCIOController"_STRID, forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(fb.var_UpdateInterval) == 25U);
  }

  BOOST_AUTO_TEST_CASE(mcc128DefaultsAreSingleEndedTenVoltAtAddressZero) {
    FORTE_MCC128 fb("MCC128"_STRID, forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(fb.var_Address) == 0U);
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(fb.var_InputMode) == 0U);
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(fb.var_InputRange) == 0U);
    BOOST_TEST(fb.getFBInterfaceSpec().getNumDIs() == 12U);
  }

  BOOST_AUTO_TEST_CASE(mcc152DefaultsMatchStateAfterReset) {
    FORTE_MCC152 fb("MCC152"_STRID, forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_TEST(static_cast<unsigned int>(static_cast<TForteByte>(fb.var_PullEnable)) == 0xFFU);
    BOOST_TEST(static_cast<unsigned int>(static_cast<TForteByte>(fb.var_PullConfig)) == 0xFFU);
    BOOST_TEST(fb.getFBInterfaceSpec().getNumDIs() == 22U);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::mccdaqhat
