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
#include "../../src/core/typelib.h"
#include "../../src/core/datatypes/forte_any.h"
#include "../../src/core/datatypes/forte_bool.h"
#include "../../src/core/datatypes/forte_sint.h"
#include "../../src/core/datatypes/forte_int.h"
#include "../../src/core/datatypes/forte_dint.h"
#include "../../src/core/datatypes/forte_lint.h"
#include "../../src/core/datatypes/forte_usint.h"
#include "../../src/core/datatypes/forte_uint.h"
#include "../../src/core/datatypes/forte_udint.h"
#include "../../src/core/datatypes/forte_ulint.h"
#include "../../src/core/datatypes/forte_byte.h"
#include "../../src/core/datatypes/forte_word.h"
#include "../../src/core/datatypes/forte_dword.h"
#include "../../src/core/datatypes/forte_lword.h"
#include "../../src/core/datatypes/forte_date.h"
#include "../../src/core/datatypes/forte_date_and_time.h"
#include "../../src/core/datatypes/forte_time_of_day.h"
#include "../../src/core/datatypes/forte_time.h"
#include "../../src/core/datatypes/forte_real.h"
#include "../../src/core/datatypes/forte_lreal.h"
#include "../../src/core/datatypes/forte_string.h"
#include "../../src/core/datatypes/forte_wstring.h"


#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "typelibdatatypetests_gen.cpp"
#else
#include "stringlist.h"
#endif

BOOST_AUTO_TEST_SUITE(TypeLibDataTypeTests)

  BOOST_AUTO_TEST_CASE(createANY){
    TForteByte acDataBuf[sizeof(CIEC_ANY)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdANY, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ANY, poVal->getDataTypeID());

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdANY, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ANY, poVal->getDataTypeID());
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createBOOL){
    TForteByte acDataBuf[sizeof(CIEC_BOOL)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdBOOL, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(false, *static_cast<CIEC_BOOL *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdBOOL, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BOOL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(false, *static_cast<CIEC_BOOL *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createSINT){
    TForteByte acDataBuf[sizeof(CIEC_SINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdSINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_SINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_SINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdSINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_SINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_SINT *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createINT){
    TForteByte acDataBuf[sizeof(CIEC_INT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_INT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_INT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_INT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_INT *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDINT){
    TForteByte acDataBuf[sizeof(CIEC_DINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdDINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdDINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DINT *>(poVal));
    delete poVal;
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(createLINT){
    TForteByte acDataBuf[sizeof(CIEC_LINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdLINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdLINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LINT *>(poVal));
    delete poVal;
  }
#endif

  BOOST_AUTO_TEST_CASE(createUSINT){
    TForteByte acDataBuf[sizeof(CIEC_USINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdUSINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_USINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_USINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdUSINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_USINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_USINT *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createUINT){
    TForteByte acDataBuf[sizeof(CIEC_UINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdUINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_UINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdUINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_UINT *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createUDINT){
    TForteByte acDataBuf[sizeof(CIEC_UDINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdUDINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UDINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_UDINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdUDINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_UDINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_UDINT *>(poVal));
    delete poVal;
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(createULINT){
    TForteByte acDataBuf[sizeof(CIEC_ULINT)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdULINT, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ULINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_ULINT *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdULINT, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_ULINT, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_ULINT *>(poVal));
    delete poVal;
  }
#endif

  BOOST_AUTO_TEST_CASE(createBYTE){
    TForteByte acDataBuf[sizeof(CIEC_BYTE)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdBYTE, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BYTE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_BYTE *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdBYTE, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_BYTE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_BYTE *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createWORD){
    TForteByte acDataBuf[sizeof(CIEC_WORD)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdWORD, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_WORD *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdWORD, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_WORD *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDWORD){
    TForteByte acDataBuf[sizeof(CIEC_DWORD)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdDWORD, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DWORD *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdDWORD, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DWORD *>(poVal));
    delete poVal;
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(createLWORD){
    TForteByte acDataBuf[sizeof(CIEC_LWORD)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdLWORD, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LWORD *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdLWORD, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LWORD, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_LWORD *>(poVal));
    delete poVal;
  }
#endif

  BOOST_AUTO_TEST_CASE(createDATE){
    TForteByte acDataBuf[sizeof(CIEC_DATE)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdDATE, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdDATE, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createTIME_OF_DAY){
    TForteByte acDataBuf[sizeof(CIEC_TIME_OF_DAY)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdTIME_OF_DAY, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME_OF_DAY, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_TIME_OF_DAY *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdTIME_OF_DAY, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME_OF_DAY, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_TIME_OF_DAY *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createDATE_AND_TIME){
    TForteByte acDataBuf[sizeof(CIEC_DATE_AND_TIME)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdDATE_AND_TIME, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE_AND_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE_AND_TIME *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdDATE_AND_TIME, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_DATE_AND_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_DATE_AND_TIME *>(poVal));
    delete poVal;
  }

  BOOST_AUTO_TEST_CASE(createTIME){
    TForteByte acDataBuf[sizeof(CIEC_TIME)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdTIME, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_TIME *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdTIME, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_TIME, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0, *static_cast<CIEC_TIME *>(poVal));
    delete poVal;
  }

#ifdef FORTE_USE_REAL_DATATYPE
  BOOST_AUTO_TEST_CASE(createREAL){
    TForteByte acDataBuf[sizeof(CIEC_REAL)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdREAL, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_REAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_REAL *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdREAL, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_REAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_REAL *>(poVal));
    delete poVal;
  }
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
  BOOST_AUTO_TEST_CASE(createLREAL){
    TForteByte acDataBuf[sizeof(CIEC_LREAL)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdLREAL, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LREAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_LREAL *>(poVal));

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdLREAL, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_LREAL, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL(0.0f, *static_cast<CIEC_LREAL *>(poVal));
    delete poVal;
  }
#endif

  BOOST_AUTO_TEST_CASE(createSTRING){
    TForteByte acDataBuf[sizeof(CIEC_STRING)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdSTRING, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_STRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_STRING *>(poVal)->getValue());

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdSTRING, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_STRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_STRING *>(poVal)->getValue());
    delete poVal;
  }

#ifdef FORTE_USE_WSTRING_DATATYPE
  BOOST_AUTO_TEST_CASE(createWSTRING){
    TForteByte acDataBuf[sizeof(CIEC_WSTRING)];
    CIEC_ANY *poVal = CTypeLib::createDataTypeInstance(g_nStringIdWSTRING, acDataBuf);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WSTRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_WSTRING *>(poVal)->getValue());

    poVal = CTypeLib::createDataTypeInstance(g_nStringIdWSTRING, 0);
    BOOST_CHECK_EQUAL(CIEC_ANY::e_WSTRING, poVal->getDataTypeID());
    BOOST_CHECK_EQUAL("", static_cast<CIEC_WSTRING *>(poVal)->getValue());
    delete poVal;
  }
#endif

 //TODO check array

  BOOST_AUTO_TEST_SUITE_END()

