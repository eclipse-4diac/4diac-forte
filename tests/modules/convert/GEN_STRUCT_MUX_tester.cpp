/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_STRUCT_MUX_tester_gen.cpp"
#endif

#include "struct_mux_demux_data.h"


struct STRUCT_MUX_TestFixture_1 : public CFBTestFixtureBase{

    STRUCT_MUX_TestFixture_1() : CFBTestFixtureBase(g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_1){
      setInputData({&mVar1, &mVar2, &mVar3});
      setOutputData({&mOut});
      CFBTestFixtureBase::setup();
    }

    CIEC_INT mVar1;
    CIEC_INT mVar2;
    CIEC_STRING mVar3;

    CIEC_Struct_Muxer_Test_Struct_1 mOut;

    void checkStructValues(){
      BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mOut.Var1));
      BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar2), static_cast<CIEC_INT::TValueType>(mOut.Var2));
      BOOST_TEST(mVar3 == mOut.Var3);
    }
};

BOOST_FIXTURE_TEST_SUITE( STRUCT_MUX_MainTests, STRUCT_MUX_TestFixture_1)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mVar1 = CIEC_INT(-256);
    mVar2 = CIEC_INT(23145);
    mVar3 = "My Test String!"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(updateValueCheck) {
    mVar1 = CIEC_INT(12);
    mVar2 = CIEC_INT(-11111);
    mVar3 = "string!"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();

    //change values to check consecutive updates

    mVar1 = CIEC_INT(32255);
    mVar2 = CIEC_INT(12345);
    mVar3 = "new string!"_STRING;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(steadyStateValueCheck) {
    mVar1 = CIEC_INT(13);
    mVar2 = CIEC_INT(234);
    mVar3 = "stable value"_STRING;

    for(size_t i = 0; i < 45; i++){
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      checkStructValues();
    }
  }

BOOST_AUTO_TEST_SUITE_END()

struct STRUCT_MUX_TestFixture_2 : public CFBTestFixtureBase{

    STRUCT_MUX_TestFixture_2() : CFBTestFixtureBase(g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_2){
      setInputData({&mVar1, &mVar2, &mVar3});
      setOutputData({&mOut});
      CFBTestFixtureBase::setup();
    }

    CIEC_INT    mVar1;
    CIEC_STRING  mVar2;
    CIEC_INT     mVar3;

    CIEC_Struct_Muxer_Test_Struct_2 mOut;

    void checkStructValues(){
     BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mOut.Var1));
     BOOST_TEST(mVar2 == mOut.Var2);
     BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar3), static_cast<CIEC_INT::TValueType>(mOut.Var3));
   }

};

BOOST_FIXTURE_TEST_SUITE( STRUCT_MUX_SecondStructTest, STRUCT_MUX_TestFixture_2)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mVar1 = CIEC_INT(1234);
    mVar2 = "this is the second struct!"_STRING;
    mVar3 = CIEC_INT(-2345);
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(STRUCT_MUX_FailedCreationTest)

  BOOST_AUTO_TEST_CASE(missingDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1UnknownType, nullptr);
    BOOST_CHECK(nullptr == fb);
  }

  BOOST_AUTO_TEST_CASE(wrongDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1STRING, nullptr);
    BOOST_CHECK(nullptr == fb);

    fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1REAL, nullptr);
    BOOST_CHECK(nullptr == fb);
  }

BOOST_AUTO_TEST_CASE(emptyStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_3, nullptr);
BOOST_CHECK(nullptr == fb);
}

BOOST_AUTO_TEST_CASE(bigStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_4, nullptr);
BOOST_CHECK(nullptr != fb);
delete fb;
}

BOOST_AUTO_TEST_SUITE_END()

struct STRUCT_MUX_TestFixture_5 : public CFBTestFixtureBase {

  STRUCT_MUX_TestFixture_5() : CFBTestFixtureBase(g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_5) {
    setInputData({&mVar1, &mVar2, &mVar3});
    setOutputData({&mOut});
    CFBTestFixtureBase::setup();
  }

  CIEC_INT mVar1;
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 3> mVar2;
  CIEC_ARRAY_FIXED<CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>, 0, 1> mVar3;

  CIEC_Struct_Muxer_Test_Struct_5 mOut;

  void checkStructValues() {
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mOut.Var1));
    for(int i = 0; i <= 3; ++i) {
      BOOST_TEST(static_cast<CIEC_INT::TValueType>(mVar2[i]) == static_cast<CIEC_INT::TValueType>(mOut.Var2[i]));
    }
    for(int j = 0; j <= 1; ++j) {
      for(int i = 0; i <= 3; ++i) {
        BOOST_TEST(static_cast<CIEC_INT::TValueType>(mVar3[j][i]) == static_cast<CIEC_INT::TValueType>(mOut.Var3[j][i]));
      }
    }
  }
};

BOOST_FIXTURE_TEST_SUITE(STRUCT_MUX_ArrayStructTest, STRUCT_MUX_TestFixture_5)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mVar1 = CIEC_INT(1234);
    mVar2 = CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{CIEC_INT(17), CIEC_INT(4), CIEC_INT(21), CIEC_INT(42)};
    mVar3 = CIEC_ARRAY_FIXED<CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>, 0, 1>{
            CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{CIEC_INT(17), CIEC_INT(4), CIEC_INT(21), CIEC_INT(42)},
            CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4)}};
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

BOOST_AUTO_TEST_SUITE_END()

