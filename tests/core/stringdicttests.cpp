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
const CStringDictionary::TStringId CStringDictionary::scmInvalidStringId;
#endif

BOOST_AUTO_TEST_SUITE(StringDictTests)

  BOOST_AUTO_TEST_CASE(newString){
    //TEst new string if it is not in string dict and after inserting is inside and the correct string
    const std::string sTestString("FORTE_4DIAC_TestString");

    BOOST_CHECK_EQUAL(CStringDictionary::scmInvalidStringId, CStringDictionary::getId(sTestString.c_str()));
    BOOST_CHECK_EQUAL(CStringDictionary::scmInvalidStringId, CStringDictionary::getId(sTestString.c_str(), sTestString.size()));

    CStringDictionary::TStringId unNewID = CStringDictionary::insert(sTestString.c_str());
    BOOST_CHECK(CStringDictionary::scmInvalidStringId != unNewID);

    BOOST_CHECK_EQUAL(unNewID, CStringDictionary::getId(sTestString.c_str()));
    BOOST_CHECK_EQUAL(unNewID, CStringDictionary::getId(sTestString.c_str(), sTestString.size()));

    BOOST_CHECK_EQUAL(sTestString, CStringDictionary::get(unNewID));
  }

  BOOST_AUTO_TEST_CASE(availableString){
    //test if a string that should be pre inserted in the stringdict on compile time is available
    std::string sBool(CStringDictionary::get(g_nStringIdBOOL));
    BOOST_CHECK_EQUAL(sBool, "BOOL");

    BOOST_CHECK_EQUAL(g_nStringIdBOOL, CStringDictionary::getId(sBool.c_str()));
    BOOST_CHECK_EQUAL(g_nStringIdBOOL, CStringDictionary::getId(sBool.c_str(), sBool.size()));

  }

  void stringIdTest(CStringDictionary::TStringId paId, const std::string &paExpectedString){
    std::string sTestString(CStringDictionary::get(paId));
    BOOST_CHECK_EQUAL(sTestString, paExpectedString);
    BOOST_CHECK_EQUAL(paId, CStringDictionary::getId(paExpectedString.c_str()));
    BOOST_CHECK_EQUAL(paId, CStringDictionary::getId(paExpectedString.c_str(), paExpectedString.size()));
  }

  BOOST_AUTO_TEST_CASE(reinsertAvailableString){
    //Test that a reinsert of a string does not corrupt the string dict
    BOOST_CHECK_EQUAL(g_nStringIdSTRING, CStringDictionary::insert("STRING"));
  }

  BOOST_AUTO_TEST_CASE(availableLowerUpperCaseTest){
    //test if lower and upper case strings are correctly sorted by the build system and look up of them works
    stringIdTest(g_nStringIdlowercasetest, "lowercasetest");
    stringIdTest(g_nStringIdLOWERCASETEST, "LOWERCASETEST");
  }

  BOOST_AUTO_TEST_CASE(newLowerUpperCaseTest){
    //test if lower and upper case strings are correctly sorted when inserting at runtime and look up of them works
    stringIdTest(CStringDictionary::insert("newteststring"), "newteststring");
    stringIdTest(CStringDictionary::insert("NEWTESTSTRING"), "NEWTESTSTRING");
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
      BOOST_CHECK_EQUAL(CStringDictionary::scmInvalidStringId, CStringDictionary::getId(newStringDictEntry));
      buffer.mID = CStringDictionary::insert(newStringDictEntry);

      stringList.push_back(buffer);

      //Check that every entry is still in the stringdict
      for(std::list<SBuffer>::iterator itRunner = stringList.begin(); itRunner != stringList.end(); ++itRunner){
        BOOST_CHECK_EQUAL(itRunner->mID, CStringDictionary::getId(itRunner->mString.c_str()));
        BOOST_CHECK_EQUAL(itRunner->mID, CStringDictionary::getId(itRunner->mString.c_str(), itRunner->mString.size()));
        BOOST_CHECK_EQUAL(itRunner->mString, CStringDictionary::get(itRunner->mID));
      }
    }

  }

BOOST_AUTO_TEST_SUITE_END()
