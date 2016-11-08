/*******************************************************************************
* Copyright (c) 2011 - 2013 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*   Martin Melik Merkumians - initial API and implementation and/or initial documentation
*******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "iec61131_functions.h"

#include "forte_string.h"

BOOST_AUTO_TEST_SUITE(IEC61131_functions)

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

BOOST_AUTO_TEST_CASE(left)
{
	CIEC_STRING sTestString("SHALL_BE_CROPPED_HERE_THIS_SHOULD_NOT_BE_SEEN");
	CIEC_STRING sLeftString(LEFT(sTestString, CIEC_INT(21)));
	BOOST_TEST(sLeftString.getValue() == "SHALL_BE_CROPPED_HERE");
}

BOOST_AUTO_TEST_CASE(right)
{
	CIEC_STRING sTestString("THIS_SHOULD_BE_CROPPED_THIS_SHOULD_BE_SEEN");
	CIEC_STRING sRightString(RIGHT(sTestString, CIEC_INT(19)));
	BOOST_TEST(sRightString.getValue() == "THIS_SHOULD_BE_SEEN");
}

BOOST_AUTO_TEST_CASE(concat)
{
	CIEC_STRING sFristString("THIS_IS_THE_FIRST_STRING");
	CIEC_STRING sSecondString("_THIS_IS_THE_SECOND_STRING");
	CIEC_STRING sConcatString(CONCAT(sFristString, sSecondString));
	BOOST_TEST(sConcatString.getValue() == "THIS_IS_THE_FIRST_STRING_THIS_IS_THE_SECOND_STRING");
}

BOOST_AUTO_TEST_SUITE_END()