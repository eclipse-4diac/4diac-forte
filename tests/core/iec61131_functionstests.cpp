/*******************************************************************************
* Copyright (c) 2011 - 2013, 2018 TU Vienna/ACIN, nxtControl, Profactor GmbH, fortiss GmbH
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*   Martin Melik Merkumians - initial API and implementation and/or initial documentation
*   Martin Melik-Merkumians - adds test for FIND
*******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "iec61131_functions.h"

#include "forte_string.h"
#include "forte_uint.h"

BOOST_AUTO_TEST_SUITE(IEC61131_functions)

BOOST_AUTO_TEST_CASE(abs)
{
  CIEC_SINT nNumber(-13);
  BOOST_TEST(ABS(nNumber).getSignedValue() == 13);
}

BOOST_AUTO_TEST_CASE(sqrt)
{
  CIEC_REAL nNumber(4);
  BOOST_TEST(static_cast<TForteFloat>(SQRT(nNumber)) == 2.0f);
}

BOOST_AUTO_TEST_CASE(to_upper)
{
  CIEC_STRING sTestString("shall_be_to_upper");
  CIEC_STRING sToUpperString(TOUPPER(sTestString));
  BOOST_TEST(sToUpperString.getValue() == "SHALL_BE_TO_UPPER");
}

BOOST_AUTO_TEST_CASE(to_lower)
{
  CIEC_STRING sTestString("SHALL_BE_TO_LOWER");
  CIEC_STRING sToLowerString(TOLOWER(sTestString));
  BOOST_TEST(sToLowerString.getValue() == "shall_be_to_lower");
}

BOOST_AUTO_TEST_CASE(standard_example_len)
{
  CIEC_STRING sTestString("ASTRING");
  CIEC_UINT nLength(LEN(sTestString));
  BOOST_TEST(nLength == 7);
}

BOOST_AUTO_TEST_CASE(len)
{
  CIEC_STRING sTestString("Lorem ipsum dolor sit amet");
  CIEC_UINT nLength(LEN(sTestString));
  BOOST_TEST(nLength == 26);
}

BOOST_AUTO_TEST_CASE(standard_example_left)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sLeftString(LEFT(sTestString, CIEC_INT(3)));
  BOOST_TEST(sLeftString.getValue() == "AST");
}

BOOST_AUTO_TEST_CASE(left)
{
  CIEC_STRING sTestString("SHALL_BE_CROPPED_HERE_THIS_SHOULD_NOT_BE_SEEN");
  CIEC_STRING sLeftString(LEFT(sTestString, CIEC_INT(21)));
  BOOST_TEST(sLeftString.getValue() == "SHALL_BE_CROPPED_HERE");
}

BOOST_AUTO_TEST_CASE(standard_example_right)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sRightString(RIGHT(sTestString, CIEC_INT(3)));
  BOOST_TEST(sRightString.getValue() == "STR");
}

BOOST_AUTO_TEST_CASE(right)
{
  CIEC_STRING sTestString("THIS_SHOULD_BE_CROPPED_THIS_SHOULD_BE_SEEN");
  CIEC_STRING sRightString(RIGHT(sTestString, CIEC_INT(19)));
  BOOST_TEST(sRightString.getValue() == "THIS_SHOULD_BE_SEEN");
}

BOOST_AUTO_TEST_CASE(standard_example_mid)
{
  CIEC_STRING sTestString("ASTR");
  CIEC_STRING sMidString(MID(sTestString, CIEC_INT(2), CIEC_INT(2)));
  BOOST_TEST(sMidString.getValue() == "ST");
}

BOOST_AUTO_TEST_CASE(mid)
{
  CIEC_STRING sTestString("THIS_SHOULD_NOT_BE_SEEN_THIS_SHALL_BE_SEEN_THIS_SHOULD_NOT_BE_SEEN");
  CIEC_STRING sMidString(MID(sTestString,CIEC_INT(18), CIEC_INT(25)));
  BOOST_TEST(sMidString.getValue() == "THIS_SHALL_BE_SEEN");
}

BOOST_AUTO_TEST_CASE(concat)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sConcatString(CONCAT(sFristString, sSecondString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING");
}

BOOST_AUTO_TEST_CASE(find_at_begin)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit amet");
  CIEC_STRING sSearchString("Lorem");
  CIEC_UINT nIndex;
  nIndex = FIND(sBigString, sSearchString).getUnsignedValue();
  BOOST_TEST(1 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_in_between)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit amet");
  CIEC_STRING sSearchString("dolor");
  CIEC_UINT nIndex;
  nIndex = FIND(sBigString, sSearchString).getUnsignedValue();
  BOOST_TEST(13 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_at_the_end)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("t");
  CIEC_UINT nIndex;
  nIndex = FIND(sBigString, sSearchString).getUnsignedValue();
  BOOST_TEST(21 == nIndex);
}

BOOST_AUTO_TEST_CASE(find_not_found)
{
  CIEC_STRING sBigString("Lorem ipsum dolor sit");
  CIEC_STRING sSearchString("Latin");
  CIEC_UINT nIndex;
  nIndex = FIND(sBigString, sSearchString).getUnsignedValue();
  BOOST_TEST(0 == nIndex);
}

BOOST_AUTO_TEST_CASE(standard_example_replace)
{
  CIEC_STRING sIn1("ABCDE");
  CIEC_STRING sIn2("X");
  CIEC_STRING sResult(REPLACE(sIn1, sIn2, CIEC_INT(2), CIEC_INT(3)));
  BOOST_TEST(sResult.getValue() == "ABXE");
}

BOOST_AUTO_TEST_CASE(replace)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sIn2("muspi");
  CIEC_STRING sResult(REPLACE(sIn1, sIn2, CIEC_INT(5), CIEC_INT(7)));
  BOOST_TEST(sResult.getValue() == "Lorem muspi dolor sit");
}

BOOST_AUTO_TEST_CASE(standard_example_delete)
{
  CIEC_STRING sIn1("ABXYC");
  CIEC_STRING sResult(DELETE(sIn1, CIEC_INT(2), CIEC_INT(3)));
  BOOST_TEST(sResult.getValue() == "ABC");
}

BOOST_AUTO_TEST_CASE(delete_function)
{
  CIEC_STRING sIn1("Lorem ipsum dolor sit");
  CIEC_STRING sResult(DELETE(sIn1, CIEC_INT(6), CIEC_INT(12)));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum sit");
}

BOOST_AUTO_TEST_CASE(eq_true)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(EQ(nInt1, nInt2) == true);
}

BOOST_AUTO_TEST_CASE(eq_false)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(EQ(nInt1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_true)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(10);
  BOOST_TEST(NE(nInt1, nInt2) == false);
}

BOOST_AUTO_TEST_CASE(ne_false)
{
  CIEC_INT nInt1(10);
  CIEC_INT nInt2(-10);
  BOOST_TEST(NE(nInt1, nInt2) == true);
}

#if __cplusplus > 201103L

BOOST_AUTO_TEST_CASE(and_function)
{
  CIEC_BYTE nByte(10);
  CIEC_DWORD nDword(30010);
  CIEC_DWORD nResult(AND(nByte, nDword));
  BOOST_TEST(static_cast<TForteDWord>(nResult) == 10);
}

//The following tests only work if we have C++ 11 or higher
BOOST_AUTO_TEST_CASE(concat3)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sThirdString("_THIS_IS_THE_THIRD_STRING");
  CIEC_STRING sConcatString(CONCAT(sFristString, sSecondString, sThirdString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING_THIS_IS_THE_THIRD_STRING");
}

BOOST_AUTO_TEST_CASE(concat4)
{
  CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
  CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
  CIEC_STRING sThirdString("_THIS_IS_THE_THIRD_STRING");
  CIEC_STRING sForthString("_THIS_IS_THE_FORTH_STRING");
  CIEC_STRING sConcatString(CONCAT(sFristString, sSecondString, sThirdString, sForthString));
  BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING_THIS_IS_THE_THIRD_STRING_THIS_IS_THE_FORTH_STRING");
}

#endif

BOOST_AUTO_TEST_CASE(standard_example_insert)
{
  CIEC_STRING sIn1("ABC");
  CIEC_STRING sIn2("XY");
  CIEC_STRING sResult = INSERT(sIn1, sIn2, CIEC_UINT(2));
  BOOST_TEST(sResult.getValue() == "ABXYC");
}

BOOST_AUTO_TEST_CASE(insert)
{
  CIEC_STRING sIn1("Lorem  sit");
  CIEC_STRING sIn2("ipsum dolor");
  CIEC_STRING sResult = INSERT(sIn1, sIn2, CIEC_UINT(6));
  BOOST_TEST(sResult.getValue() == "Lorem ipsum dolor sit");
}

BOOST_AUTO_TEST_SUITE_END()
