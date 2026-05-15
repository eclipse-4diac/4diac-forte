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

namespace forte::eclipse4diac::utils::arrays {
  struct F_LEN_ARRAY_TestFixture : public test::CFBTestFixtureBase {

      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_LEN_ARRAY_TestFixture() : CFBTestFixtureBase("eclipse4diac::utils::arrays::F_LEN_ARRAY"_STRID) {
        setInputData({&mDIM_LEN_ARRAY, &mARR_LEN_ARRAY});
        setOutputData({&mOUT_LEN_ARRAY});
        setup();
      }

      CIEC_LINT mDIM_LEN_ARRAY; // DATA INPUT
      CIEC_ARRAY_FIXED<CIEC_INT, -10, 10> mARR_LEN_ARRAY; // DATA INPUT

      CIEC_LINT mOUT_LEN_ARRAY;
  };

  BOOST_FIXTURE_TEST_SUITE(F_LEN_ARRAY_Tests, F_LEN_ARRAY_TestFixture)

  BOOST_AUTO_TEST_CASE(firstDimensionTest) {
    mDIM_LEN_ARRAY = 1_INT;
    mARR_LEN_ARRAY = CIEC_ARRAY_FIXED<CIEC_INT, -10, 10>();
    /* trigger the inputevent */
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(21, static_cast<CIEC_LINT::TValueType>(mOUT_LEN_ARRAY));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::eclipse4diac::utils::arrays
