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

#include "cwin32sercomparameterparser.h"

#include "forte_string.h"

BOOST_AUTO_TEST_SUITE(Win32SerComParameterParser)

BOOST_AUTO_TEST_CASE(ParameterParserParameterAmount)
{
	BOOST_TEST(CWin32SerComParameterParser::ESerComParameter::eSerComParamterAmount == 6);
}

BOOST_AUTO_TEST_CASE(ParameterParsingCase1)
{
	CIEC_STRING sParameterString;
	sParameterString.fromString("COM1, 19200, 8, 1, NONE, $n");
	char *pcParameterString = sParameterString.getValue();
	CWin32SerComParameterParser parser(pcParameterString);
	parser.setSeparator(',');
	parser.parseParameters();
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eInterface] == "COM1");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eBaudrate] == "19200");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eByteSize] == "8");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eStopBits] == "1");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eParity] == "NONE");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eTerminationSymbol] == "$n");
}

BOOST_AUTO_TEST_CASE(ParameterParsingCase2)
{
	CIEC_STRING sParameterString;
	sParameterString.fromString("COM1, 19200, 8, 1, NONE, $r$n");
	char *pcParameterString = sParameterString.getValue();
	CWin32SerComParameterParser parser(pcParameterString);
	parser.setSeparator(',');
	parser.parseParameters();
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eInterface] == "COM1");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eBaudrate] == "19200");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eByteSize] == "8");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eStopBits] == "1");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eParity] == "NONE");
	BOOST_TEST(parser[CWin32SerComParameterParser::ESerComParameter::eTerminationSymbol] == "$r$n");
}

BOOST_AUTO_TEST_SUITE_END()