/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_array_fixed.h"

using namespace forte::literals;

namespace forte::iec61131::conversion {

  struct F_FROM_BIG_ENDIAN_LWORD_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_LWORD_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_LWORD mIn;
      CIEC_LWORD mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_LWORD_Tests, F_FROM_BIG_ENDIAN_LWORD_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_lword_1) {
    mIn = CIEC_LWORD(72057594037927936ULL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(1ULL, static_cast<CIEC_LWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_lword_2) {
    mIn = CIEC_LWORD(144115188075855872ULL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(2ULL, static_cast<CIEC_LWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_lword_3) {
    mIn = CIEC_LWORD(216172782113783808ULL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(3ULL, static_cast<CIEC_LWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_lword_max) {
    mIn = CIEC_LWORD(0xFFFFFFFFFFFFFFFFULL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(0xFFFFFFFFFFFFFFFFULL, static_cast<CIEC_LWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_WORD_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_WORD_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_WORD mIn;
      CIEC_WORD mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_WORD_Tests, F_FROM_BIG_ENDIAN_WORD_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_word_1) {
    mIn = CIEC_WORD(256);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(1U, static_cast<CIEC_WORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_word_2) {
    mIn = CIEC_WORD(512);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(2U, static_cast<CIEC_WORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_word_max) {
    mIn = CIEC_WORD(0xFFFF);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(0xFFFFU, static_cast<CIEC_WORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_DWORD_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_DWORD_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DWORD mIn;
      CIEC_DWORD mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_DWORD_Tests, F_FROM_BIG_ENDIAN_DWORD_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_dword_1) {
    mIn = CIEC_DWORD(16777216);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(1U, static_cast<CIEC_DWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_dword_2) {
    mIn = CIEC_DWORD(33554432);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(2U, static_cast<CIEC_DWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_dword_max) {
    mIn = CIEC_DWORD(0xFFFFFFFF);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(0xFFFFFFFFU, static_cast<CIEC_DWORD::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_BYTE_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_BYTE_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_BYTE mIn;
      CIEC_BYTE mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_BYTE_Tests, F_FROM_BIG_ENDIAN_BYTE_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_byte_1) {
    mIn = CIEC_BYTE(1);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(1U, static_cast<CIEC_BYTE::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_byte_2) {
    mIn = CIEC_BYTE(2);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(2U, static_cast<CIEC_BYTE::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_byte_max) {
    mIn = CIEC_BYTE(255);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(255U, static_cast<CIEC_BYTE::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_SINT_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_SINT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_SINT mIn;
      CIEC_SINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_SINT_Tests, F_FROM_BIG_ENDIAN_SINT_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_sint_10) {
    mIn = CIEC_SINT(10);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(10, static_cast<CIEC_SINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_sint_max) {
    mIn = CIEC_SINT(127);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(127, static_cast<CIEC_SINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_sint_min) {
    mIn = CIEC_SINT(-128);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(-128, static_cast<CIEC_SINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_INT_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_INT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_INT mIn;
      CIEC_INT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_INT_Tests, F_FROM_BIG_ENDIAN_INT_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_int_11) {
    mIn = CIEC_INT(2816);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(11, static_cast<CIEC_INT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_int_max) {
    mIn = CIEC_INT(32767);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_INT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_int_min) {
    mIn = CIEC_INT(-32768);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_INT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_DINT_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_DINT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn;
      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_DINT_Tests, F_FROM_BIG_ENDIAN_DINT_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_dint_12) {
    mIn = CIEC_DINT(201326592);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(12, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_dint_max) {
    mIn = CIEC_DINT(2147483647);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_dint_min) {
    mIn = CIEC_DINT(-2147483648);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_LINT_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_LINT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_LINT mIn;
      CIEC_LINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_LINT_Tests, F_FROM_BIG_ENDIAN_LINT_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_lint_13) {
    mIn = CIEC_LINT(936748722493063168LL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(13, static_cast<CIEC_LINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_lint_max) {
    mIn = CIEC_LINT(9223372036854775807LL);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_LINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_CASE(from_big_endian_lint_min) {
    mIn = CIEC_LINT(-9223372036854775807LL - 1);
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_LINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_FROM_BIG_ENDIAN_ARRAY_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_FROM_BIG_ENDIAN_ARRAY_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_FROM_BIG_ENDIAN"_STRID) {
        setInputData({&mIn});
        setOutputData({&mOut});
        setup();
      }

      CIEC_ARRAY_FIXED<CIEC_LWORD, -1, 1> mIn;
      CIEC_ARRAY_FIXED<CIEC_LWORD, -1, 1> mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_FROM_BIG_ENDIAN_ARRAY_Tests, F_FROM_BIG_ENDIAN_ARRAY_TestFixture)

  BOOST_AUTO_TEST_CASE(from_big_endian_array_1_2_3) {
    mIn = CIEC_ARRAY_FIXED<CIEC_LWORD, -1, 1>{CIEC_LWORD(72057594037927936ULL), CIEC_LWORD(144115188075855872ULL),
                                              CIEC_LWORD(216172782113783808ULL)};
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(1ULL, static_cast<CIEC_LWORD::TValueType>(mOut[-1]));
    BOOST_CHECK_EQUAL(2ULL, static_cast<CIEC_LWORD::TValueType>(mOut[0]));
    BOOST_CHECK_EQUAL(3ULL, static_cast<CIEC_LWORD::TValueType>(mOut[1]));
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::iec61131::conversion
