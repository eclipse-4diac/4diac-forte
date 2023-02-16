/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 TU Vienna/ACIN, nxtControl, Profactor GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *   Martin Melik-Merkumians - adds test for FIND
 *   Martin Melik-Merkumians - adds tests for LEN, LEFT, RIGHT, MID, REPLACE, DELETE,
 *     INSERT, EQ, NE, ADD, and AND
 *   Martin Melik-Merkumians - adds test for TRUNC
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "iec61131_functions.h"

#include "forte_string.h"
#include "forte_uint.h"
#include "forte_lword.h"
#include "forte_any_bit_not_decorator.h"
#include "../../src/core/typelib.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "iec61131_functionstests_gen.cpp"
#endif

/*** STRUCT for tests *********/
class CIEC_EndianessTestStruct : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(EndianessTestStruct);

public:
  CIEC_EndianessTestStruct();

  virtual ~CIEC_EndianessTestStruct() = default;

  CIEC_EndianessTestStruct &operator=(const CIEC_EndianessTestStruct &paValue) {
    if (this != &paValue) {
      setValue(paValue);
    }
    return *this;
  }

private:
  static const CStringDictionary::TStringId scm_unElementTypes[];
  static const CStringDictionary::TStringId scm_unElementNames[];
};

const CStringDictionary::TStringId CIEC_EndianessTestStruct::scm_unElementTypes[] = {g_nStringIdBOOL, g_nStringIdDINT, g_nStringIdLWORD};
const CStringDictionary::TStringId CIEC_EndianessTestStruct::scm_unElementNames[] = {g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3};

DEFINE_FIRMWARE_DATATYPE(EndianessTestStruct, g_nStringIdEndianessTestStruct)
CIEC_EndianessTestStruct::CIEC_EndianessTestStruct() : CIEC_STRUCT(g_nStringIdEndianessTestStruct, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1)
{
}

void testSTInIsOutBoolDummyFunction(CIEC_BOOL paIn, CIEC_BOOL &paOut) {
  paOut = paIn;
}

void testSTInIsOutLWordDummyFunction(CIEC_LWORD paIn, CIEC_LWORD &paOut) {
  paOut = paIn;
}

BOOST_AUTO_TEST_SUITE(IEC61131_functions)

BOOST_AUTO_TEST_CASE(abs)
{
  CIEC_SINT nNumber(-13);
  BOOST_TEST(func_ABS(nNumber).getSignedValue() == 13);
}

BOOST_AUTO_TEST_CASE(sqrt)
{
  CIEC_REAL nNumber(4);
  BOOST_TEST(static_cast<TForteFloat>(func_SQRT(nNumber)) == 2.0f);
}

BOOST_AUTO_TEST_CASE(to_upper)
{
  CIEC_STRING sTestString("shall_be_to_upper");
  CIEC_STRING sToUpperString(func_TOUPPER(sTestString));
  BOOST_TEST(sToUpperString.getValue() == "SHALL_BE_TO_UPPER");
}

BOOST_AUTO_TEST_CASE(to_lower)
{
  CIEC_STRING sTestString("SHALL_BE_TO_LOWER");
  CIEC_STRING sToLowerString(func_TOLOWER(sTestString));
  BOOST_TEST(sToLowerString.getValue() == "shall_be_to_lower");
}

BOOST_AUTO_TEST_CASE(standard_example_len)
{
  CIEC_STRING sTestString("ASTRING");
  CIEC_UINT nLength(func_LEN(sTestString));
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nLength) == 7);
}

BOOST_AUTO_TEST_CASE(len)
{
  CIEC_STRING sTestString("Lorem ipsum dolor sit amet");
  CIEC_UINT nLength;
  nLength = func_LEN(sTestString);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nLength) == 26);
}

BOOST_AUTO_TEST_CASE(standard_example_left)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sLeftString(func_LEFT(sTestString, CIEC_INT(3)));
  BOOST_TEST(sLeftString.getValue() == "AST");
}

BOOST_AUTO_TEST_CASE(left)
{
  CIEC_STRING sTestString("SHALL_BE_CROPPED_HERE_THIS_SHOULD_NOT_BE_SEEN");
  CIEC_STRING sLeftString(func_LEFT(sTestString, CIEC_INT(21)));
  BOOST_TEST(sLeftString.getValue() == "SHALL_BE_CROPPED_HERE");
}

BOOST_AUTO_TEST_CASE(standard_example_right)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sRightString(func_RIGHT(sTestString, CIEC_INT(3)));
  BOOST_TEST(sRightString.getValue() == "STR");
}

BOOST_AUTO_TEST_CASE(right)
{
  CIEC_STRING sTestString("THIS_SHOULD_BE_CROPPED_THIS_SHOULD_BE_SEEN");
  CIEC_STRING sRightString(func_RIGHT(sTestString, CIEC_INT(19)));
  BOOST_TEST(sRightString.getValue() == "THIS_SHOULD_BE_SEEN");
}

BOOST_AUTO_TEST_CASE(standard_example_mid)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sMidString(func_MID(sTestString, CIEC_INT(2), CIEC_INT(2)));
  BOOST_TEST(sMidString.getValue() == "ST");
}

BOOST_AUTO_TEST_CASE(mid)
{
  CIEC_STRING sTestString("THIS_SHOULD_NOT_BE_SEEN_THIS_SHALL_BE_SEEN_THIS_SHOULD_NOT_BE_SEEN");
  CIEC_STRING sMidString(func_MID(sTestString, CIEC_INT(18), CIEC_INT(25)));
  BOOST_TEST(sMidString.getValue() == "THIS_SHALL_BE_SEEN");
}

BOOST_AUTO_TEST_CASE(concat)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sConcatString(func_CONCAT(sFristString, sSecondString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING");
}

BOOST_AUTO_TEST_CASE(find_at_begin)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit amet");
  CIEC_STRING sSearchString("Lorem");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(1 == static_cast<CIEC_UINT::TValueType>(nIndex));
}

BOOST_AUTO_TEST_CASE(find_in_between)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit amet");
  CIEC_STRING sSearchString("dolor");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(13 == static_cast<CIEC_UINT::TValueType>(nIndex));
}

BOOST_AUTO_TEST_CASE(find_at_the_end)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("t");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(21 == static_cast<CIEC_UINT::TValueType>(nIndex));
}

BOOST_AUTO_TEST_CASE(find_not_found)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("Latin");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(0 == static_cast<CIEC_UINT::TValueType>(nIndex));
}

BOOST_AUTO_TEST_CASE(standard_example_replace)
{
  CIEC_STRING sIn1("ABCDE");
  CIEC_STRING sIn2("X");
  CIEC_STRING sResult(func_REPLACE(sIn1, sIn2, CIEC_INT(2), CIEC_INT(3)));
  BOOST_TEST(sResult.getValue() == "ABXE");
}

BOOST_AUTO_TEST_CASE(replace)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sIn2("muspi");
  CIEC_STRING sResult(func_REPLACE(sIn1, sIn2, CIEC_INT(5), CIEC_INT(7)));
  BOOST_TEST(sResult.getValue() == "Lorem muspi dolor sit");
}

BOOST_AUTO_TEST_CASE(replace_P_signed_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sIn2("muspi");
  CIEC_STRING sResult(func_REPLACE(sIn1, sIn2, CIEC_INT(5), CIEC_INT(0)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(replace_P_signed_negative_number)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sIn2("muspi");
  CIEC_STRING sResult(func_REPLACE(sIn1, sIn2, CIEC_INT(5), CIEC_INT(-200)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(replace_P_unsigned_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sIn2("muspi");
  CIEC_STRING sResult(func_REPLACE(sIn1, sIn2, CIEC_UINT(5), CIEC_UINT(0)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(standard_example_delete)
{
  CIEC_STRING sIn1("ABXYC");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(2), CIEC_INT(3)));
  BOOST_TEST(sResult.getValue() == "ABC");
}

BOOST_AUTO_TEST_CASE(delete_function)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(6), CIEC_INT(12)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum sit");
}

BOOST_AUTO_TEST_CASE(delete_function_L_unsigned_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_UINT(0), CIEC_INT(12)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_L_signed_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(0), CIEC_INT(12)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_L_signed_negative_number)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(-4), CIEC_INT(12)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_P_unsigned_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(6), CIEC_UINT(0)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_P_signed_0)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(6), CIEC_INT(0)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_P_signed_negative_number)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(6), CIEC_INT(-32370)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(delete_function_length_exceeding_string)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(func_DELETE(sIn1, CIEC_INT(6), CIEC_INT(30)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(eq_true)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(func_EQ(nInt1, nInt2) == true);
}

BOOST_AUTO_TEST_CASE(eq_false)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(func_EQ(nInt1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_true)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(func_NE(nInt1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_false)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(func_NE(nInt1, nInt2) == true);
}

BOOST_AUTO_TEST_CASE(eq_true_mixed_types)
{
  CIEC_LINT nLint1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(func_EQ(nLint1, nInt2) == true);
}

BOOST_AUTO_TEST_CASE(eq_false_mixed_types)
{
  CIEC_LINT nLint1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(func_EQ(nLint1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_true_mixed_types)
{
  CIEC_LINT nLint1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(func_NE(nLint1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_false_mixed_types)
{
  CIEC_LINT nLint1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(func_NE(nLint1, nInt2) == true);
}

BOOST_AUTO_TEST_CASE(implicit_bool_casts)
{
  bool bSame = std::is_same<CIEC_BOOL, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_BOOL>::type>::value;
  BOOST_TEST(bSame);
  bSame = std::is_same<CIEC_BYTE, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_BYTE>::type>::value;
  BOOST_TEST(bSame);
  bSame = std::is_same<CIEC_WORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_WORD>::type>::value;
  BOOST_TEST(bSame);
  bSame = std::is_same<CIEC_DWORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_DWORD>::type>::value;
  BOOST_TEST(bSame);
  bSame = std::is_same<CIEC_LWORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_LWORD>::type>::value;
  BOOST_TEST(bSame);
}

BOOST_AUTO_TEST_CASE(and_function_BYTE_DWORD)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30010);
  CIEC_DWORD nResult(func_AND(nByte, nDword));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 10);
}

BOOST_AUTO_TEST_CASE(and_function_DWORD_BYTE)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30010);
  CIEC_DWORD nResult(func_AND(nDword, nByte));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 10);
}

BOOST_AUTO_TEST_CASE(or_function_BYTE_DWORD)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30100);
  CIEC_DWORD nResult(func_OR(nByte, nDword));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30110);
}

BOOST_AUTO_TEST_CASE(or_function_DWORD_BYTE)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30100);
  CIEC_DWORD nResult(func_OR(nDword, nByte));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30110);
}

BOOST_AUTO_TEST_CASE(xor_function_BYTE_DWORD)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30100);
  CIEC_DWORD nResult(func_XOR(nByte, nDword));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30110);
}

BOOST_AUTO_TEST_CASE(xor_function_DWORD_BYTE)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30100);
  CIEC_DWORD nResult(func_XOR(nDword, nByte));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30110);
}

BOOST_AUTO_TEST_CASE(add_function_UDINT_USINT)
{
  CIEC_USINT nUsint(10);
  CIEC_UDINT nUdint(30010);
  CIEC_UDINT nResult(func_ADD(nUdint, nUsint));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30020);
}

BOOST_AUTO_TEST_CASE(add_function_LREAL_USINT)
{
  CIEC_USINT nUsint(10);
  CIEC_LREAL nLreal(30010);
  CIEC_LREAL nResult(func_ADD(nLreal, nUsint));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30020);
}

//BOOST_AUTO_TEST_CASE(add_function_LREAL_ULINT) //should fail, currently no way to automatically test this
//{
//  CIEC_ULINT nUsint(10);
//  CIEC_LREAL nLreal(30010);
//  CIEC_LREAL nResult(ADD(nLreal, nUsint));
//  BOOST_TEST(static_cast<TForteDWord>(nResult) == 30020);
//}

BOOST_AUTO_TEST_CASE(add_different_int_types_of_literals) {
  CIEC_LINT result;

  result = func_ADD(CIEC_SINT(-9), CIEC_DINT(9));
  BOOST_REQUIRE_EQUAL(static_cast<CIEC_LINT::TValueType>(CIEC_LINT(0)), static_cast<CIEC_LINT::TValueType>(result));
}

BOOST_AUTO_TEST_CASE(concat3)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sThirdString("_THIS_IS_THE_THIRD_STRING");
  CIEC_STRING sConcatString(func_CONCAT(sFristString, sSecondString, sThirdString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING_THIS_IS_THE_THIRD_STRING");
}

BOOST_AUTO_TEST_CASE(concat4)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sThirdString("_THIS_IS_THE_THIRD_STRING");
  CIEC_STRING sForthString("_THIS_IS_THE_FORTH_STRING");
  CIEC_STRING sConcatString(func_CONCAT(sFristString, sSecondString, sThirdString, sForthString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING_THIS_IS_THE_THIRD_STRING_THIS_IS_THE_FORTH_STRING");
}

BOOST_AUTO_TEST_CASE(standard_example_insert)
{
  CIEC_STRING sIn1("ABC");
  CIEC_STRING sIn2("XY");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_UINT(2));
  BOOST_TEST(sResult.getValue() == "ABXYC");
}

BOOST_AUTO_TEST_CASE(insert)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_UINT(6));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_CASE(insert_P_larger_than_input_string)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_UINT(11));
  BOOST_TEST(sResult.getValue() == "Lorem  sit");
}

BOOST_AUTO_TEST_CASE(insert_P_unsigned_0)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_UINT(0));
  BOOST_TEST(sResult.getValue() == "Lorem  sit");
}

BOOST_AUTO_TEST_CASE(insert_P_signed_0)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_INT(0));
  BOOST_TEST(sResult.getValue() == "Lorem  sit");
}

BOOST_AUTO_TEST_CASE(insert_P_signed_negative_number)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = func_INSERT(sIn1, sIn2, CIEC_INT(-20));
  BOOST_TEST(sResult.getValue() == "Lorem  sit");
}

BOOST_AUTO_TEST_CASE(trunc)
{
  CIEC_REAL real(50.6f);
  CIEC_LREAL lreal(50.6);

  // checking return type
  BOOST_REQUIRE(true == static_cast<CIEC_BOOL::TValueType>(func_EQ(CIEC_SINT(50), func_TRUNC<CIEC_SINT>(real))));
  BOOST_REQUIRE(true == static_cast<CIEC_BOOL::TValueType>(func_EQ(CIEC_ULINT(50), func_TRUNC<CIEC_ULINT>(lreal))));

  // checking rare value
  BOOST_REQUIRE(50 == static_cast<CIEC_SINT::TValueType>(func_TRUNC<CIEC_SINT>(real)));
  BOOST_REQUIRE(50 == static_cast<CIEC_ULINT::TValueType>(CIEC_ULINT(50), func_TRUNC<CIEC_ULINT>(lreal)));
}

BOOST_AUTO_TEST_CASE(rol_unsigned)
{
  CIEC_BYTE byte(128);
  CIEC_BYTE result;
  CIEC_USINT usint(1);
  CIEC_UINT uint(2);
  CIEC_UDINT udint(3);
  CIEC_ULINT ulint(4);

  result = func_ROL(byte, usint);
  BOOST_REQUIRE_EQUAL(0x1, result);

  result = func_ROL(byte, uint);
  BOOST_REQUIRE_EQUAL(0x2, result);

  result = func_ROL(byte, udint);
  BOOST_REQUIRE_EQUAL(0x4, result);

  result = func_ROL(byte, ulint);
  BOOST_REQUIRE_EQUAL(0x8, result);
}

BOOST_AUTO_TEST_CASE(rol_signed)
{
  CIEC_BYTE byte(128);
  CIEC_BYTE result;
  CIEC_USINT sint(5);
  CIEC_UINT inte(6);
  CIEC_UDINT dint(7);
  CIEC_ULINT lint(8);

  result = func_ROL(byte, sint);
  BOOST_REQUIRE_EQUAL(0x10, result);

  result = func_ROL(byte, inte);
  BOOST_REQUIRE_EQUAL(0x20, result);

  result = func_ROL(byte, dint);
  BOOST_REQUIRE_EQUAL(0x40, result);

  result = func_ROL(byte, lint);
  BOOST_REQUIRE_EQUAL(0x80, result);
}

BOOST_AUTO_TEST_CASE(shl_unsigned)
{
  CIEC_BYTE byte(1);
  CIEC_BYTE result;
  CIEC_USINT usint(1);
  CIEC_UINT uint(2);
  CIEC_UDINT udint(3);
  CIEC_ULINT ulint(4);

  result = func_SHL(byte, usint);
  BOOST_REQUIRE_EQUAL(0x2, result);

  result = func_SHL(byte, uint);
  BOOST_REQUIRE_EQUAL(0x4, result);

  result = func_SHL(byte, udint);
  BOOST_REQUIRE_EQUAL(0x8, result);

  result = func_SHL(byte, ulint);
  BOOST_REQUIRE_EQUAL(0x10, result);
}

BOOST_AUTO_TEST_CASE(shl_signed)
{
  CIEC_BYTE byte(1);
  CIEC_BYTE result;
  CIEC_USINT sint(5);
  CIEC_UINT inte(6);
  CIEC_UDINT dint(7);
  CIEC_ULINT lint(8);

  result = func_SHL(byte, sint);
  BOOST_REQUIRE_EQUAL(0x20, result);

  result = func_SHL(byte, inte);
  BOOST_REQUIRE_EQUAL(0x40, result);

  result = func_SHL(byte, dint);
  BOOST_REQUIRE_EQUAL(0x80, result);

  result = func_SHL(byte, lint);
  BOOST_REQUIRE_EQUAL(0x0, result);
}

BOOST_AUTO_TEST_CASE(mul_numbers) {
  CIEC_USINT sint(5);
  CIEC_UINT inte(6);
  CIEC_UINT result;

  result = func_MUL(sint, inte);
  BOOST_REQUIRE_EQUAL(30, static_cast<CIEC_UINT::TValueType>(result));
}

BOOST_AUTO_TEST_CASE(mul_number_and_time) {
  CIEC_USINT sint(5);
  CIEC_TIME time(6);
  CIEC_TIME result;

  result = func_MUL(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(30), result);
}

BOOST_AUTO_TEST_CASE(mul_number_and_time_MUL_TIME) {
  CIEC_USINT sint(5);
  CIEC_TIME time(6);
  CIEC_TIME result;

  result = func_MUL_TIME(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(30), result);
}

BOOST_AUTO_TEST_CASE(mul_number_and_ltime) {
  CIEC_USINT sint(5);
  CIEC_LTIME time(6);
  CIEC_LTIME result;

  result = func_MUL(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(30), result);
}

BOOST_AUTO_TEST_CASE(mul_number_and_ltime_MUL_LTIME) {
  CIEC_USINT sint(5);
  CIEC_LTIME time(6);
  CIEC_LTIME result;

  result = func_MUL_LTIME(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(30), result);
}

BOOST_AUTO_TEST_CASE(div_numbers) {
  CIEC_USINT sint(30);
  CIEC_UINT inte(6);
  CIEC_UINT result;

  result = func_DIV(sint, inte);
  BOOST_REQUIRE_EQUAL(5, static_cast<CIEC_UINT::TValueType>(result));
}

BOOST_AUTO_TEST_CASE(div_number_and_time) {
  CIEC_USINT sint(5);
  CIEC_TIME time(30);
  CIEC_TIME result;

  result = func_DIV(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(6), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_time_DIV_TIME) {
  CIEC_USINT sint(5);
  CIEC_TIME time(30);
  CIEC_TIME result;

  result = func_DIV_TIME(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(6), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_time_real) {
  CIEC_REAL real(5);
  CIEC_TIME time(2000000);
  CIEC_TIME result;

  result = func_DIV(time, real);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(400000), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_time_real_DIV_TIME) {
  CIEC_REAL real(5);
  CIEC_TIME time(2000000);
  CIEC_TIME result;

  result = func_DIV_TIME(time, real);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(400000), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_ltime)
{
  CIEC_USINT sint(5);
  CIEC_LTIME time(30);
  CIEC_LTIME result;

  result = func_DIV(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(6), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_ltime_DIV_LTIME) {
  CIEC_USINT sint(5);
  CIEC_LTIME time(30);
  CIEC_LTIME result;

  result = func_DIV_LTIME(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(6), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_ltime_real) {
  CIEC_REAL real(5);
  CIEC_LTIME time(2000000);
  CIEC_LTIME result;

  result = func_DIV(time, real);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(400000), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_ltime_real_DIV_LTIME) {
  CIEC_REAL real(5);
  CIEC_LTIME time(2000000);
  CIEC_LTIME result;

  result = func_DIV_LTIME(time, real);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(400000), result);
}

BOOST_AUTO_TEST_CASE(add_two_times_with_add) {
  CIEC_TIME time1 = CIEC_TIME(1000);
  CIEC_TIME time2 = CIEC_TIME(2000);
  CIEC_TIME result;

  result = func_ADD(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(3000), result);
}

BOOST_AUTO_TEST_CASE(add_two_times_with_add_time)
{
  CIEC_TIME time1 = CIEC_TIME(1000);
  CIEC_TIME time2 = CIEC_TIME(2000);
  CIEC_TIME result;

  result = func_ADD_TIME(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(3000), result);
}

BOOST_AUTO_TEST_CASE(add_time_and_tod_with_add) {
  CIEC_TIME time1 = CIEC_TIME(1000000);
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(2000000);
  CIEC_TIME_OF_DAY result;

  result = func_ADD(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_TIME_OF_DAY(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_time_and_tod_with_add_tod_time) {
  CIEC_TIME time1 = CIEC_TIME(1000000);
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(2000000);
  CIEC_TIME_OF_DAY result;

  result = func_ADD_TOD_TIME(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_TIME_OF_DAY(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_date_and_time_and_time_with_add) {
  CIEC_TIME time1 = CIEC_TIME(1000000);
  CIEC_DATE_AND_TIME date_and_time1 = CIEC_DATE_AND_TIME(2000000);
  CIEC_DATE_AND_TIME result;

  result = func_ADD(date_and_time1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_DATE_AND_TIME(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_date_and_time_and_time_with_add_dt_time) {
  CIEC_TIME time1 = CIEC_TIME(1000000);
  CIEC_DATE_AND_TIME date_and_time1 = CIEC_DATE_AND_TIME(2000000);
  CIEC_DATE_AND_TIME result;

  result = func_ADD_DT_TIME(date_and_time1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_DATE_AND_TIME(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_two_ltimes_with_add) {
  CIEC_LTIME time1 = CIEC_LTIME(1000);
  CIEC_LTIME time2 = CIEC_LTIME(2000);
  CIEC_LTIME result;

  result = func_ADD(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(3000), result);
}

BOOST_AUTO_TEST_CASE(add_two_ltimes_with_add_time) {
  CIEC_LTIME time1 = CIEC_LTIME(1000);
  CIEC_LTIME time2 = CIEC_LTIME(2000);
  CIEC_LTIME result;

  result = func_ADD_LTIME(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(3000), result);
}

BOOST_AUTO_TEST_CASE(add_ltime_and_ltod_with_add) {
  CIEC_LTIME time1 = CIEC_LTIME(1000000);
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(2000000);
  CIEC_LTIME_OF_DAY result;

  result = func_ADD(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME_OF_DAY(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_ltime_and_ltod_with_add_ltod_ltime) {
  CIEC_LTIME time1 = CIEC_LTIME(1000000);
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(2000000);
  CIEC_LTIME_OF_DAY result;

  result = func_ADD_LTOD_LTIME(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME_OF_DAY(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_ldate_and_time_and_ltime_with_add)
{
  CIEC_LTIME time1 = CIEC_LTIME(1000000);
  CIEC_LDATE_AND_TIME date_and_time1 = CIEC_LDATE_AND_TIME(2000000);
  CIEC_LDATE_AND_TIME result;

  result = func_ADD(date_and_time1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LDATE_AND_TIME(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(add_ldate_and_time_and_ltime_with_add_ldt_ltime)
{
  CIEC_LTIME time1 = CIEC_LTIME(1000000);
  CIEC_LDATE_AND_TIME date_and_time1 = CIEC_LDATE_AND_TIME(2000000);
  CIEC_LDATE_AND_TIME result;

  result = func_ADD_LDT_LTIME(date_and_time1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LDATE_AND_TIME(1000000 + 2000000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_times_with_sub) {
  CIEC_TIME time1 = CIEC_TIME(1000);
  CIEC_TIME time2 = CIEC_TIME(2000);
  CIEC_TIME result;

  result = func_SUB(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(-1000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_times_with_sub_time) {
  CIEC_TIME time1 = CIEC_TIME(1000);
  CIEC_TIME time2 = CIEC_TIME(2000);
  CIEC_TIME result;

  result = func_SUB_TIME(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(-1000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_dates_with_sub) {
  CIEC_DATE date1 = CIEC_DATE(1000000000000);
  CIEC_DATE date2 = CIEC_DATE(2000000000000);
  CIEC_TIME result;

  result = func_SUB(date1, date2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 2000000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_dates_with_sub_DATE_DATE) {
  CIEC_DATE date1 = CIEC_DATE(1000000000000);
  CIEC_DATE date2 = CIEC_DATE(2000000000000);
  CIEC_TIME result;

  result = func_SUB_DATE_DATE(date1, date2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 2000000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_time_with_sub) {
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(1000000);
  CIEC_TIME time1 = CIEC_TIME(500000);
  CIEC_TIME_OF_DAY result;

  result = func_SUB(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_TIME_OF_DAY(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_time_with_sub_TOD_TIME) {
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(1000000);
  CIEC_TIME time1 = CIEC_TIME(500000);
  CIEC_TIME_OF_DAY result;

  result = func_SUB_TOD_TIME(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_TIME_OF_DAY(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_tod_with_sub) {
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(1000000000000);
  CIEC_TIME_OF_DAY tod2 = CIEC_TIME_OF_DAY(500000000000);
  CIEC_TIME result;

  result = func_SUB(tod1, tod2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_tod_with_sub_tod_tod) {
  CIEC_TIME_OF_DAY tod1 = CIEC_TIME_OF_DAY(1000000000000);
  CIEC_TIME_OF_DAY tod2 = CIEC_TIME_OF_DAY(500000000000);
  CIEC_TIME result;

  result = func_SUB_TOD_TOD(tod1, tod2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_dt_time_with_sub) {
  CIEC_DATE_AND_TIME dt1 = CIEC_DATE_AND_TIME(1000000);
  CIEC_TIME time1 = CIEC_TIME(500000);
  CIEC_DATE_AND_TIME result;

  result = func_SUB(dt1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_DATE_AND_TIME(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_dt_time_with_sub_dt_time) {
  CIEC_DATE_AND_TIME dt1 = CIEC_DATE_AND_TIME(1000000);
  CIEC_TIME time1 = CIEC_TIME(500000);
  CIEC_DATE_AND_TIME result;

  result = func_SUB_DT_TIME(dt1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_DATE_AND_TIME(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_dt_dt_with_sub) {
  CIEC_DATE_AND_TIME dt1 = CIEC_DATE_AND_TIME(1000000000000);
  CIEC_DATE_AND_TIME dt2 = CIEC_DATE_AND_TIME(500000000000);
  CIEC_TIME result;

  result = func_SUB(dt1, dt2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_dt_dt_with_sub_dt_dt) {
  CIEC_DATE_AND_TIME dt1 = CIEC_DATE_AND_TIME(1000000000000);
  CIEC_DATE_AND_TIME dt2 = CIEC_DATE_AND_TIME(500000000000);
  CIEC_TIME result;

  result = func_SUB_DT_DT(dt1, dt2);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_ltimes_with_sub) {
  CIEC_LTIME time1 = CIEC_LTIME(1000);
  CIEC_LTIME time2 = CIEC_LTIME(2000);
  CIEC_LTIME result;

  result = func_SUB(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000 - 2000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_ltimes_with_sub_ltime) {
  CIEC_LTIME time1 = CIEC_LTIME(1000);
  CIEC_LTIME time2 = CIEC_LTIME(2000);
  CIEC_LTIME result;

  result = func_SUB_LTIME(time1, time2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000 - 2000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_ldates_with_sub) {
  CIEC_LDATE date1 = CIEC_LDATE(1000000000000);
  CIEC_LDATE date2 = CIEC_LDATE(2000000000000);
  CIEC_LTIME result;

  result = func_SUB(date1, date2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 2000000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_two_ldates_with_SUB_LDATE_LDATE) {
  CIEC_LDATE date1 = CIEC_LDATE(1000000000000);
  CIEC_LDATE date2 = CIEC_LDATE(2000000000000);
  CIEC_LTIME result;

  result = func_SUB_LDATE_LDATE(date1, date2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 2000000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_ltime_with_sub) {
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(1000000);
  CIEC_LTIME time1 = CIEC_LTIME(500000);
  CIEC_LTIME_OF_DAY result;

  result = func_SUB(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME_OF_DAY(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_tod_ltime_with_sub_LTOD_LTIME) {
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(1000000);
  CIEC_LTIME time1 = CIEC_LTIME(500000);
  CIEC_LTIME_OF_DAY result;

  result = func_SUB_LTOD_LTIME(tod1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME_OF_DAY(1000000 - 500000), result);
}

BOOST_AUTO_TEST_CASE(sub_ltod_ltod_with_sub) {
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(1000000000000);
  CIEC_LTIME_OF_DAY tod2 = CIEC_LTIME_OF_DAY(500000000000);
  CIEC_LTIME result;

  result = func_SUB(tod1, tod2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_ltod_ltod_with_sub_ltod_ltod) {
  CIEC_LTIME_OF_DAY tod1 = CIEC_LTIME_OF_DAY(1000000000000);
  CIEC_LTIME_OF_DAY tod2 = CIEC_LTIME_OF_DAY(500000000000);
  CIEC_LTIME result;

  result = func_SUB_LTOD_LTOD(tod1, tod2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_ldt_ltime_with_sub) {
  CIEC_LDATE_AND_TIME dt1 = CIEC_LDATE_AND_TIME(1000000000000);
  CIEC_LTIME time1 = CIEC_LTIME(500000000000);
  CIEC_LDATE_AND_TIME result;

  result = func_SUB(dt1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LDATE_AND_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_ldt_ltime_with_sub_ldt_ltime) {
  CIEC_LDATE_AND_TIME dt1 = CIEC_LDATE_AND_TIME(1000000000000);
  CIEC_LTIME time1 = CIEC_LTIME(500000000000);
  CIEC_LDATE_AND_TIME result;

  result = func_SUB_LDT_LTIME(dt1, time1);
  BOOST_REQUIRE_EQUAL(CIEC_LDATE_AND_TIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_ldt_ldt_with_sub) {
  CIEC_LDATE_AND_TIME dt1 = CIEC_LDATE_AND_TIME(1000000000000);
  CIEC_LDATE_AND_TIME dt2 = CIEC_LDATE_AND_TIME(500000000000);
  CIEC_LTIME result;

  result = func_SUB(dt1, dt2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_ldt_ldt_with_sub_ldt_ldt) {
  CIEC_LDATE_AND_TIME dt1 = CIEC_LDATE_AND_TIME(1000000000000);
  CIEC_LDATE_AND_TIME dt2 = CIEC_LDATE_AND_TIME(500000000000);
  CIEC_LTIME result;

  result = func_SUB_LDT_LDT(dt1, dt2);
  BOOST_REQUIRE_EQUAL(CIEC_LTIME(1000000000000 - 500000000000), result);
}

BOOST_AUTO_TEST_CASE(sub_different_int_types_of_literals) {
  CIEC_LINT result;

  result = func_SUB(CIEC_SINT(-9), CIEC_DINT(9));
  BOOST_REQUIRE_EQUAL(static_cast<CIEC_LINT::TValueType>(CIEC_LINT(-18)), static_cast<CIEC_LINT::TValueType>(result));
}

BOOST_AUTO_TEST_CASE(Partial_Bool_NOT_Operation) {
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_BOOL result;

  result = func_NOT(lword.partial<CIEC_BOOL>(4));
  BOOST_REQUIRE_EQUAL(result, CIEC_BOOL(true));
}

BOOST_AUTO_TEST_CASE(Partial_Byte_NOT_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_BYTE result;

  result = func_NOT(lword.partial<CIEC_BYTE>(4));
  BOOST_REQUIRE_EQUAL(result, CIEC_BYTE(~0xA5));
}

BOOST_AUTO_TEST_CASE(Partial_DWord_NOT_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD result;

  result = func_NOT(lword.partial<CIEC_DWORD>(0));
  BOOST_REQUIRE_EQUAL(result, CIEC_DWORD(~0xA5A5A5A5));
}

BOOST_AUTO_TEST_CASE(Partial_Word_NOT_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_WORD result;

  result = func_NOT(lword.partial<CIEC_WORD>(0));
  BOOST_REQUIRE_EQUAL(result, CIEC_WORD(~0xA5A5));
}

BOOST_AUTO_TEST_CASE(Partial_LWord_EQ_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xA5A5A5A5);

  CIEC_BOOL result = func_EQ(lword.partial<CIEC_DWORD>(0), dword);
  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), result);
}

BOOST_AUTO_TEST_CASE(Both_Partial_EQ_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xA5A5A5A5);

  CIEC_BOOL result = func_EQ(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0));
  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), result);
}

BOOST_AUTO_TEST_CASE(Partial_LWord_NE_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  CIEC_BOOL result = func_NE(lword.partial<CIEC_DWORD>(0), dword);
  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), result);
}

BOOST_AUTO_TEST_CASE(Both_Partial_NE_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  CIEC_BOOL result = func_NE(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0));
  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), result);
}

BOOST_AUTO_TEST_CASE(Both_Partial_GT_Operation)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  BOOST_REQUIRE_EQUAL(CIEC_BOOL(false), func_GT(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0)));
}

BOOST_AUTO_TEST_CASE(Both_Partial_GE_Operations)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  BOOST_REQUIRE_EQUAL(CIEC_BOOL(false), func_GE(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0)));
}

BOOST_AUTO_TEST_CASE(Both_Partial_LT_Operations)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), func_LT(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0)));
}

BOOST_AUTO_TEST_CASE(Both_Partial_LE_Operations)
{
  CIEC_LWORD lword(0xA5A5A5A5A5A5A5A5);
  CIEC_DWORD dword(0xFEFEFEFE);

  BOOST_REQUIRE_EQUAL(CIEC_BOOL(true), func_LE(lword.partial<CIEC_BYTE>(0), dword.partial<CIEC_BYTE>(0)));
}

BOOST_AUTO_TEST_CASE(func_minus)
{
  CIEC_SINT sint(5);
  CIEC_INT integer(5);
  CIEC_DINT dint(5);
  CIEC_LINT lint(5);

  CIEC_REAL real(5.0f);
  CIEC_LREAL lreal(5.0);

  CIEC_TIME time(5);

  BOOST_REQUIRE(-5 == static_cast<CIEC_SINT::TValueType>(func_MINUS(sint)));
  BOOST_REQUIRE(-5 == static_cast<CIEC_INT::TValueType>(func_MINUS(integer)));
  BOOST_REQUIRE(-5 == static_cast<CIEC_DINT::TValueType>(func_MINUS(dint)));
  BOOST_REQUIRE(-5 == static_cast<CIEC_LINT::TValueType>(func_MINUS(lint)));

  BOOST_REQUIRE(-5.0f == static_cast<CIEC_REAL::TValueType>(func_MINUS(real)));
  BOOST_REQUIRE(-5.0 == static_cast<CIEC_LREAL::TValueType>(func_MINUS(lreal)));

  BOOST_REQUIRE(CIEC_TIME(-5) == func_MINUS(time));
}

BOOST_AUTO_TEST_CASE(func_concat_date_ulints) {
  CIEC_ULINT year(2017);
  CIEC_ULINT month(3);
  CIEC_ULINT day(20);
  CIEC_STRING dateString;
  CIEC_DATE expected;
  expected.fromString("DATE#2017-03-20");

  CIEC_DATE testDate = func_CONCAT_DATE(year, month, day);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_date_lints) {
  CIEC_LINT year(2017);
  CIEC_LINT month(3);
  CIEC_LINT day(20);
  CIEC_STRING dateString;
  CIEC_DATE expected;
  expected.fromString("DATE#2017-03-20");

  CIEC_DATE testDate = func_CONCAT_DATE(year, month, day);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_dt_ulints) {
  CIEC_ULINT year(2017);
  CIEC_ULINT month(3);
  CIEC_ULINT day(20);
  CIEC_ULINT hour(15);
  CIEC_ULINT minute(43);
  CIEC_ULINT second(13);
  CIEC_ULINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_DATE_AND_TIME expected;
  expected.fromString("DT#2017-03-20-15:43:13.574");

  CIEC_DATE_AND_TIME testDate = func_CONCAT_DT(year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_dt_lints) {
  CIEC_LINT year(2017);
  CIEC_LINT month(3);
  CIEC_LINT day(20);
  CIEC_LINT hour(15);
  CIEC_LINT minute(43);
  CIEC_LINT second(13);
  CIEC_LINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_DATE_AND_TIME expected;
  expected.fromString("DT#2017-03-20-15:43:13.574");

  CIEC_DATE_AND_TIME testDate = func_CONCAT_DT(year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_tod_ulints) {
  CIEC_ULINT hour(15);
  CIEC_ULINT minute(43);
  CIEC_ULINT second(13);
  CIEC_ULINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_TIME_OF_DAY expected;
  expected.fromString("TOD#15:43:13.574");

  CIEC_TIME_OF_DAY testTod = func_CONCAT_TOD(hour, minute, second, millisecond);
  BOOST_TEST(expected == testTod);
}

BOOST_AUTO_TEST_CASE(func_concat_tod_lints) {
  CIEC_LINT hour(15);
  CIEC_LINT minute(43);
  CIEC_LINT second(13);
  CIEC_LINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_TIME_OF_DAY expected;
  expected.fromString("15:43:13.574");

  CIEC_TIME_OF_DAY testTod = func_CONCAT_TOD(hour, minute, second, millisecond);
  BOOST_TEST(expected == testTod);
}

BOOST_AUTO_TEST_CASE(func_concat_ldate_ulints) {
  CIEC_ULINT year(2017);
  CIEC_ULINT month(3);
  CIEC_ULINT day(20);
  CIEC_STRING dateString;
  CIEC_LDATE expected;
  expected.fromString("LDATE#2017-03-20");

  CIEC_LDATE testDate = func_CONCAT_LDATE(year, month, day);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_ldate_lints) {
  CIEC_LINT year(2017);
  CIEC_LINT month(3);
  CIEC_LINT day(20);
  CIEC_STRING dateString;
  CIEC_LDATE expected;
  expected.fromString("LDATE#2017-03-20");

  CIEC_LDATE testDate = func_CONCAT_LDATE(year, month, day);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_ldt_ulints) {
  CIEC_ULINT year(2017);
  CIEC_ULINT month(3);
  CIEC_ULINT day(20);
  CIEC_ULINT hour(15);
  CIEC_ULINT minute(43);
  CIEC_ULINT second(13);
  CIEC_ULINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_LDATE_AND_TIME expected;
  expected.fromString("LDT#2017-03-20-15:43:13.574");

  CIEC_LDATE_AND_TIME testDate = func_CONCAT_LDT(year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_ldt_lints) {
  CIEC_LINT year(2017);
  CIEC_LINT month(3);
  CIEC_LINT day(20);
  CIEC_LINT hour(15);
  CIEC_LINT minute(43);
  CIEC_LINT second(13);
  CIEC_LINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_LDATE_AND_TIME expected;
  expected.fromString("LDT#2017-03-20-15:43:13.574");

  CIEC_LDATE_AND_TIME testDate = func_CONCAT_LDT(year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(expected == testDate);
}

BOOST_AUTO_TEST_CASE(func_concat_ltod_ulints) {
  CIEC_ULINT hour(15);
  CIEC_ULINT minute(43);
  CIEC_ULINT second(13);
  CIEC_ULINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_LTIME_OF_DAY expected;
  expected.fromString("LTOD#15:43:13.574");

  CIEC_LTIME_OF_DAY testTod = func_CONCAT_LTOD(hour, minute, second, millisecond);
  BOOST_TEST(expected == testTod);
}

BOOST_AUTO_TEST_CASE(func_concat_ltod_lints) {
  CIEC_LINT hour(15);
  CIEC_LINT minute(43);
  CIEC_LINT second(13);
  CIEC_LINT millisecond(574);
  CIEC_STRING dateString;
  CIEC_LTIME_OF_DAY expected;
  expected.fromString("15:43:13.574");

  CIEC_LTIME_OF_DAY testTod = func_CONCAT_LTOD(hour, minute, second, millisecond);
  BOOST_TEST(expected == testTod);
}

BOOST_AUTO_TEST_CASE(func_split_date_ulints) {
  CIEC_DATE date;
  date.fromString("DATE#2017-03-20");

  CIEC_ULINT year;
  CIEC_ULINT month;
  CIEC_ULINT day;

  func_SPLIT_DATE(date, year, month, day);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(day) == 20);
}

BOOST_AUTO_TEST_CASE(func_split_date_lints) {
  CIEC_DATE date;
  date.fromString("DATE#2017-03-20");

  CIEC_LINT year;
  CIEC_LINT month;
  CIEC_LINT day;

  func_SPLIT_DATE(date, year, month, day);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(day) == 20);
}

BOOST_AUTO_TEST_CASE(func_split_ldate_ulints) {
  CIEC_LDATE date;
  date.fromString("LDATE#2017-03-20");

  CIEC_ULINT year;
  CIEC_ULINT month;
  CIEC_ULINT day;

  func_SPLIT_LDATE(date, year, month, day);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(day) == 20);
}

BOOST_AUTO_TEST_CASE(func_split_ldate_lints) {
  CIEC_LDATE date;
  date.fromString("LDATE#2017-03-20");

  CIEC_LINT year;
  CIEC_LINT month;
  CIEC_LINT day;

  func_SPLIT_LDATE(date, year, month, day);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(day) == 20);
}

BOOST_AUTO_TEST_CASE(func_split_tod_ulints) {
  CIEC_TIME_OF_DAY given;
  given.fromString("TOD#15:43:13.574");

  CIEC_ULINT hour;
  CIEC_ULINT minute;
  CIEC_ULINT second;
  CIEC_ULINT millisecond;

  func_SPLIT_TOD(given, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_tod_lints) {
  CIEC_TIME_OF_DAY given;
  given.fromString("TOD#15:43:13.574");

  CIEC_LINT hour;
  CIEC_LINT minute;
  CIEC_LINT second;
  CIEC_LINT millisecond;

  func_SPLIT_TOD(given, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_ltod_ulints) {
  CIEC_LTIME_OF_DAY given;
  given.fromString("LTOD#15:43:13.574");

  CIEC_ULINT hour;
  CIEC_ULINT minute;
  CIEC_ULINT second;
  CIEC_ULINT millisecond;

  func_SPLIT_LTOD(given, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_ltod_lints) {
  CIEC_LTIME_OF_DAY given;
  given.fromString("LTOD#15:43:13.574");

  CIEC_LINT hour;
  CIEC_LINT minute;
  CIEC_LINT second;
  CIEC_LINT millisecond;

  func_SPLIT_LTOD(given, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_dt_ulints) {
  CIEC_DATE_AND_TIME given;
  given.fromString("DT#2017-03-20-15:43:13.574");

  CIEC_ULINT year;
  CIEC_ULINT month;
  CIEC_ULINT day;
  CIEC_ULINT hour;
  CIEC_ULINT minute;
  CIEC_ULINT second;
  CIEC_ULINT millisecond;

  func_SPLIT_DT(given, year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(day) == 20);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_dt_lints) {
  CIEC_DATE_AND_TIME given;
  given.fromString("DT#2017-03-20-15:43:13.574");

  CIEC_LINT year;
  CIEC_LINT month;
  CIEC_LINT day;
  CIEC_LINT hour;
  CIEC_LINT minute;
  CIEC_LINT second;
  CIEC_LINT millisecond;

  func_SPLIT_DT(given, year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(day) == 20);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_ldt_ulints) {
  CIEC_LDATE_AND_TIME given;
  given.fromString("LDT#2017-03-20-15:43:13.574");

  CIEC_ULINT year;
  CIEC_ULINT month;
  CIEC_ULINT day;
  CIEC_ULINT hour;
  CIEC_ULINT minute;
  CIEC_ULINT second;
  CIEC_ULINT millisecond;

  func_SPLIT_LDT(given, year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(day) == 20);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_split_ldt_lints) {
  CIEC_LDATE_AND_TIME given;
  given.fromString("LDT#2017-03-20-15:43:13.574");

  CIEC_LINT year;
  CIEC_LINT month;
  CIEC_LINT day;
  CIEC_LINT hour;
  CIEC_LINT minute;
  CIEC_LINT second;
  CIEC_LINT millisecond;

  func_SPLIT_LDT(given, year, month, day, hour, minute, second, millisecond);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(year) == 2017);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(month) == 3);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(day) == 20);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(hour) == 15);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(minute) == 43);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(second) == 13);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(millisecond) == 574);
}

BOOST_AUTO_TEST_CASE(func_day_of_week) {
  CIEC_DATE date;
  date.fromString("DATE#2017-03-20");

  CIEC_ULINT weekday;
  weekday = func_DAY_OF_WEEK(date);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(weekday) == 1);
}

BOOST_AUTO_TEST_CASE(func_to_little_endian) {
  CIEC_SINT sint(10);
  CIEC_INT inte(11);
  CIEC_DINT dint(12);
  CIEC_LINT lint(13);

  CIEC_USINT usint(14);
  CIEC_UINT uinte(15);
  CIEC_UDINT udint(16);
  CIEC_ULINT ulint(17);

  CIEC_SINT toLittleSint;
  CIEC_INT toLittleInt;
  CIEC_DINT toLittleDint;
  CIEC_LINT toLittleLint;

  CIEC_USINT toLittleUsint;
  CIEC_UINT toLittleUint;
  CIEC_UDINT toLittleUdint;
  CIEC_ULINT toLittleUlint;

  toLittleSint = func_TO_LITTLE_ENDIAN(sint);
  BOOST_TEST(static_cast<CIEC_SINT::TValueType>(toLittleSint) == 10);
  toLittleInt = func_TO_LITTLE_ENDIAN(inte);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(toLittleInt) == 11);
  toLittleDint = func_TO_LITTLE_ENDIAN(dint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(toLittleDint) == 12);
  toLittleLint = func_TO_LITTLE_ENDIAN(lint);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(toLittleLint) == 13);

  toLittleUsint = func_TO_LITTLE_ENDIAN(usint);
  BOOST_TEST(static_cast<CIEC_USINT::TValueType>(toLittleUsint) == 14);
  toLittleUint = func_TO_LITTLE_ENDIAN(uinte);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(toLittleUint) == 15);
  toLittleUdint = func_TO_LITTLE_ENDIAN(udint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(toLittleUdint) == 16);
  toLittleUlint = func_TO_LITTLE_ENDIAN(ulint);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(toLittleUlint) == 17);
}

BOOST_AUTO_TEST_CASE(func_from_little_endian) {
  CIEC_SINT sint(10);
  CIEC_INT inte(11);
  CIEC_DINT dint(12);
  CIEC_LINT lint(13);

  CIEC_USINT usint(14);
  CIEC_UINT uinte(15);
  CIEC_UDINT udint(16);
  CIEC_ULINT ulint(17);

  CIEC_SINT toLittleSint;
  CIEC_INT toLittleInt;
  CIEC_DINT toLittleDint;
  CIEC_LINT toLittleLint;

  CIEC_USINT toLittleUsint;
  CIEC_UINT toLittleUint;
  CIEC_UDINT toLittleUdint;
  CIEC_ULINT toLittleUlint;

  toLittleSint = func_FROM_LITTLE_ENDIAN(sint);
  BOOST_TEST(static_cast<CIEC_SINT::TValueType>(toLittleSint) == 10);
  toLittleInt = func_FROM_LITTLE_ENDIAN(inte);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(toLittleInt) == 11);
  toLittleDint = func_FROM_LITTLE_ENDIAN(dint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(toLittleDint) == 12);
  toLittleLint = func_FROM_LITTLE_ENDIAN(lint);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(toLittleLint) == 13);

  toLittleUsint = func_FROM_LITTLE_ENDIAN(usint);
  BOOST_TEST(static_cast<CIEC_USINT::TValueType>(toLittleUsint) == 14);
  toLittleUint = func_FROM_LITTLE_ENDIAN(uinte);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(toLittleUint) == 15);
  toLittleUdint = func_FROM_LITTLE_ENDIAN(udint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(toLittleUdint) == 16);
  toLittleUlint = func_FROM_LITTLE_ENDIAN(ulint);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(toLittleUlint) == 17);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian)
{
  CIEC_SINT sint(10);
  CIEC_INT inte(11);
  CIEC_DINT dint(12);
  CIEC_LINT lint(13);

  CIEC_USINT usint(14);
  CIEC_UINT uinte(15);
  CIEC_UDINT udint(16);
  CIEC_ULINT ulint(17);

  CIEC_BYTE byte(18);
  CIEC_WORD word(19);
  CIEC_DWORD dword(20);
  CIEC_LWORD lword(21);

  CIEC_SINT toSint;
  CIEC_INT toInt;
  CIEC_DINT toDint;
  CIEC_LINT toLint;

  CIEC_USINT toUsint;
  CIEC_UINT toUint;
  CIEC_UDINT toUdint;
  CIEC_ULINT toUlint;

  CIEC_BYTE toByte;
  CIEC_WORD toWord;
  CIEC_DWORD toDword;
  CIEC_LWORD toLword;

  toSint = func_TO_BIG_ENDIAN(sint);
  BOOST_TEST(static_cast<CIEC_SINT::TValueType>(toSint) == 10);
  toInt = func_TO_BIG_ENDIAN(inte);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(toInt) == 2816);
  toDint = func_TO_BIG_ENDIAN(dint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(toDint) == 201326592);
  toLint = func_TO_BIG_ENDIAN(lint);
  BOOST_TEST(static_cast<CIEC_LINT::TValueType>(toLint) == 936748722493063168);

  toUsint = func_TO_BIG_ENDIAN(usint);
  BOOST_TEST(static_cast<CIEC_USINT::TValueType>(toUsint) == 14);
  toUint = func_TO_BIG_ENDIAN(uinte);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(toUint) == 3840);
  toUdint = func_TO_BIG_ENDIAN(udint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(toUdint) == 268435456);
  toUlint = func_TO_BIG_ENDIAN(ulint);
  BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(toUlint) == 1224979098644774912);

  toByte = func_TO_BIG_ENDIAN(byte);
  BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(toByte) == 18);
  toWord = func_TO_BIG_ENDIAN(word);
  BOOST_TEST(static_cast<CIEC_WORD::TValueType>(toWord) == 4864);
  toDword = func_TO_BIG_ENDIAN(dword);
  BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(toDword) == 335544320);
  toLword = func_TO_BIG_ENDIAN(lword);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(toLword) == 1513209474796486656);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian_array_fixed) {
  CIEC_ARRAY_FIXED<CIEC_LWORD, -1, 1> originalArray = {CIEC_LWORD(1), CIEC_LWORD(2), CIEC_LWORD(3)};
  CIEC_ARRAY_FIXED<CIEC_LWORD, -1, 1> reversedArray;
  reversedArray = func_TO_BIG_ENDIAN(originalArray);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[-1]) == 72057594037927936);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[0]) == 144115188075855872);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[1]) == 216172782113783808);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian_array_variable) {
  CIEC_ARRAY_VARIABLE<CIEC_LWORD> originalArray = {CIEC_LWORD(1), CIEC_LWORD(2), CIEC_LWORD(3)};
  CIEC_ARRAY_VARIABLE<CIEC_LWORD> reversedArray(-1, 1);
  reversedArray = func_TO_BIG_ENDIAN(originalArray);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[-1]) == 72057594037927936);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[0]) == 144115188075855872);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[1]) == 216172782113783808);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian_array_variable_copy_ctor) {
  CIEC_ARRAY_VARIABLE<CIEC_LWORD> originalArray = {CIEC_LWORD(1), CIEC_LWORD(2), CIEC_LWORD(3)};
  CIEC_ARRAY_VARIABLE<CIEC_LWORD> reversedArray(func_TO_BIG_ENDIAN(originalArray));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[0]) == 72057594037927936);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[1]) == 144115188075855872);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reversedArray[2]) == 216172782113783808);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian_array_typelib_copy_ctor) {
  CIEC_ARRAY<CIEC_ANY> originalArray(3, g_nStringIdLWORD);
  originalArray[0].setValue(CIEC_LWORD(1));
  originalArray[1].setValue(CIEC_LWORD(2));
  originalArray[2].setValue(CIEC_LWORD(3));

  CIEC_ARRAY<CIEC_ANY> reversedArray(func_TO_BIG_ENDIAN(originalArray));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reinterpret_cast<CIEC_LWORD&>(reversedArray[0])) == 72057594037927936);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reinterpret_cast<CIEC_LWORD&>(reversedArray[1])) == 144115188075855872);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(reinterpret_cast<CIEC_LWORD&>(reversedArray[2])) == 216172782113783808);
}

BOOST_AUTO_TEST_CASE(func_to_big_endian_struct){
  CIEC_EndianessTestStruct original;

  (*static_cast<CIEC_BOOL *>(original.getMemberNamed(g_nStringIdVal1))) = CIEC_BOOL(true);
  (*static_cast<CIEC_DINT *>(original.getMemberNamed(g_nStringIdVal2))) = CIEC_DINT(55);
  (*static_cast<CIEC_LWORD *>(original.getMemberNamed(g_nStringIdVal3))) = CIEC_LWORD(65536UL);

  CIEC_EndianessTestStruct reversed;
  reversed = func_TO_BIG_ENDIAN(original);
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(*reinterpret_cast<CIEC_BOOL *>(reversed.getMemberNamed(g_nStringIdVal1))) == true);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(*reinterpret_cast<CIEC_DINT *>(reversed.getMemberNamed(g_nStringIdVal2))) == 922746880);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(*reinterpret_cast<CIEC_LWORD *>(reversed.getMemberNamed(g_nStringIdVal3))) == 1099511627776);
}

BOOST_AUTO_TEST_CASE(output_negation_bool_test){
  CIEC_BOOL inBool(false);
  CIEC_BOOL outBool(false);
  testSTInIsOutBoolDummyFunction(inBool, ST_EXTEND_LIFETIME(CIEC_ANY_BIT_NOT(outBool)));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(outBool) == true);
  inBool = CIEC_BOOL(true);
  testSTInIsOutBoolDummyFunction(inBool, ST_EXTEND_LIFETIME(CIEC_ANY_BIT_NOT(outBool)));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(outBool) == false);
}

BOOST_AUTO_TEST_CASE(output_negation_lword_test){
  CIEC_LWORD inLword(0xFEFEFEFEFEFEFEFE);
  CIEC_LWORD outLword(0);
  testSTInIsOutLWordDummyFunction(inLword, ST_EXTEND_LIFETIME(CIEC_ANY_BIT_NOT(outLword)));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(outLword) == 0x0101010101010101);
}

BOOST_AUTO_TEST_SUITE_END()
