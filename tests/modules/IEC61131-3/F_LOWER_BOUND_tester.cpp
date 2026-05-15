/*******************************************************************************
 * Copyright (c) 2026 Martin Melik Merkumians
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#include "../../core/include/forte/datatypes/forte_any_derived_variant.h"
#include "../../core/include/forte/datatypes/forte_any_int_variant.h"
#include "../../core/include/forte/datatypes/forte_array_fixed.h"

using namespace forte::literals;

namespace forte::iec61131::arrays {
  struct F_LOWER_BOUND_TestFixture : public test::CFBTestFixtureBase {

      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_LOWER_BOUND_TestFixture() : CFBTestFixtureBase("iec61131::arrays::F_LOWER_BOUND"_STRID) {
        setInputData({&mDIM_LOWER_BOUND, &mARR_LOWER_BOUND});
        setOutputData({&mOUT_LOWER_BOUND});
        setup();
      }

      CIEC_LINT mDIM_LOWER_BOUND; // DATA INPUT
      CIEC_ARRAY_FIXED<CIEC_INT, -10, 10> mARR_LOWER_BOUND; // DATA INPUT

      CIEC_LINT mOUT_LOWER_BOUND;
  };

  BOOST_FIXTURE_TEST_SUITE(F_LOWER_BOUND_Tests, F_LOWER_BOUND_TestFixture)

  BOOST_AUTO_TEST_CASE(firstDimensionTest) {
    mDIM_LOWER_BOUND = 1_LINT;
    mARR_LOWER_BOUND = CIEC_ARRAY_FIXED<CIEC_INT, -10, 10>();
    /* trigger the inputevent */
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(-10, static_cast<CIEC_LINT::TValueType>(mOUT_LOWER_BOUND));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arrays
