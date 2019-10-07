/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
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
#include "../../../src/core/utils/parameterParser.h"
#include "../../../src/core/datatypes/forte_string.h"
#include <string.h>

BOOST_AUTO_TEST_SUITE(ParameterParser_Test)

  void testParameterParameterParser_NullString(CParameterParser& paParser) {
    BOOST_CHECK(1 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == paParser[1]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_NullString) {
    CParameterParser test1(0, ',');
    CParameterParser test2(0, ',', 0);
    CParameterParser test3(0, ',', 1);
    CParameterParser test4(0, ',', 20);

    testParameterParameterParser_NullString(test1);
    testParameterParameterParser_NullString(test2);
    testParameterParameterParser_NullString(test3);
    testParameterParameterParser_NullString(test4);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SourceStringRemainsUntouched) {
    CIEC_STRING stringTest = "123,456";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    BOOST_CHECK(2 == test1.parseParameters());
    BOOST_CHECK(0 == strcmp(stringTest.getValue(), "123,456"));

    BOOST_CHECK(2 == test2.parseParameters());
    BOOST_CHECK(0 == strcmp(stringTest.getValue(), "123,456"));

    BOOST_CHECK(2 == test3.parseParameters());
    BOOST_CHECK(0 == strcmp(stringTest.getValue(), "123,456"));

    BOOST_CHECK(2 == test4.parseParameters());
    BOOST_CHECK(0 == strcmp(stringTest.getValue(), "123,456"));
  }

  void testParameterParser_InitializedToZero(CParameterParser& paParser) {
    BOOST_CHECK(0 == paParser[0]);
    BOOST_CHECK(0 == paParser[1]);
    BOOST_CHECK(0 == paParser[2]);
    BOOST_CHECK(0 == paParser[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_InitializedToZero) {
    CIEC_STRING stringTest = "123,456";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_InitializedToZero(test1);
    testParameterParser_InitializedToZero(test2);
    testParameterParser_InitializedToZero(test3);
    testParameterParser_InitializedToZero(test4);
  }

  void testParameterParser_RegularParams(CParameterParser& paParser) {
    BOOST_CHECK(2 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "123"));
    BOOST_CHECK(0 == strcmp(paParser[1], "456"));
    BOOST_CHECK(0 == paParser[2]);
    BOOST_CHECK(0 == paParser[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_RegularParams) {
    CIEC_STRING stringTest = "123,456";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_RegularParams(test1);
    testParameterParser_RegularParams(test2);
    testParameterParser_RegularParams(test3);
    testParameterParser_RegularParams(test4);
  }

  void testParameterParser_EmptyString(CParameterParser& paParser) {
    BOOST_CHECK(1 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == paParser[1]);
    BOOST_CHECK(0 == paParser[2]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyString) {
    CIEC_STRING stringTest = "";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_EmptyString(test1);
    testParameterParser_EmptyString(test2);
    testParameterParser_EmptyString(test3);
    testParameterParser_EmptyString(test4);
  }

  void testParameterParser_BlankSpaces(CParameterParser& paParser) {
    BOOST_CHECK(3 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "1  2 3"));
    BOOST_CHECK(0 == strcmp(paParser[1], "4 56"));
    BOOST_CHECK(0 == strcmp(paParser[2], "789"));
    BOOST_CHECK(0 == paParser[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpaces) {
    CIEC_STRING stringTest = "  1  2 3 ,             4 56     ,       789              ";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_BlankSpaces(test1);
    testParameterParser_BlankSpaces(test2);
    testParameterParser_BlankSpaces(test3);
    testParameterParser_BlankSpaces(test4);
  }

  void testParameterParser_EmptyParameters(CParameterParser& paParser) {
    BOOST_CHECK(6 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], "123"));
    BOOST_CHECK(0 == strcmp(paParser[2], "456"));
    BOOST_CHECK(0 == strcmp(paParser[3], ""));
    BOOST_CHECK(0 == strcmp(paParser[4], "789"));
    BOOST_CHECK(0 == strcmp(paParser[5], ""));
    BOOST_CHECK(0 == paParser[6]);
    BOOST_CHECK(0 == paParser[7]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParameters) {
    CIEC_STRING stringTest = ",123,456,,789,";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_EmptyParameters(test1);
    testParameterParser_EmptyParameters(test2);
    testParameterParser_EmptyParameters(test3);
    testParameterParser_EmptyParameters(test4);
  }

  void testParameterParser_OtherSeparatorPresentInString(CParameterParser& paParser) {
    BOOST_CHECK(2 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "123, 4"));
    BOOST_CHECK(0 == strcmp(paParser[1], "6, 789"));
    BOOST_CHECK(0 == paParser[2]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorPresentInString) {
    CIEC_STRING stringTest = " 123, 456, 789   ";
    CParameterParser test1(stringTest.getValue(), '5');
    CParameterParser test2(stringTest.getValue(), '5', 2);
    CParameterParser test3(stringTest.getValue(), '5', 1);
    CParameterParser test4(stringTest.getValue(), '5', 20);

    testParameterParser_OtherSeparatorPresentInString(test1);
    testParameterParser_OtherSeparatorPresentInString(test2);
    testParameterParser_OtherSeparatorPresentInString(test3);
    testParameterParser_OtherSeparatorPresentInString(test4);
  }

  void testParameterParser_OtherSeparatorNonPresentInString(CParameterParser& paParser) {
    BOOST_CHECK(1 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "123, 456, 789"));
    BOOST_CHECK(0 == paParser[1]);;
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorNonPresentInString) {
    CIEC_STRING stringTest = " 123, 456, 789   ";
    CParameterParser test1(stringTest.getValue(), ';');
    CParameterParser test2(stringTest.getValue(), ';', 2);
    CParameterParser test3(stringTest.getValue(), ';', 1);
    CParameterParser test4(stringTest.getValue(), ';', 20);

    testParameterParser_OtherSeparatorNonPresentInString(test1);
    testParameterParser_OtherSeparatorNonPresentInString(test2);
    testParameterParser_OtherSeparatorNonPresentInString(test3);
    testParameterParser_OtherSeparatorNonPresentInString(test4);
  }

  void testParameterParser_BlankSpaceSeparator(CParameterParser& paParser) {
    BOOST_CHECK(7 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], "123,"));
    BOOST_CHECK(0 == strcmp(paParser[2], "456,"));
    BOOST_CHECK(0 == strcmp(paParser[3], "789"));
    BOOST_CHECK(0 == strcmp(paParser[4], ""));
    BOOST_CHECK(0 == strcmp(paParser[5], ""));
    BOOST_CHECK(0 == strcmp(paParser[6], ""));
    BOOST_CHECK(0 == paParser[7]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpaceSeparator) {
    CIEC_STRING stringTest = " 123, 456, 789   ";
    CParameterParser test1(stringTest.getValue(), ' ');
    CParameterParser test2(stringTest.getValue(), ' ', 2);
    CParameterParser test3(stringTest.getValue(), ' ', 1);
    CParameterParser test4(stringTest.getValue(), ' ', 20);

    testParameterParser_BlankSpaceSeparator(test1);
    testParameterParser_BlankSpaceSeparator(test2);
    testParameterParser_BlankSpaceSeparator(test3);
    testParameterParser_BlankSpaceSeparator(test4);
  }

  void testParameterParser_OnlyBlankSpace(CParameterParser& paParser) {
    BOOST_CHECK(1 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == paParser[1]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpace) {
    CIEC_STRING stringTest = "   "; //3 Blank Spaces
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_OnlyBlankSpace(test1);
    testParameterParser_OnlyBlankSpace(test2);
    testParameterParser_OnlyBlankSpace(test3);
    testParameterParser_OnlyBlankSpace(test4);
  }

  void testParameterParser_EmptyParametersWithBlankSpace(CParameterParser& paParser) {
    BOOST_CHECK(2 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], ""));
    BOOST_CHECK(0 == paParser[2]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpace) {
    CIEC_STRING stringTest = " , ";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_EmptyParametersWithBlankSpace(test1);
    testParameterParser_EmptyParametersWithBlankSpace(test2);
    testParameterParser_EmptyParametersWithBlankSpace(test3);
    testParameterParser_EmptyParametersWithBlankSpace(test4);
  }

  void testParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter(CParameterParser& paParser) {
    BOOST_CHECK(3 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], ""));
    BOOST_CHECK(0 == strcmp(paParser[2], ""));
    BOOST_CHECK(0 == paParser[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter) {
    CIEC_STRING stringTest = " , ,";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter(test1);
    testParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter(test2);
    testParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter(test3);
    testParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter(test4);
  }

  void testParameterParser_OnlySeparator(CParameterParser& paParser) {
    BOOST_CHECK(4 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], ""));
    BOOST_CHECK(0 == strcmp(paParser[2], ""));
    BOOST_CHECK(0 == strcmp(paParser[3], ""));
    BOOST_CHECK(0 == paParser[4]);
    BOOST_CHECK(0 == paParser[5]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlySeparator) {
    CIEC_STRING stringTest = ",,,";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_OnlySeparator(test1);
    testParameterParser_OnlySeparator(test2);
    testParameterParser_OnlySeparator(test3);
    testParameterParser_OnlySeparator(test4);
  }

  void testParameterParser_OnlyBlankSpaceSeparator(CParameterParser& paParser) {
    BOOST_CHECK(4 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], ""));
    BOOST_CHECK(0 == strcmp(paParser[1], ""));
    BOOST_CHECK(0 == strcmp(paParser[2], ""));
    BOOST_CHECK(0 == strcmp(paParser[3], ""));
    BOOST_CHECK(0 == paParser[4]);
    BOOST_CHECK(0 == paParser[5]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpaceSeparator) {
    CIEC_STRING stringTest = "   "; //3 Blank Spaces
    CParameterParser test1(stringTest.getValue(), ' ');
    CParameterParser test2(stringTest.getValue(), ' ', 2);
    CParameterParser test3(stringTest.getValue(), ' ', 1);
    CParameterParser test4(stringTest.getValue(), ' ', 20);

    testParameterParser_OnlyBlankSpaceSeparator(test1);
    testParameterParser_OnlyBlankSpaceSeparator(test2);
    testParameterParser_OnlyBlankSpaceSeparator(test3);
    testParameterParser_OnlyBlankSpaceSeparator(test4);
  }

  void testParameterParser_SerialCase1(CParameterParser& paParser) {
    BOOST_CHECK(6 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "COM1"));
    BOOST_CHECK(0 == strcmp(paParser[1], "19200"));
    BOOST_CHECK(0 == strcmp(paParser[2], "8"));
    BOOST_CHECK(0 == strcmp(paParser[3], "1"));
    BOOST_CHECK(0 == strcmp(paParser[4], "NONE"));
    BOOST_CHECK(0 == strcmp(paParser[5], "$n"));
    BOOST_CHECK(0 == paParser[6]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase1) {
    CIEC_STRING stringTest = "COM1, 19200, 8, 1, NONE, $n";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_SerialCase1(test1);
    testParameterParser_SerialCase1(test2);
    testParameterParser_SerialCase1(test3);
    testParameterParser_SerialCase1(test4);
  }

  void testParameterParser_SerialCase2(CParameterParser& paParser) {
    BOOST_CHECK(6 == paParser.parseParameters());

    BOOST_CHECK(0 == strcmp(paParser[0], "COM1"));
    BOOST_CHECK(0 == strcmp(paParser[1], "19200"));
    BOOST_CHECK(0 == strcmp(paParser[2], "8"));
    BOOST_CHECK(0 == strcmp(paParser[3], "1"));
    BOOST_CHECK(0 == strcmp(paParser[4], "NONE"));
    BOOST_CHECK(0 == strcmp(paParser[5], "$r$n"));
    BOOST_CHECK(0 == paParser[6]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase2) {
    CIEC_STRING stringTest = "COM1, 19200, 8, 1, NONE, $r$n";
    CParameterParser test1(stringTest.getValue(), ',');
    CParameterParser test2(stringTest.getValue(), ',', 2);
    CParameterParser test3(stringTest.getValue(), ',', 1);
    CParameterParser test4(stringTest.getValue(), ',', 20);

    testParameterParser_SerialCase2(test1);
    testParameterParser_SerialCase2(test2);
    testParameterParser_SerialCase2(test3);
    testParameterParser_SerialCase2(test4);
  }

  BOOST_AUTO_TEST_SUITE_END()
