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

using namespace forte::literals;

#include "struct_mux_demux_data.h"
#include "fbcontainermock.h"
#include "../../core/src/typelib_internal.h"

namespace forte::eclipse4diac::convert::test {
  struct STRUCT_DEMUX_TestFixture_1 : public forte::test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      STRUCT_DEMUX_TestFixture_1() :
          CFBTestFixtureBase("eclipse4diac::convert::STRUCT_DEMUX_1Struct_Muxer_Test_Struct_1"_STRID) {
        setInputData({&mIn});
        setOutputData({&mVar1, &mVar2, &mVar3});
        setup();
      }

      CIEC_INT mVar1;
      CIEC_INT mVar2;
      CIEC_STRING mVar3;

      CIEC_Struct_Muxer_Test_Struct_1 mIn;

      void checkStructValues() {
        BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mIn.Var1));
        BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar2), static_cast<CIEC_INT::TValueType>(mIn.Var2));
        BOOST_TEST(mVar3 == mIn.Var3);
      }
  };

  BOOST_FIXTURE_TEST_SUITE(STRUCT_DEMUX_MainTests, STRUCT_DEMUX_TestFixture_1)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mIn.Var1 = -256_INT;
    mIn.Var2 = 23145_INT;
    mIn.Var3 = "My Test String!"_STRING;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(updateValueCheck) {
    mIn.Var1 = 12_INT;
    mIn.Var2 = 11111_INT;
    mIn.Var3 = "string!"_STRING;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();

    // change values to check consecutive updates

    mIn.Var1 = 32255_INT;
    mIn.Var2 = 12345_INT;
    mIn.Var3 = "new string!"_STRING;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(steadyStateValueCheck) {
    mIn.Var1 = 13_INT;
    mIn.Var2 = 234_INT;
    mIn.Var3 = "stable value"_STRING;

    for (size_t i = 0; i < 45; i++) {
      triggerEvent(REQ);
      BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
      checkStructValues();
    }
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct STRUCT_DEMUX_TestFixture_2 : public forte::test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      STRUCT_DEMUX_TestFixture_2() :
          CFBTestFixtureBase("eclipse4diac::convert::STRUCT_DEMUX_1Struct_Muxer_Test_Struct_2"_STRID) {
        setInputData({&mIn});
        setOutputData({&mVar1, &mVar2, &mVar3});
        setup();
      }

      CIEC_INT mVar1;
      CIEC_STRING mVar2;
      CIEC_INT mVar3;

      CIEC_Struct_Muxer_Test_Struct_2 mIn;

      void checkStructValues() {
        BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mIn.Var1));
        BOOST_TEST(mVar2 == mIn.Var2);
        BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar3), static_cast<CIEC_INT::TValueType>(mIn.Var3));
      }
  };

  BOOST_FIXTURE_TEST_SUITE(STRUCT_DEMUX_SecondStructTest, STRUCT_DEMUX_TestFixture_2)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mIn.Var1 = 1234_INT;
    mIn.Var2 = "this is the second struct!"_STRING;
    mIn.Var3 = -2345_INT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(STRUCT_DEMUX_FailedCreationTest)

  BOOST_AUTO_TEST_CASE(missingDataType) {
    CFunctionBlock *fb = createFB("InstanceName"_STRID, "eclipse4diac::convert::STRUCT_DEMUX_1UnknownType"_STRID,
                                  forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_CHECK(nullptr == fb);
  }

  BOOST_AUTO_TEST_CASE(wrongDataType) {
    CFunctionBlock *fb = createFB("InstanceName"_STRID, "eclipse4diac::convert::STRUCT_DEMUX_1STRING"_STRID,
                                  forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_CHECK(nullptr == fb);

    fb = createFB("InstanceName"_STRID, "eclipse4diac::convert::STRUCT_DEMUX_1REAL"_STRID,
                  forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_CHECK(nullptr == fb);
  }

  BOOST_AUTO_TEST_CASE(emptyStruct) {
    CFunctionBlock *fb =
        createFB("InstanceName"_STRID, "eclipse4diac::convert::STRUCT_DEMUX_1Struct_Muxer_Test_Struct_3"_STRID,
                 forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_CHECK(nullptr == fb);
  }

  BOOST_AUTO_TEST_CASE(bigStruct) {
    CFunctionBlock *fb =
        createFB("InstanceName"_STRID, "eclipse4diac::convert::STRUCT_DEMUX_1Struct_Muxer_Test_Struct_4"_STRID,
                 forte::test::CFBContainerMock::smDefaultFBContMock);
    BOOST_CHECK(nullptr != fb);
    delete fb;
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct STRUCT_DEMUX_TestFixture_5 : public forte::test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      STRUCT_DEMUX_TestFixture_5() :
          CFBTestFixtureBase("eclipse4diac::convert::STRUCT_DEMUX_1Struct_Muxer_Test_Struct_5"_STRID) {
        setInputData({&mIn});
        setOutputData({&mVar1, &mVar2, &mVar3});
        setup();
      }

      CIEC_INT mVar1;
      CIEC_ARRAY_FIXED<CIEC_INT, 0, 3> mVar2;
      CIEC_ARRAY_FIXED<CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>, 0, 1> mVar3;

      CIEC_Struct_Muxer_Test_Struct_5 mIn;

      void checkStructValues() {
        BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(mVar1), static_cast<CIEC_INT::TValueType>(mIn.Var1));
        for (int i = 0; i <= 3; ++i) {
          BOOST_TEST(static_cast<CIEC_INT::TValueType>(mVar2[i]) == static_cast<CIEC_INT::TValueType>(mIn.Var2[i]));
        }
        for (int j = 0; j <= 1; ++j) {
          for (int i = 0; i <= 3; ++i) {
            BOOST_TEST(static_cast<CIEC_INT::TValueType>(mVar3[j][i]) ==
                       static_cast<CIEC_INT::TValueType>(mIn.Var3[j][i]));
          }
        }
      }
  };

  BOOST_FIXTURE_TEST_SUITE(STRUCT_DEMUX_ArrayStructTest, STRUCT_DEMUX_TestFixture_5)

  BOOST_AUTO_TEST_CASE(initalValueCheck) {
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_CASE(changeValueCheck) {
    mIn.Var1 = 1234_INT;
    mIn.Var2 = CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{17_INT, 4_INT, 21_INT, 42_INT};
    mIn.Var3 = CIEC_ARRAY_FIXED<CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>, 0, 1>{
        CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{17_INT, 4_INT, 21_INT, 42_INT},
        CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>{1_INT, 2_INT, 3_INT, 4_INT}};
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    checkStructValues();
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::eclipse4diac::convert::test
