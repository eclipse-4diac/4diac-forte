/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
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
#include "forte_boost_output_support.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_date.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

using namespace forte::literals;

using namespace std::string_literals;

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(TypeLibDataTypeTests)

  BOOST_AUTO_TEST_CASE(createANY) {
    TForteByte acDataBuf[sizeof(CIEC_ANY_VARIANT)];
    CIEC_ANY *poVal = createDataTypeInstance("ANY"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ANY, poVal->getDataTypeID());

    poVal = createDataTypeInstance("ANY"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ANY, poVal->getDataTypeID());
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createBOOL) {
    TForteByte acDataBuf[sizeof(CIEC_BOOL)];
    CIEC_ANY *poVal = createDataTypeInstance("BOOL"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(false, *static_cast<CIEC_BOOL *>(poVal));

    poVal = createDataTypeInstance("BOOL"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(false, *static_cast<CIEC_BOOL *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createSINT) {
    TForteByte acDataBuf[sizeof(CIEC_SINT)];
    CIEC_ANY *poVal = createDataTypeInstance("SINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_SINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_SINT::TValueType>(*static_cast<CIEC_SINT *>(poVal)));

    poVal = createDataTypeInstance("SINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_SINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_SINT::TValueType>(*static_cast<CIEC_SINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createINT) {
    TForteByte acDataBuf[sizeof(CIEC_INT)];
    CIEC_ANY *poVal = createDataTypeInstance("INT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_INT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(poVal)));

    poVal = createDataTypeInstance("INT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_INT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDINT) {
    TForteByte acDataBuf[sizeof(CIEC_DINT)];
    CIEC_ANY *poVal = createDataTypeInstance("DINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_DINT::TValueType>(*static_cast<CIEC_DINT *>(poVal)));

    poVal = createDataTypeInstance("DINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_DINT::TValueType>(*static_cast<CIEC_DINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createLINT) {
    TForteByte acDataBuf[sizeof(CIEC_LINT)];
    CIEC_ANY *poVal = createDataTypeInstance("LINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_LINT::TValueType>(*static_cast<CIEC_LINT *>(poVal)));

    poVal = createDataTypeInstance("LINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_LINT::TValueType>(*static_cast<CIEC_LINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createUSINT) {
    TForteByte acDataBuf[sizeof(CIEC_USINT)];
    CIEC_ANY *poVal = createDataTypeInstance("USINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_USINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_USINT::TValueType>(*static_cast<CIEC_USINT *>(poVal)));

    poVal = createDataTypeInstance("USINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_USINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_USINT::TValueType>(*static_cast<CIEC_USINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createUINT) {
    TForteByte acDataBuf[sizeof(CIEC_UINT)];
    CIEC_ANY *poVal = createDataTypeInstance("UINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, poVal->getDataTypeID());
    BOOST_TEST(0 == static_cast<CIEC_UINT::TValueType>(*static_cast<CIEC_UINT *>(poVal)));

    poVal = createDataTypeInstance("UINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, poVal->getDataTypeID());
    BOOST_TEST(0 == static_cast<CIEC_UINT::TValueType>(*static_cast<CIEC_UINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createUDINT) {
    TForteByte acDataBuf[sizeof(CIEC_UDINT)];
    CIEC_ANY *poVal = createDataTypeInstance("UDINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UDINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_UDINT::TValueType>(*static_cast<CIEC_UDINT *>(poVal)));

    poVal = createDataTypeInstance("UDINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UDINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_UDINT::TValueType>(*static_cast<CIEC_UDINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createULINT) {
    TForteByte acDataBuf[sizeof(CIEC_ULINT)];
    CIEC_ANY *poVal = createDataTypeInstance("ULINT"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ULINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_ULINT::TValueType>(*static_cast<CIEC_ULINT *>(poVal)));

    poVal = createDataTypeInstance("ULINT"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ULINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_ULINT::TValueType>(*static_cast<CIEC_ULINT *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createBYTE) {
    TForteByte acDataBuf[sizeof(CIEC_BYTE)];
    CIEC_ANY *poVal = createDataTypeInstance("BYTE"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BYTE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_BYTE *>(poVal));

    poVal = createDataTypeInstance("BYTE"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BYTE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_BYTE *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createWORD) {
    TForteByte acDataBuf[sizeof(CIEC_WORD)];
    CIEC_ANY *poVal = createDataTypeInstance("WORD"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_WORD *>(poVal));

    poVal = createDataTypeInstance("WORD"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_WORD *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDWORD) {
    TForteByte acDataBuf[sizeof(CIEC_DWORD)];
    CIEC_ANY *poVal = createDataTypeInstance("DWORD"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DWORD *>(poVal));

    poVal = createDataTypeInstance("DWORD"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DWORD *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createLWORD) {
    TForteByte acDataBuf[sizeof(CIEC_LWORD)];
    CIEC_ANY *poVal = createDataTypeInstance("LWORD"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LWORD *>(poVal));

    poVal = createDataTypeInstance("LWORD"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LWORD *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDATE) {
    TForteByte acDataBuf[sizeof(CIEC_DATE)];
    CIEC_ANY *poVal = createDataTypeInstance("DATE"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE *>(poVal));

    poVal = createDataTypeInstance("DATE"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createTIME_OF_DAY) {
    TForteByte acDataBuf[sizeof(CIEC_TIME_OF_DAY)];
    CIEC_ANY *poVal = createDataTypeInstance("TIME_OF_DAY"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME_OF_DAY, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME_OF_DAY::TValueType>(*static_cast<CIEC_TIME_OF_DAY *>(poVal)));

    poVal = createDataTypeInstance("TIME_OF_DAY"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME_OF_DAY, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME_OF_DAY::TValueType>(*static_cast<CIEC_TIME_OF_DAY *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDATE_AND_TIME) {
    TForteByte acDataBuf[sizeof(CIEC_DATE_AND_TIME)];
    CIEC_ANY *poVal = createDataTypeInstance("DATE_AND_TIME"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE_AND_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE_AND_TIME *>(poVal));

    poVal = createDataTypeInstance("DATE_AND_TIME"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE_AND_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE_AND_TIME *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createTIME) {
    TForteByte acDataBuf[sizeof(CIEC_TIME)];
    CIEC_ANY *poVal = createDataTypeInstance("TIME"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(poVal)));

    poVal = createDataTypeInstance("TIME"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createREAL) {
    TForteByte acDataBuf[sizeof(CIEC_REAL)];
    CIEC_ANY *poVal = createDataTypeInstance("REAL"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_REAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_REAL *>(poVal));

    poVal = createDataTypeInstance("REAL"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_REAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_REAL *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createLREAL) {
    TForteByte acDataBuf[sizeof(CIEC_LREAL)];
    CIEC_ANY *poVal = createDataTypeInstance("LREAL"_STRID, acDataBuf);
    BOOST_TEST(CIEC_ANY::e_LREAL == poVal->getDataTypeID());
    BOOST_TEST(0.0 == static_cast<CIEC_LREAL::TValueType>(*static_cast<CIEC_LREAL *>(poVal)));

    poVal = createDataTypeInstance("LREAL"_STRID, nullptr);
    BOOST_TEST(CIEC_ANY::e_LREAL == poVal->getDataTypeID());
    BOOST_TEST(0.0 == static_cast<CIEC_LREAL::TValueType>(*static_cast<CIEC_LREAL *>(poVal)));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createSTRING) {
    TForteByte acDataBuf[sizeof(CIEC_STRING)];
    CIEC_ANY *poVal = createDataTypeInstance("STRING"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_STRING, poVal->getDataTypeID());
    BOOST_TEST(""s == static_cast<CIEC_STRING *>(poVal)->getStorage());

    poVal = createDataTypeInstance("STRING"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_STRING, poVal->getDataTypeID());
    BOOST_TEST(""s == static_cast<CIEC_STRING *>(poVal)->getStorage());
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createWSTRING) {
    TForteByte acDataBuf[sizeof(CIEC_WSTRING)];
    CIEC_ANY *poVal = createDataTypeInstance("WSTRING"_STRID, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WSTRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_WSTRING *>(poVal)->getValue());

    poVal = createDataTypeInstance("WSTRING"_STRID, nullptr);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WSTRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_WSTRING *>(poVal)->getValue());
    delete poVal;
  }

  // TODO check array

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
