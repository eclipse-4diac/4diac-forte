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

#include "iec61131_functions.h"

#include "forte_string.h"
#include "forte_uint.h"

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
  BOOST_TEST(nLength == 7);
}

BOOST_AUTO_TEST_CASE(len)
{
  CIEC_STRING sTestString("Lorem ipsum dolor sit amet");
  CIEC_UINT nLength(func_LEN(sTestString));
  BOOST_TEST(nLength == 26);
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
  BOOST_TEST(1 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_in_between)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit amet");
  CIEC_STRING sSearchString("dolor");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(13 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_at_the_end)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("t");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(21 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_not_found)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("Latin");
  CIEC_UINT nIndex;
  nIndex = CIEC_UINT(func_FIND(sBigString, sSearchString).getUnsignedValue());
  BOOST_TEST(0 == nIndex);
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
  bool bSame = forte::core::mpl::is_same<CIEC_BOOL, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_BOOL>::type>::value;
  BOOST_TEST(bSame);
  bSame = forte::core::mpl::is_same<CIEC_BYTE, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_BYTE>::type>::value;
  BOOST_TEST(bSame);
  bSame = forte::core::mpl::is_same<CIEC_WORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_WORD>::type>::value;
  BOOST_TEST(bSame);
  bSame = forte::core::mpl::is_same<CIEC_DWORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_DWORD>::type>::value;
  BOOST_TEST(bSame);
  bSame = forte::core::mpl::is_same<CIEC_LWORD, forte::core::mpl::implicit_cast<CIEC_BOOL, CIEC_LWORD>::type>::value;
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

//The following tests only work if we have C++ 11 or higher
#if __cplusplus > 201103L

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

#endif

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

  BOOST_REQUIRE_EQUAL(CIEC_SINT(50), func_TRUNC<CIEC_SINT>(real));
  BOOST_REQUIRE_EQUAL(CIEC_ULINT(50), func_TRUNC<CIEC_ULINT>(lreal));
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

BOOST_AUTO_TEST_CASE(mul_numbers)
{
  
  CIEC_USINT sint(5);
  CIEC_UINT inte(6);
  CIEC_UINT result;

  result = func_MUL(sint, inte);
  BOOST_REQUIRE_EQUAL(CIEC_UINT(30), result);
}

BOOST_AUTO_TEST_CASE(mul_number_and_time)
{
  CIEC_USINT sint(5);
  CIEC_TIME time(6);
  CIEC_TIME result;

  result = func_MUL(sint, time);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(30), result);
}

BOOST_AUTO_TEST_CASE(div_numbers)
{

  CIEC_USINT sint(30);
  CIEC_UINT inte(6);
  CIEC_UINT result;

  result = func_DIV(sint, inte);
  BOOST_REQUIRE_EQUAL(CIEC_UINT(5), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_time)
{
  CIEC_USINT sint(5);
  CIEC_TIME time(30);
  CIEC_TIME result;

  result = func_DIV(time, sint);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(6), result);
}

BOOST_AUTO_TEST_CASE(div_number_and_time_real)
{
  CIEC_REAL real(5);
  CIEC_TIME time(2000000);
  CIEC_TIME result;

  result = func_DIV(time, real);
  BOOST_REQUIRE_EQUAL(CIEC_TIME(400000), result);
}

BOOST_AUTO_TEST_CASE(Partial_Bool_NOT_Operation)
{
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

  BOOST_REQUIRE_EQUAL(CIEC_SINT(-5), func_MINUS(sint));
  BOOST_REQUIRE_EQUAL(CIEC_INT(-5), func_MINUS(integer));
  BOOST_REQUIRE_EQUAL(CIEC_DINT(-5), func_MINUS(dint));
  BOOST_REQUIRE_EQUAL(CIEC_LINT(-5), func_MINUS(lint));

  BOOST_REQUIRE_EQUAL(CIEC_REAL(-5.0f), func_MINUS(real));
  BOOST_REQUIRE_EQUAL(CIEC_LREAL(-5.0), func_MINUS(lreal));

  BOOST_REQUIRE_EQUAL(CIEC_TIME(-5), func_MINUS(time));
}

BOOST_AUTO_TEST_SUITE_END()
