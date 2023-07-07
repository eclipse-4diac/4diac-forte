/*******************************************************************************
 * Copyright (c) 2022 Michael Hansal
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hansal - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_CSV_WRITER_tester_gen.cpp"
#endif

#include <cstdio>
#include <fstream>

template<typename T1, typename T2>
class GEN_CSV_WRITER_TestFixture : public CFBTestFixtureBase {
  public:
    GEN_CSV_WRITER_TestFixture() : CFBTestFixtureBase(g_nStringIdGEN_CSV_WRITER) {
      setInputData({&mQI, &mFILE_NAME, &mSD_1, &mSD_2});
      setOutputData({&mQO, &mSTATUS});
      CFBTestFixtureBase::setup("CSV_WRITER_2");

      char tempName[L_tmpnam];
      BOOST_CHECK(std::tmpnam(tempName));

      mQI = CIEC_BOOL(true);
      mFILE_NAME = CIEC_STRING(tempName, strlen(tempName));

      triggerEvent(InputEventINIT);

      BOOST_REQUIRE(checkForSingleOutputEventOccurence(OutputEventINITO));
      BOOST_REQUIRE(mQO);
      mIsOpen = true;
    }

    ~GEN_CSV_WRITER_TestFixture() {
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

    enum InputEvents {
      InputEventINIT,
      InputEventREQ
    };

    enum OutputEvents {
      OutputEventINITO,
      OutputEventCNF
    };

    void closeFile() {
      mQI = CIEC_BOOL(false);

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

struct GEN_CSV_WRITER_TestFixtureIntString :
        public GEN_CSV_WRITER_TestFixture<CIEC_INT, CIEC_STRING> {
};

BOOST_AUTO_TEST_SUITE(GEN_CSV_WRITER_Tests)

  BOOST_FIXTURE_TEST_CASE(blockedWrite, GEN_CSV_WRITER_TestFixtureIntString) {
    mQI = CIEC_BOOL(false);

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(!mQO);

    closeFile();
    BOOST_TEST(getFileContent() == "");
  }

  BOOST_FIXTURE_TEST_CASE(validWrite, GEN_CSV_WRITER_TestFixtureIntString) {
    mQI = CIEC_BOOL(true);
    mSD_1 = CIEC_INT(1);
    mSD_2 = "Test"_STRING;

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(mQO);

    mQI = CIEC_BOOL(true);
    mSD_1 = CIEC_INT(20);
    mSD_2 = "Test2"_STRING;

    triggerEvent(InputEventREQ);

    BOOST_CHECK(checkForSingleOutputEventOccurence(OutputEventCNF));
    BOOST_CHECK(mQO);

    closeFile();
    BOOST_TEST(getFileContent() == "1; 'Test'; \n20; 'Test2'; \n");
  }

BOOST_AUTO_TEST_SUITE_END()
