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
      SETUP_INPUTDATA(&mVar1, &mVar2, &mVar3);
      SETUP_OUTPUTDATA(&mOut);
      CFBTestFixtureBase::setup();
    }

    CIEC_INT    mVar1;
    CIEC_INT    mVar2;
    CIEC_STRING mVar3;

    CIEC_Struct_Muxer_Test_Struct_1 mOut;

    void checkStructValues(){
      BOOST_CHECK_EQUAL(mVar1.operator TForteInt16(), mOut.Var1());
      BOOST_CHECK_EQUAL(mVar2.operator TForteInt16(), mOut.Var2());
      BOOST_CHECK_EQUAL(strcmp(mVar3.getValue(), mOut.Var3().getValue()), 0);
    }
};

BOOST_FIXTURE_TEST_SUITE( STRUCT_MUX_MainTests, STRUCT_MUX_TestFixture_1)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mVar1 = -256;
    mVar2 = 23145;
    mVar3 = "My Test String!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(updateValueCheck) {
    mVar1 = 12;
    mVar2 = -11111;
    mVar3 = "string!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();

    //change values to check consecutive updates

    mVar1 = 32255;
    mVar2 = 12345;
    mVar3 = "new string!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(steadyStateValueCheck) {
    mVar1 = 13;
    mVar2 = 234;
    mVar3 = "stable value";

    for(size_t i = 0; i < 45; i++){
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      checkStructValues();
    }
  }

}

struct STRUCT_MUX_TestFixture_2 : public CFBTestFixtureBase{

    STRUCT_MUX_TestFixture_2() : CFBTestFixtureBase(g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_2){
      SETUP_INPUTDATA(&mVar1, &mVar2, &mVar3);
      SETUP_OUTPUTDATA(&mOut);
      CFBTestFixtureBase::setup();
    }

    CIEC_INT    mVar1;
    CIEC_STRING  mVar2;
    CIEC_INT     mVar3;

    CIEC_Struct_Muxer_Test_Struct_2 mOut;

    void checkStructValues(){
     BOOST_CHECK_EQUAL(mVar1.operator TForteInt16(), mOut.Var1());
     BOOST_CHECK_EQUAL(strcmp(mVar2.getValue(), mOut.Var2().getValue()), 0);
     BOOST_CHECK_EQUAL(mVar3.operator TForteInt16(), mOut.Var3());
   }

};

BOOST_FIXTURE_TEST_SUITE( STRUCT_MUX_SecondStructTest, STRUCT_MUX_TestFixture_2)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mVar1 = 1234;
    mVar2 = "this is the second struct!";
    mVar3 = -2345;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

}

BOOST_AUTO_TEST_SUITE(STRUCT_MUX_FailedCreationTest)

  BOOST_AUTO_TEST_CASE(missingDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1UnknownType, 0);
    BOOST_CHECK(0 == fb);
  }

  BOOST_AUTO_TEST_CASE(wrongDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1STRING, 0);
    BOOST_CHECK(0 == fb);

    fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1REAL, 0);
    BOOST_CHECK(0 == fb);
  }

BOOST_AUTO_TEST_CASE(emptyStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_3, 0);
BOOST_CHECK(0 == fb);
}

BOOST_AUTO_TEST_CASE(bigStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_MUX_1Struct_Muxer_Test_Struct_4, 0);
BOOST_CHECK(0 == fb);
}

}

