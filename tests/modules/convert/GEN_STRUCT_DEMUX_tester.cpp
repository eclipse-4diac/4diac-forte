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
#include "GEN_STRUCT_DEMUX_tester_gen.cpp"
#endif

#include "struct_mux_demux_data.h"


struct STRUCT_DEMUX_TestFixture_1 : public CFBTestFixtureBase{

    STRUCT_DEMUX_TestFixture_1() : CFBTestFixtureBase(g_nStringIdSTRUCT_DEMUX_1Struct_Muxer_Test_Struct_1){
      SETUP_INPUTDATA(&mIn);
      SETUP_OUTPUTDATA(&mVar1, &mVar2, &mVar3);
      CFBTestFixtureBase::setup();
    }

    CIEC_INT    mVar1;
    CIEC_INT   mVar2;
    CIEC_STRING mVar3;

    CIEC_Struct_Muxer_Test_Struct_1 mIn;

    void checkStructValues(){
      BOOST_CHECK_EQUAL(mVar1, mIn.Var1());
      BOOST_CHECK_EQUAL(mVar2, mIn.Var2());
      BOOST_CHECK_EQUAL(strcmp(mVar3.getValue(), mIn.Var3().getValue()), 0);
    }
};

BOOST_FIXTURE_TEST_SUITE( STRUCT_DEMUX_MainTests, STRUCT_DEMUX_TestFixture_1)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mIn.Var1() = -256;
    mIn.Var2() = 23145;
    mIn.Var3() = "My Test String!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(updateValueCheck) {
    mIn.Var1() = 12;
    mIn.Var2() = 11111;
    mIn.Var3() = "string!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();

    //change values to check consecutive updates

    mIn.Var1() = 32255;
    mIn.Var2() = 12345;
    mIn.Var3() = "new string!";
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(steadyStateValueCheck) {
    mIn.Var1() = 13;
    mIn.Var2() = 234;
    mIn.Var3() = "stable value";

    for(size_t i = 0; i < 45; i++){
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      checkStructValues();
    }
  }

}

struct STRUCT_DEMUX_TestFixture_2 : public CFBTestFixtureBase{

    STRUCT_DEMUX_TestFixture_2() : CFBTestFixtureBase(g_nStringIdSTRUCT_DEMUX_1Struct_Muxer_Test_Struct_2){
      SETUP_INPUTDATA(&mIn);
      SETUP_OUTPUTDATA(&mVar1, &mVar2, &mVar3);
      CFBTestFixtureBase::setup();
    }

    CIEC_INT    mVar1;
    CIEC_STRING  mVar2;
    CIEC_INT     mVar3;

    CIEC_Struct_Muxer_Test_Struct_2 mIn;

    void checkStructValues(){
     BOOST_CHECK_EQUAL(mVar1, mIn.Var1());
     BOOST_CHECK_EQUAL(strcmp(mVar2.getValue(), mIn.Var2().getValue()), 0);
     BOOST_CHECK_EQUAL(mVar3, mIn.Var3());
   }

};

BOOST_FIXTURE_TEST_SUITE( STRUCT_DEMUX_SecondStructTest, STRUCT_DEMUX_TestFixture_2)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mIn.Var1() = 1234;
    mIn.Var2() = "this is the second struct!";
    mIn.Var3() = -2345;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    checkStructValues();
  }

}

BOOST_AUTO_TEST_SUITE(STRUCT_DEMUX_FailedCreationTest)

  BOOST_AUTO_TEST_CASE(missingDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_DEMUX_1UnknownType, 0);
    BOOST_CHECK(0 == fb);
  }

  BOOST_AUTO_TEST_CASE(wrongDataType){
    CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_DEMUX_1STRING, 0);
    BOOST_CHECK(0 == fb);

    fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_DEMUX_1REAL, 0);
    BOOST_CHECK(0 == fb);
  }

BOOST_AUTO_TEST_CASE(emptyStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_DEMUX_1Struct_Muxer_Test_Struct_3, 0);
BOOST_CHECK(0 == fb);
}

BOOST_AUTO_TEST_CASE(bigStruct) {
CFunctionBlock *fb = CTypeLib::createFB(g_nStringIdInstanceName, g_nStringIdSTRUCT_DEMUX_1Struct_Muxer_Test_Struct_4, 0);
BOOST_CHECK(0 == fb);
}

}

