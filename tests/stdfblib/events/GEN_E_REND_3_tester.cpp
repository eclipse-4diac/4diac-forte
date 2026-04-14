/*******************************************************************************
 * Copyright (c) 2026 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../../core/fbtests/fbtestfixture.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct GEN_E_REND_3_TestFixture : public forte::test::CFBTestFixtureBase {
      GEN_E_REND_3_TestFixture() : CFBTestFixtureBase("iec61499::events::E_REND_3"_STRID) {
        setup();
      }

      static constexpr TEventID EI1 = 0;
      static constexpr TEventID EI2 = 1;
      static constexpr TEventID EI3 = 2;
      static constexpr TEventID R = 3;
      static constexpr TEventID EO = 0;
  };

  BOOST_FIXTURE_TEST_SUITE(GenERend3Tests, GEN_E_REND_3_TestFixture)

  BOOST_AUTO_TEST_CASE(Rendezvous3) {
    triggerEvent(EI1);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI2);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI3);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
  }

  BOOST_AUTO_TEST_CASE(Rendezvous3_OutOfOrder) {
    triggerEvent(EI2);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI3);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI1);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
  }

  BOOST_AUTO_TEST_CASE(Reset) {
    triggerEvent(EI1);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI2);
    BOOST_CHECK(eventChainEmpty());

    triggerEvent(R);
    BOOST_CHECK(eventChainEmpty());

    // After reset, triggering the last one should not cause EO
    triggerEvent(EI3);
    BOOST_CHECK(eventChainEmpty());

    // Now need to trigger 1 and 2 again
    triggerEvent(EI1);
    BOOST_CHECK(eventChainEmpty());
    triggerEvent(EI2);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
  }

  BOOST_AUTO_TEST_CASE(MultipleRendezvous) {
    for (int i = 0; i < 5; i++) {
      triggerEvent(EI1);
      BOOST_CHECK(eventChainEmpty());
      triggerEvent(EI2);
      BOOST_CHECK(eventChainEmpty());
      triggerEvent(EI3);
      BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
