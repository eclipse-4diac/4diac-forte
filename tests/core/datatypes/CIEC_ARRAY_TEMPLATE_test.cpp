/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_array.h"
#include "../../../src/core/datatypes/forte_array_variable.h"
#include "../../../src/core/datatypes/forte_array_fixed.h"
#include "../../../src/core/datatypes/forte_bool.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_dint.h"
#include "../../../src/core/typelib.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CIEC_ARRAY_TEMPLATE_test_gen.cpp"
#endif

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_TEMPLATE_function_test)

/* Static length tests */

BOOST_AUTO_TEST_CASE(InstantiateTemplateWithLimitsArray) {
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

  BOOST_CHECK_EQUAL(intArray.size(), 11);
  BOOST_CHECK_EQUAL(intArray.getLowerBound(), 0);
  BOOST_CHECK_EQUAL(intArray.getUpperBound(), 10);
  BOOST_CHECK_EQUAL(intArray.getDataTypeID(), CIEC_ANY::e_ARRAY);
  BOOST_CHECK_EQUAL(intArray.getElementDataTypeID(), CIEC_ANY::e_INT);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 0);
}

BOOST_AUTO_TEST_CASE(InitializerListStaticArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                      CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                      CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

BOOST_AUTO_TEST_CASE(InitializeNegativeRangesStaticArray) {
  CIEC_ARRAY_FIXED<CIEC_INT, -10, 0> intArray = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                    CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                    CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-10]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-9]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-8]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-7]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-6]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-4]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-3]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-2]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-1]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 11);
}

BOOST_AUTO_TEST_CASE(StaticArrayIndexAccesWithIECTypes)
{
  CIEC_ARRAY_FIXED<CIEC_INT, -10, 0> intArray = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                 CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                 CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-10)]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-9)]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-8)]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-7)]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-6)]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-5)]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-4)]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-3)]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-2)]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-1)]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(0)]), 11);
}

BOOST_AUTO_TEST_CASE(WriteElementsStaticArray) {
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

  intArray[0] = CIEC_INT(1);
  intArray[1] = CIEC_INT(2);
  intArray[2] = CIEC_INT(3);
  intArray[3] = CIEC_INT(4);
  intArray[4] = CIEC_INT(5);
  intArray[5] = CIEC_INT(6);
  intArray[6] = CIEC_INT(7);
  intArray[7] = CIEC_INT(8);
  intArray[8] = CIEC_INT(9);
  intArray[9] = CIEC_INT(10);
  intArray[10] = CIEC_INT(11);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
}

BOOST_AUTO_TEST_CASE(WriteElementsStaticArrayWithIECTypeIndices)
{
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

  intArray[CIEC_UINT(0)] = CIEC_INT(1);
  intArray[CIEC_UINT(1)] = CIEC_INT(2);
  intArray[CIEC_UINT(2)] = CIEC_INT(3);
  intArray[CIEC_UINT(3)] = CIEC_INT(4);
  intArray[CIEC_UINT(4)] = CIEC_INT(5);
  intArray[CIEC_UINT(5)] = CIEC_INT(6);
  intArray[CIEC_UINT(6)] = CIEC_INT(7);
  intArray[CIEC_UINT(7)] = CIEC_INT(8);
  intArray[CIEC_UINT(8)] = CIEC_INT(9);
  intArray[CIEC_UINT(9)] = CIEC_INT(10);
  intArray[CIEC_UINT(10)] = CIEC_INT(11);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
}

BOOST_AUTO_TEST_CASE(StaticToStaticCopyConstructor)
{
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                   CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                   CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray(intArraySource);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
}

BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeSameSizeCopyConstructor)
{
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                         CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                         CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  CIEC_ARRAY_FIXED<CIEC_DINT, 0, 10> intArray(intArraySource);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[10]), 11);
}

BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeDifferentSizeWindowCopyConstructor)
{
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                         CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                         CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  CIEC_ARRAY_FIXED<CIEC_DINT, 3, 5> intArray(intArraySource);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[5]), 6);
}

BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeMovedSizeWindowCopyConstructor)
{
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                         CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                         CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  CIEC_ARRAY_FIXED<CIEC_DINT, 3, 12> dintArray(intArraySource);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[10]), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[11]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[12]), 0);
}

/* Variable length tests */

BOOST_AUTO_TEST_CASE(InstantiateSizeOnlyVariableArray)
{
  CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray(0, 10);
  BOOST_CHECK_EQUAL(boolArray.size(), 11);
  BOOST_CHECK_EQUAL(boolArray.getLowerBound(), 0);
  BOOST_CHECK_EQUAL(boolArray.getUpperBound(), 10);
  BOOST_CHECK_EQUAL(boolArray.getDataTypeID(), CIEC_ANY::e_ARRAY);
  BOOST_CHECK_EQUAL(boolArray.getElementDataTypeID(), CIEC_ANY::e_BOOL);
}

BOOST_AUTO_TEST_CASE(InstantiateVariableArrayFromInitList)
{
  CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {CIEC_BOOL(true), CIEC_BOOL(false), CIEC_BOOL(false), CIEC_BOOL(true)};
  BOOST_CHECK_EQUAL(boolArray.size(), 4);
  BOOST_CHECK_EQUAL(boolArray.getLowerBound(), 0);
  BOOST_CHECK_EQUAL(boolArray.getUpperBound(), 3);
  BOOST_CHECK_EQUAL(boolArray.getDataTypeID(), CIEC_ANY::e_ARRAY);
  BOOST_CHECK_EQUAL(boolArray.getElementDataTypeID(), CIEC_ANY::e_BOOL);
}

BOOST_AUTO_TEST_CASE(VariableArrayAccess)
{
  CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {CIEC_BOOL(true), CIEC_BOOL(false), CIEC_BOOL(false), CIEC_BOOL(true)};
  BOOST_CHECK_EQUAL(boolArray[0], CIEC_BOOL(true));
  BOOST_CHECK_EQUAL(boolArray[1], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[2], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[3], CIEC_BOOL(true));
}

BOOST_AUTO_TEST_CASE(VariableArrayAccessIECTypes)
{
  CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {CIEC_BOOL(true), CIEC_BOOL(false), CIEC_BOOL(false), CIEC_BOOL(true)};
  BOOST_CHECK_EQUAL(boolArray[CIEC_INT(0)], CIEC_BOOL(true));
  BOOST_CHECK_EQUAL(boolArray[CIEC_INT(1)], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[CIEC_INT(2)], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[CIEC_INT(3)], CIEC_BOOL(true));
}

/* copy tests */

BOOST_AUTO_TEST_CASE(CopyFromStaticToVariableArray) {
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),
                                                         CIEC_INT(5), CIEC_INT(6), CIEC_INT(7), CIEC_INT(8),
                                                         CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

  CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(intArraySource);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
}

BOOST_AUTO_TEST_CASE(CopyFromVariableToStaticArray)
{
  CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArraySource = {CIEC_BOOL(true), CIEC_BOOL(false), CIEC_BOOL(false), CIEC_BOOL(true)};
  CIEC_ARRAY_FIXED<CIEC_BOOL, 0, 3> boolArray(boolArraySource);

  BOOST_CHECK_EQUAL(boolArray[0], CIEC_BOOL(true));
  BOOST_CHECK_EQUAL(boolArray[1], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[2], CIEC_BOOL(false));
  BOOST_CHECK_EQUAL(boolArray[3], CIEC_BOOL(true));
}

BOOST_AUTO_TEST_CASE(CopyFromPlainOldToStaticArray) {
  CIEC_ARRAY_DYNAMIC arraySource(5, g_nStringIdINT);

  static_cast<CIEC_INT &>(arraySource[0]) = CIEC_INT(1);
  static_cast<CIEC_INT &>(arraySource[1]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(arraySource[2]) = CIEC_INT(256);
  static_cast<CIEC_INT &>(arraySource[3]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(arraySource[4]) = CIEC_INT(256);

  CIEC_ARRAY_FIXED<CIEC_INT, 0, 4> arrayDestination(arraySource);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[1]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[2]), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[3]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[4]), 256);
}

BOOST_AUTO_TEST_CASE(CopyFromPlainOldToVariableArray)
{
  CIEC_ARRAY_DYNAMIC arraySource(5, g_nStringIdINT);

  static_cast<CIEC_INT &>(arraySource[0]) = CIEC_INT(1);
  static_cast<CIEC_INT &>(arraySource[1]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(arraySource[2]) = CIEC_INT(256);
  static_cast<CIEC_INT &>(arraySource[3]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(arraySource[4]) = CIEC_INT(256);

  CIEC_ARRAY_VARIABLE<CIEC_INT> arrayDestination(arraySource);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[1]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[2]), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[3]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[4]), 256);
}

BOOST_AUTO_TEST_CASE(CopyFromVariableToPlainOldArray)
{
  CIEC_ARRAY_VARIABLE<CIEC_INT> arraySource(0, 5);
  (arraySource[0]) = CIEC_INT(1);
  (arraySource[1]) = CIEC_INT(-32259);
  (arraySource[2]) = CIEC_INT(256);
  (arraySource[3]) = CIEC_INT(-32259);
  (arraySource[4]) = CIEC_INT(256);

  CIEC_ARRAY_DYNAMIC arrayDestination(arraySource);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[1])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[2])), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[3])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[4])), 256);
}

BOOST_AUTO_TEST_SUITE_END() // CIEC_ARRAY_TEMPLATE_function_test
