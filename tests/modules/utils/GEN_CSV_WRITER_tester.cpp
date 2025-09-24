/*******************************************************************************
 * Copyright (c) 2022, 2025 Michael Hansal, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hansal - initial tests
 *   Alois Zoitl    - migrated data type toString to std::string
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"
#include <cstdio>
#include <fstream>
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

using namespace std::string_literals;

namespace forte::eclipse4diac::utils::test {
  template<typename T1, typename T2>
  class GEN_CSV_WRITER_TestFixture : public forte::test::CFBTestFixtureBase {
    public:
      GEN_CSV_WRITER_TestFixture() :
          CFBTestFixtureBase("eclipse4diac::utils::CSV_WRITER_2"_STRID),
          mSD_1(T1()),
          mSD_2(T2()) {
        setInputData({&mQI, &mFILE_NAME, &mSD_1, &mSD_2});
        setOutputData({&mQO, &mSTATUS});
        setup();

        mQI = true_BOOL;
        mFILE_NAME = CIEC_STRING("./GEN_CSV_WRITER.temp"s);

        triggerEvent(InputEventINIT);

        BOOST_REQUIRE(checkForSingleOutputEventOccurence(OutputEventINITO));
        BOOST_REQUIRE(mQO);
        mIsOpen = true;
      }

      ~GEN_CSV_WRITER_TestFixture() override {
        if (mIsOpen) {
          closeFile();
        }

        BOOST_CHECK_EQUAL(unlink(mFILE_NAME.getStorage().c_str()), 0);
      }

      CIEC_BOOL mQI;
      T1 mSD_1;
      T2 mSD_2;

      CIEC_BOOL mQO;
      CIEC_STRING mSTATUS;

      enum InputEvents { InputEventINIT, InputEventREQ };

      enum OutputEvents { OutputEventINITO, OutputEventCNF };

      void closeFile() {
        mQI = false_BOOL;

        triggerEvent(InputEventINIT);

        BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventINITO));
        BOOST_CHECK(!mQO);
        mIsOpen = false;
      }

      std::string getFileContent() {
        BOOST_CHECK(!mFILE_NAME.empty());
        BOOST_CHECK(!mIsOpen);

        std::stringstream buffer;
        buffer << std::ifstream(mFILE_NAME.getStorage().c_str()).rdbuf();
        return buffer.str();
      }

    private:
      CIEC_STRING mFILE_NAME;

      bool mIsOpen;
  };

  struct GEN_CSV_WRITER_TestFixtureIntString : public GEN_CSV_WRITER_TestFixture<CIEC_INT, CIEC_STRING> {};

  struct GEN_CSV_WRITER_TestFixtureArrayArray
      : public GEN_CSV_WRITER_TestFixture<CIEC_ARRAY_FIXED<CIEC_INT, 0, 2>, CIEC_ARRAY_FIXED<CIEC_STRING, 0, 2>> {};

  BOOST_AUTO_TEST_SUITE(GEN_CSV_WRITER_Tests)

  BOOST_FIXTURE_TEST_CASE(blockedWrite, GEN_CSV_WRITER_TestFixtureIntString) {
    mQI = false_BOOL;

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(!mQO);

    closeFile();
    BOOST_TEST(getFileContent() == "");
  }

  BOOST_FIXTURE_TEST_CASE(validWrite, GEN_CSV_WRITER_TestFixtureIntString) {
    mQI = true_BOOL;
    mSD_1 = CIEC_INT(1);
    mSD_2 = "Test"_STRING;

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(mQO);

    mQI = true_BOOL;
    mSD_1 = CIEC_INT(20);
    mSD_2 = "Test2"_STRING;

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(mQO);

    closeFile();
    BOOST_TEST(getFileContent() == "1; 'Test'; \n20; 'Test2'; \n");
  }

  BOOST_FIXTURE_TEST_CASE(arrayWrite, GEN_CSV_WRITER_TestFixtureArrayArray) {
    mQI = true_BOOL;
    mSD_1[0].setValue(CIEC_INT(1));
    mSD_1[1].setValue(CIEC_INT(2));
    mSD_1[2].setValue(CIEC_INT(3));
    mSD_2[0].setValue(CIEC_STRING("A"s));
    mSD_2[1].setValue(CIEC_STRING("B"s));
    mSD_2[2].setValue(CIEC_STRING("C"s));

    triggerEvent(InputEventREQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(mQO);

    closeFile();
    BOOST_CHECK_EQUAL(getFileContent(), "1; 'A'; \n2; 'B'; \n3; 'C'; \n");
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::eclipse4diac::utils::test
