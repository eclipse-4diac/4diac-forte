/*******************************************************************************
 * Copyright (c) 2013 - 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../src/core/stringdict.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "stringdicttests_gen.cpp"
#else
#include "stringlist.h"
#endif

#include <list>
#include <stdio.h>

#ifndef _MSC_VER //somehow required here, because visual studio gives a linker error
const CStringDictionary::TStringId CStringDictionary::scm_nInvalidStringId;
#endif

BOOST_AUTO_TEST_SUITE(StringDictTests)

  BOOST_AUTO_TEST_CASE(newString){
    //TEst new string if it is not in string dict and after inserting is inside and the correct string
    const std::string sTestString("FORTE_4DIAC_TestString");

    BOOST_CHECK_EQUAL(CStringDictionary::scm_nInvalidStringId, CStringDictionary::getInstance().getId(sTestString.c_str()));

    CStringDictionary::TStringId unNewID = CStringDictionary::getInstance().insert(sTestString.c_str());
    BOOST_CHECK(CStringDictionary::scm_nInvalidStringId != unNewID);

    BOOST_CHECK_EQUAL(unNewID, CStringDictionary::getInstance().getId(sTestString.c_str()));

    BOOST_CHECK_EQUAL(sTestString, CStringDictionary::getInstance().get(unNewID));
  }

  BOOST_AUTO_TEST_CASE(availableString){
    //test if a string that should be pre inserted in the stringdict on compile time is available
    std::string sBool(CStringDictionary::getInstance().get(g_nStringIdBOOL));
    BOOST_CHECK_EQUAL(sBool, "BOOL");

    BOOST_CHECK_EQUAL(g_nStringIdBOOL, CStringDictionary::getInstance().getId(sBool.c_str()));

  }

  void stringIdTest(CStringDictionary::TStringId pa_unId, const std::string &pa_rsExpectedString){
    std::string sTestString(CStringDictionary::getInstance().get(pa_unId));
    BOOST_CHECK_EQUAL(sTestString, pa_rsExpectedString);
    BOOST_CHECK_EQUAL(pa_unId, CStringDictionary::getInstance().getId(pa_rsExpectedString.c_str()));
  }

  BOOST_AUTO_TEST_CASE(reinsertAvailableString){
    //Test that a reinsert of a string does not corrupt the string dict
    BOOST_CHECK_EQUAL(g_nStringIdSTRING, CStringDictionary::getInstance().insert("STRING"));
  }

  BOOST_AUTO_TEST_CASE(availableLowerUpperCaseTest){
    //test if lower and upper case strings are correctly sorted by the build system and look up of them works
    stringIdTest(g_nStringIdlowercasetest, "lowercasetest");
    stringIdTest(g_nStringIdLOWERCASETEST, "LOWERCASETEST");
  }

  BOOST_AUTO_TEST_CASE(newLowerUpperCaseTest){
    //test if lower and upper case strings are correctly sorted when inserting at runtime and look up of them works
    stringIdTest(CStringDictionary::getInstance().insert("newteststring"), "newteststring");
    stringIdTest(CStringDictionary::getInstance().insert("NEWTESTSTRING"), "NEWTESTSTRING");
  }

    struct SBuffer{
        CStringDictionary::TStringId mID;
        std::string mString;
    };

  BOOST_AUTO_TEST_CASE(reallocateTest){
    //Test a large amount of strings so that a reallocation of the stringdict buffers is necessary

    std::list<SBuffer> stringList;


    for(unsigned int i = 0; i < 1000; i++){
      SBuffer buffer;
      char newStringDictEntry[40];
  #ifdef WIN32
      _snprintf(
  #else
      snprintf(
  #endif
          newStringDictEntry, 40, "LargeStringTestStringNumber%ud", i);

      buffer.mString = newStringDictEntry;
      BOOST_CHECK_EQUAL(CStringDictionary::scm_nInvalidStringId, CStringDictionary::getInstance().getId(newStringDictEntry));
      buffer.mID = CStringDictionary::getInstance().insert(newStringDictEntry);

      stringList.push_back(buffer);

      //Check that every entry is still in the stringdict
      for(std::list<SBuffer>::iterator itRunner = stringList.begin(); itRunner != stringList.end(); ++itRunner){
        BOOST_CHECK_EQUAL(itRunner->mID, CStringDictionary::getInstance().getId(itRunner->mString.c_str()));
        BOOST_CHECK_EQUAL(itRunner->mString, CStringDictionary::getInstance().get(itRunner->mID));
      }
    }

  }

BOOST_AUTO_TEST_SUITE_END()
