/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 *               2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial tests
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GET_STRUCT_VALUE_tester_gen.cpp"
#endif

class CIEC_GET_STRUCT_VALUE_Struct_test1 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test1)

    /* Struct:
     *   Val1 : String[2]
     *   Val2 : INT
     */

  public:
    CIEC_ARRAY_FIXED<CIEC_STRING, 0, 1> Var1;
    CIEC_INT Var2;

    CIEC_GET_STRUCT_VALUE_Struct_test1() : Var1(), Var2(0) {}

    size_t getStructSize() const override {
      return 2;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override {
      return g_nStringIdGET_STRUCT_VALUE_Struct_test1;
    }

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
      }
      return nullptr;
    }

    static const unsigned int sizeOfFirstArray = 2;

  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test1::scmElementNames[] = { g_nStringIdVal1, g_nStringIdVal2 };

DEFINE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test1, g_nStringIdGET_STRUCT_VALUE_Struct_test1)

class CIEC_GET_STRUCT_VALUE_Struct_test2 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test2)

    /* Struct:
     *   Val1 : INT
     *   Val2 : CIEC_GET_STRUCT_VALUE_Struct_test1
     */

  public:
    CIEC_INT Var1;
    CIEC_GET_STRUCT_VALUE_Struct_test1 Var2;

    CIEC_GET_STRUCT_VALUE_Struct_test2() = default;

    size_t getStructSize() const override {
      return 2;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override {
      return g_nStringIdGET_STRUCT_VALUE_Struct_test2;
    }

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
      }
      return nullptr;
    }

private:
    static const CStringDictionary::TStringId scmElementNames[];
};

const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test2::scmElementNames[] = { g_nStringIdVal1, g_nStringIdVal2 };

DEFINE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test2, g_nStringIdGET_STRUCT_VALUE_Struct_test2)



struct GET_STRUCT_VALUE_GenericTestFixture : public CFBTestFixtureBase{

    GET_STRUCT_VALUE_GenericTestFixture(CIEC_ANY *paIN_STRUCT, CIEC_ANY *paOUT) :
        CFBTestFixtureBase(g_nStringIdGET_STRUCT_VALUE){
      setInputData({paIN_STRUCT, &mMember});
      setOutputData({&mQO, paOUT});
    }

    CIEC_STRING mMember;
    CIEC_BOOL mQO;
};

struct GET_STRUCT_VALUE_Main_TestFixture : public GET_STRUCT_VALUE_GenericTestFixture{

    GET_STRUCT_VALUE_Main_TestFixture() :
        GET_STRUCT_VALUE_GenericTestFixture(&mIn_struct, &mOut){
      mIn_struct.fromString("(Val1:=1,Val2:=(Val1:=['strin1','string2',''], Val2:=2))");
      CFBTestFixtureBase::setup();
    }

    CIEC_GET_STRUCT_VALUE_Struct_test2 mIn_struct;
    CIEC_INT mOut;
};

BOOST_FIXTURE_TEST_SUITE( GET_STRUCT_VALUE_MainTests, GET_STRUCT_VALUE_Main_TestFixture)

  BOOST_AUTO_TEST_CASE(firstLevel) {
    mMember = "Val1"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(true, mQO);
    BOOST_CHECK_EQUAL(1, static_cast<CIEC_INT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(secondLevel) {
    mMember = "Val2.Val2"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(true, mQO);
    BOOST_CHECK_EQUAL(2, static_cast<CIEC_INT::TValueType>(mOut));
  }


  BOOST_AUTO_TEST_CASE(firstLevelWrongName) {
    mMember = "xVal1"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false, mQO);
  }

  BOOST_AUTO_TEST_CASE(firstLevelWrongNameWithSecondLevel) {
    mMember = "xVal1.Val2"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false, mQO);
  }

  BOOST_AUTO_TEST_CASE(secondLevelWrongName) {
    mMember = "Val2.xVal2"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false, mQO);
  }

  BOOST_AUTO_TEST_CASE(accessNonStruct) {
    mMember = "Val1.Val1"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false, mQO);
  }

  BOOST_AUTO_TEST_CASE(WrongEventInput) {
    mMember = "Val2.Val1"_STRING;
    triggerEvent(1);
    BOOST_CHECK(eventChainEmpty());
  }

BOOST_AUTO_TEST_SUITE_END()


struct GET_STRUCT_VALUE_WRONG_OUTPUT_TYPE_TestFixture : public GET_STRUCT_VALUE_GenericTestFixture{

    GET_STRUCT_VALUE_WRONG_OUTPUT_TYPE_TestFixture() :
        GET_STRUCT_VALUE_GenericTestFixture(&mIn_struct, &mOut){
      mIn_struct = CIEC_INT(1);
      CFBTestFixtureBase::setup();
    }

    CIEC_INT mIn_struct;
    CIEC_INT mOut;
};

BOOST_FIXTURE_TEST_SUITE( GET_STRUCT_VALUE_WRONG_OUTPUT_TYPETests, GET_STRUCT_VALUE_WRONG_OUTPUT_TYPE_TestFixture)

  BOOST_AUTO_TEST_CASE(wrongInputType) {
    mMember = "Val1"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false, mQO);
  }

BOOST_AUTO_TEST_SUITE_END()

