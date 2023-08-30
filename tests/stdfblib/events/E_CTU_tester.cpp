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
#include <forte_uint.h>
#include <forte_bool.h>

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTU_tester_gen.cpp"
#endif

struct E_CTU_TestFixture : public CFBTestFixtureBase {

    E_CTU_TestFixture() :
        CFBTestFixtureBase(g_nStringIdE_CTU) {
      setInputData({&mInPV});
      setOutputData({&mOutQ, &mOutCV});
      CFBTestFixtureBase::setup();
    }

    CIEC_UINT mInPV; //Preset value
    CIEC_BOOL mOutQ; //CV >= PV
    CIEC_UINT mOutCV; //count value

    bool checkCU(TForteUInt16 paPrevCV) {
      if(paPrevCV < CIEC_UINT::scmMaxVal) {
        if(func_NE(CIEC_UINT(paPrevCV + 1), mOutCV)) {
          return false;
        } else if(!checkForSingleOutputEventOccurence(0)) {
          return false;
        }
      } else {
        if(func_AND(func_NE(CIEC_UINT(CIEC_UINT::scmMaxVal), mOutCV), func_NOT(CIEC_BOOL(eventChainEmpty())))) {
          return false;
        }
      }
      if(func_NE(mOutQ, func_GE(mOutCV, mInPV))) {
        return false;
      }
      return true;
    }

    bool checkR() {
      if(func_OR(func_NE(CIEC_UINT(0), mOutCV), mOutQ)) {
        return false;
      }
      if(!checkForSingleOutputEventOccurence(1)) {
        return false;
      }
      return true;
    }
};

BOOST_FIXTURE_TEST_SUITE( CTUTests, E_CTU_TestFixture)

BOOST_AUTO_TEST_CASE(EventCU) {
  TForteUInt16 prevCV = 0;
  TForteUInt16 valuesToTest[] = {10, 1, 0, 65534, 65535};
  size_t numberOfValues = static_cast<size_t>(sizeof(valuesToTest) / sizeof(TForteUInt16));
  for(size_t j = 0; j < numberOfValues; j++) {
    triggerEvent(1);
    BOOST_CHECK(checkR());
    mInPV = CIEC_UINT(valuesToTest[j]);
    for (unsigned int k = 0U; k < static_cast<CIEC_UINT::TValueType>(mInPV) + 3U; k++) {
        prevCV = static_cast<CIEC_UINT::TValueType>(mOutCV);
        // Send event
        triggerEvent(0);
        BOOST_CHECK(checkCU(prevCV));
    }
  }
}

BOOST_AUTO_TEST_CASE(EventR) {
  size_t numberOfTries = 100;
  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
  size_t numberOfValues = static_cast<size_t>(sizeof(valuesToTest) / sizeof(TForteUInt16));
  for(size_t i = 0; i < numberOfTries; i++) {
    for(size_t j = 0; j < numberOfValues; j++) {
      mInPV = CIEC_UINT(valuesToTest[j]);
      triggerEvent(1);
      BOOST_CHECK(checkR());
    }
  }
}

BOOST_AUTO_TEST_CASE(Mix) {
  mInPV = CIEC_UINT(0);
  triggerEvent(1);
  BOOST_CHECK (checkR());

triggerEvent(0);
BOOST_CHECK(checkCU(0));

mInPV = CIEC_UINT(1);
triggerEvent(0);
BOOST_CHECK(checkCU(1));

triggerEvent(1);
BOOST_CHECK (checkR());

triggerEvent(0);
BOOST_CHECK(checkCU(0));
triggerEvent(0);
BOOST_CHECK(checkCU(1));

triggerEvent(0);
BOOST_CHECK(checkCU(2));

triggerEvent(1);
BOOST_CHECK (checkR());

mInPV = CIEC_UINT(65533);
for(TForteUInt16 i = 0; i < CIEC_UINT::scmMaxVal; i++) {
  //Send event
  triggerEvent(0);
  BOOST_CHECK(checkCU(i));
}

triggerEvent(0);
BOOST_CHECK (checkCU(CIEC_UINT::scmMaxVal));

triggerEvent(0);
BOOST_CHECK (checkCU(CIEC_UINT::scmMaxVal));

triggerEvent(0);
BOOST_CHECK (checkCU(CIEC_UINT::scmMaxVal));

triggerEvent(1);
BOOST_CHECK (checkR());}

BOOST_AUTO_TEST_SUITE_END()
