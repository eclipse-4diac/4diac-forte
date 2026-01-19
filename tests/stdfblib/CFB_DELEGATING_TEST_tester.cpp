/*******************************************************************************
 * Copyright (c) 2014, 2026 fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *   Martin Erich Jobst - copied from CFB_TEST_tester and adapted for CFB_DELEGATING_TEST
 *******************************************************************************/

#include "CFB_DELEGATING_TEST.h"
#include "../../core/fbtests/fbtestfixture.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::test {
  struct CFB_DELEGATING_TEST_TestFixture : CFBTestFixtureBase {

      CFB_DELEGATING_TEST_TestFixture() : CFBTestFixtureBase("test::CFB_DELEGATING_TEST"_STRID) {
        setInputData({&var_QI, &var_DI1});
        setOutputData({&var_QO, &var_DO1});
        setup();
      }

      CIEC_BOOL var_QI;
      CIEC_DelegatingTestStruct var_DI1;
      CIEC_BOOL var_QO;
      CIEC_DelegatingTestStruct var_DO1;
  };

  BOOST_FIXTURE_TEST_SUITE(DelegatingTests, CFB_DELEGATING_TEST_TestFixture)

  BOOST_AUTO_TEST_CASE(initTest) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(var_QO == true);
    BOOST_TEST(var_DO1.var_VAR1 == false);
    BOOST_TEST(var_DO1.var_VAR2 == false);
    BOOST_TEST(var_DO1.var_VAR3 == false);
  }

  BOOST_AUTO_TEST_CASE(dataTest) {
    var_QI = true_BOOL;
    var_DI1.var_VAR1 = true_BOOL;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(var_QO == false);
    BOOST_TEST(var_DO1.var_VAR1 == true);
    BOOST_TEST(var_DO1.var_VAR2 == false);
    BOOST_TEST(var_DO1.var_VAR3 == false);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
