/*******************************************************************************
 * Copyright (c) 2026 MSR Software GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Tim Grocki - initial API and implementation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../core/fbcontainermock.h"
#include "../../../modules/ethercat/types/ECController.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {

  BOOST_AUTO_TEST_SUITE(ECController_Tests)

  BOOST_AUTO_TEST_CASE(updateIntervalDefaultIsPositive) {
    FORTE_ECController fb("ECController"_STRID, test::CFBContainerMock::smDefaultFBContMock);
    BOOST_TEST(fb.var_UpdateInterval.getInNanoSeconds() > 0);
  }

  BOOST_AUTO_TEST_CASE(updateIntervalResetMatchesConstructorDefault) {
    FORTE_ECController fb("ECController"_STRID, test::CFBContainerMock::smDefaultFBContMock);
    const auto defaultInterval = fb.var_UpdateInterval.getInNanoSeconds();
    fb.var_UpdateInterval = 0_TIME;
    BOOST_TEST((fb.changeExecutionState(EMGMCommandType::Start) == EMGMResponse::Ready));
    BOOST_TEST((fb.changeExecutionState(EMGMCommandType::Stop) == EMGMResponse::Ready));
    BOOST_TEST((fb.changeExecutionState(EMGMCommandType::Reset) == EMGMResponse::Ready));
    BOOST_TEST(fb.var_UpdateInterval.getInNanoSeconds() == defaultInterval);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::eclipse4diac::io::ethercat
