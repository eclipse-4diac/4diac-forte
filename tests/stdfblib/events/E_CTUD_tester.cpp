/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH, 2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"
#include "forte/iec61131_functions/func_GE.h"
#include "forte/iec61131_functions/func_LT.h"
#include "forte/iec61131_functions/func_NE.h"
#include "forte/iec61131_functions/func_NOT.h"
#include "forte/iec61131_functions/func_OR.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct E_CTUD_TestFixture : public forte::test::CFBTestFixtureBase {
      static constexpr TEventID CU = 0;
      static constexpr TEventID CD = 1;
      static constexpr TEventID R = 2;
      static constexpr TEventID LD = 3;
      static constexpr TEventID CO = 0;
      static constexpr TEventID RO = 1;
      static constexpr TEventID LDO = 2;

      E_CTUD_TestFixture() : CFBTestFixtureBase("iec61499::events::E_CTUD"_STRID) {
        setInputData({&mInPV});
        setOutputData({&mOutQU, &mOutQD, &mOutCV});
        setup();
      }

      CIEC_UINT mInPV; // Preset value
      CIEC_BOOL mOutQU; // CV >= PV
      CIEC_BOOL mOutQD; // CV <= 0
      CIEC_UINT mOutCV; // count value

      bool checkCU(unsigned int paPrevCV) {
        if (paPrevCV < 65535) {
          if (((paPrevCV + 1) != static_cast<CIEC_UINT::TValueType>(mOutCV))) {
            return false;
          } else if (!checkForSingleOutputEventOccurence(CO)) {
            return false;
          }
        } else {
          if (65535 != static_cast<CIEC_UINT::TValueType>(mOutCV) && !eventChainEmpty()) {
            return false;
          }
        }
        if (!checkBooleans()) {
          return false;
        }
        return true;
      }

      bool checkCD(unsigned int paPrevCV) {
        if (paPrevCV < 1) {
          // no algorithm should have been executed
          if (static_cast<CIEC_UINT::TValueType>(mOutCV) != paPrevCV || !mOutQD || !eventChainEmpty()) {
            return false;
          }
        } else {
          if (((paPrevCV - 1) != static_cast<CIEC_UINT::TValueType>(mOutCV))) {
            return false;
          } else if (mOutQD != (static_cast<CIEC_UINT::TValueType>(mOutCV) < 1)) {
            return false;
          } else if (!checkForSingleOutputEventOccurence(CO)) {
            return false;
          }
        }
        if (!checkBooleans()) {
          return false;
        }
        return true;
      }

      bool checkR() {
        if (0 != static_cast<CIEC_UINT::TValueType>(mOutCV)) {
          return false;
        }
        if (!checkForSingleOutputEventOccurence(RO)) {
          return false;
        }
        if (!checkBooleans()) {
          return false;
        }
        return true;
      }

      bool checkLD(unsigned int paUsedPV) {
        if (paUsedPV != static_cast<CIEC_UINT::TValueType>(mInPV) ||
            static_cast<CIEC_UINT::TValueType>(mInPV) != static_cast<CIEC_UINT::TValueType>(mOutCV) ||
            ((paUsedPV < 1) != (true == static_cast<CIEC_BOOL::TValueType>(mOutQD)))) {
          return false;
        }
        if (!checkForSingleOutputEventOccurence(LDO)) {
          return false;
        }
        if (!checkBooleans()) {
          return false;
        }
        return true;
      }

      bool checkBooleans() {
        return func_NOT(func_OR(func_NE(mOutQU, func_GE(mOutCV, mInPV)), func_NE(mOutQD, func_LT(mOutCV, 1_UINT))));
      }
  };

  BOOST_FIXTURE_TEST_SUITE(CTUDTests, E_CTUD_TestFixture)

  BOOST_AUTO_TEST_CASE(EventCU) {
    TForteUInt16 prevCV = 0;
    TForteUInt16 valuesToTest[] = {10, 1, 0, 65534, 65535};
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for (unsigned int j = 0; j < numberOfValues; j++) {
      triggerEvent(R);
      BOOST_CHECK(checkR());
      mInPV = CIEC_UINT(valuesToTest[j]);
      for (unsigned int k = 0U; k < static_cast<CIEC_UINT::TValueType>(mInPV) + 3U; k++) {
        prevCV = static_cast<CIEC_UINT::TValueType>(mOutCV);
        // Send event
        triggerEvent(CU);
        BOOST_CHECK(checkCU(prevCV));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventCD) {
    unsigned int numberOfTries = 100;
    TForteUInt16 valuesToTest[] = {10, 1, 0, 65534, 65535};
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for (unsigned int i = 0; i < numberOfTries; i++) {
      for (unsigned int j = 0; j < numberOfValues; j++) {
        mInPV = CIEC_UINT(valuesToTest[j]);
        triggerEvent(LD);
        checkForSingleOutputEventOccurence(RO);
        // Send event
        triggerEvent(CD);
        BOOST_CHECK(checkCD(valuesToTest[j]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventR) {
    unsigned int numberOfTries = 100;
    TForteUInt16 valuesToTest[] = {10, 1, 0, 65534, 65535};
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for (unsigned int i = 0; i < numberOfTries; i++) {
      for (unsigned int j = 0; j < numberOfValues; j++) {
        mInPV = CIEC_UINT(valuesToTest[j]);
        triggerEvent(LD); // loads the value to input of the FB, because the Rese event doesn't scan the PV input.
        checkForSingleOutputEventOccurence(RO);
        triggerEvent(R);
        BOOST_CHECK(checkR());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventLD) {
    unsigned int numberOfTries = 100;
    TForteUInt16 PVToTest[] = {10, 1, 0, 65534, 65535};
    unsigned int numberOftest = static_cast<unsigned int>(sizeof(PVToTest) / sizeof(TForteUInt16));
    for (unsigned int i = 0; i < numberOfTries; i++) {
      for (unsigned int j = 0; j < numberOftest; j++) {
        mInPV = CIEC_UINT(PVToTest[j]);
        triggerEvent(LD);
        BOOST_CHECK(checkLD(PVToTest[j]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(Mix) {
    unsigned int numberOfTries = 10;
    for (unsigned int i = 0; i < numberOfTries; i++) {
      mInPV = 0_UINT;
      triggerEvent(LD);
      BOOST_CHECK(checkLD(0));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(0));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(1));
      triggerEvent(CD);
      BOOST_CHECK(checkCD(2));
      triggerEvent(LD);
      BOOST_CHECK(checkLD(0));
      triggerEvent(R);
      BOOST_CHECK(checkR());

      mInPV = 1_UINT;
      triggerEvent(CU);
      BOOST_CHECK(checkCU(0));
      triggerEvent(LD);
      BOOST_CHECK(checkLD(1));
      triggerEvent(LD);
      BOOST_CHECK(checkLD(1));

      mInPV = 65533_UINT;
      triggerEvent(LD);
      BOOST_CHECK(checkLD(65533));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65533));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65534));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65535));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65535));
      triggerEvent(CD);
      BOOST_CHECK(checkCD(65535));
      triggerEvent(CD);
      BOOST_CHECK(checkCD(65534));
      triggerEvent(R);
      BOOST_CHECK(checkR());

      mInPV = 65533_UINT;
      for (unsigned int j = 0; j < 65533; j++) {
        // Send event
        triggerEvent(CU);
        BOOST_CHECK(checkCU(j));
      }
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65533));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65534));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65535));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65535));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65535));
      triggerEvent(CD);
      BOOST_CHECK(checkCD(65535));
      triggerEvent(CU);
      BOOST_CHECK(checkCU(65534));
      triggerEvent(R);
      BOOST_CHECK(checkR());
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
