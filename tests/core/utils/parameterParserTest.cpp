/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../../src/core/utils/parameterParser.h"
#include "../../../src/core/datatypes/forte_string.h"
#include <string.h>


// For most cases, the test case checks the boundaries of the number of
// arguments specified, or the actual amount of parameter in the string
// E.g.: If 3 arguments were specified in the constructor,
// it will be checked for index 2,3 and 4, event though only up to index 2
// there should be something. In the case the string has 4 arguments, index
// up to 5 should be checked, to make sure the parser is not parsing more than
// asked
BOOST_AUTO_TEST_SUITE(ParameterParser_Test)

void testNullString(CParameterParser& paParser) {
  BOOST_CHECK(1 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == paParser[1]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_NullString){
  CParameterParser test1(0, 2, ',');
  CParameterParser test2(0, ',');

  testNullString (test1);
  testNullString (test2);
  }


  BOOST_AUTO_TEST_CASE(ParameterParser_SourceStringRemainsUntouched){
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 2, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  BOOST_CHECK(2 == test1.parseParameters());
  BOOST_CHECK(2 == test2.parseParameters());

  BOOST_CHECK(0 == strcmp(stringTest.getValue(), "123,456"));
  }

void testInitializedToZero(CParameterParser& paParser) {
  BOOST_CHECK(0 == paParser[0]);
  BOOST_CHECK(0 == paParser[1]);
  BOOST_CHECK(0 == paParser[2]);
  BOOST_CHECK(0 == paParser[3]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_InitializedToZero){
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 2, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testInitializedToZero (test1);
  testInitializedToZero (test2);
  }

BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsZeroArgumentsSpecified) {
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 0, ',');

  BOOST_CHECK(0 == test1.parseParameters());

  BOOST_CHECK(0 == test1[0]);
  BOOST_CHECK(0 == test1[1]);

  }

BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsTooFewArgumentsSpecified) {
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 1, ',');

  BOOST_CHECK(1 == test1.parseParameters());

  BOOST_CHECK(0 == strcmp(test1[0], "123"));
  BOOST_CHECK(0 == test1[1]);
  BOOST_CHECK(0 == test1[2]);

  }

void testRegularParamsRightArgumentsSpecified(CParameterParser& paParser) {
  BOOST_CHECK(2 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "123"));
  BOOST_CHECK(0 == strcmp(paParser[1], "456"));
  BOOST_CHECK(0 == paParser[2]);
  BOOST_CHECK(0 == paParser[3]);
}

BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsRightArgumentsSpecified) {
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 2, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testRegularParamsRightArgumentsSpecified (test1);
  testRegularParamsRightArgumentsSpecified (test2);

  }

BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsTooManyArgumentsSpecified) {
  CIEC_STRING stringTest = "123,456";
  CParameterParser test1(stringTest.getValue(), 3, ',');

  BOOST_CHECK(2 == test1.parseParameters());

  BOOST_CHECK(0 == strcmp(test1[0], "123"));
  BOOST_CHECK(0 == strcmp(test1[1], "456"));
  BOOST_CHECK(0 == test1[3]);

  }

BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringZeroArgumentsSpecified) {
  CIEC_STRING stringTest = "";
  CParameterParser test1(stringTest.getValue(), 0, ',');

  BOOST_CHECK(0 == test1.parseParameters());

  BOOST_CHECK(0 == test1[0]);
  BOOST_CHECK(0 == test1[1]);

  }

void testEmptyStringRightArgumentsSpecified(CParameterParser& paParser) {
  BOOST_CHECK(1 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == paParser[1]);
  BOOST_CHECK(0 == paParser[2]);
}

BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringRightArgumentsSpecified) {
  CIEC_STRING stringTest = "";
  CParameterParser test1(stringTest.getValue(), 1, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testEmptyStringRightArgumentsSpecified (test1);
  testEmptyStringRightArgumentsSpecified (test2);
  }


  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringTooManyArgumentsSpecified){
  CIEC_STRING stringTest = "";
  CParameterParser test1(stringTest.getValue(), 2, ',');

  BOOST_CHECK(1 == test1.parseParameters());

  BOOST_CHECK(0 == strcmp(test1[0], ""));
  BOOST_CHECK(0 == test1[1]);
  BOOST_CHECK(0 == test1[2]);
  BOOST_CHECK(0 == test1[3]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesZeroArgumentsSpecified){
  CIEC_STRING stringTest = "  1  2 3 ,             4 56     ,       789              ";
  CParameterParser test1(stringTest.getValue(), 0, ',');

  BOOST_CHECK(0 == test1.parseParameters());

  BOOST_CHECK(0 == test1[0]);
  BOOST_CHECK(0 == test1[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesTooFewArgumentsSpecified){
  CIEC_STRING stringTest = "  1  2 3 ,             4 56     ,       789              ";
  CParameterParser test1(stringTest.getValue(), 2, ',');

  BOOST_CHECK(2 == test1.parseParameters());

  BOOST_CHECK(0 == strcmp(test1[0], "1  2 3"));
  BOOST_CHECK(0 == strcmp(test1[1], "4 56"));
  BOOST_CHECK(0 == test1[2]);
  BOOST_CHECK(0 == test1[3]);

  }

void testBlankSpacesRightArgumentsSpecified(CParameterParser& paParser) {
  BOOST_CHECK(3 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "1  2 3"));
  BOOST_CHECK(0 == strcmp(paParser[1], "4 56"));
  BOOST_CHECK(0 == strcmp(paParser[2], "789"));
  BOOST_CHECK(0 == paParser[3]);

  }

BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesRightArgumentsSpecified) {
  CIEC_STRING stringTest = "  1  2 3 ,             4 56     ,       789              ";
  CParameterParser test1(stringTest.getValue(), 3, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testBlankSpacesRightArgumentsSpecified (test1);
  testBlankSpacesRightArgumentsSpecified (test2);

}

BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesTooManyArgumentsSpecified) {
  CIEC_STRING stringTest = "  1  2 3 ,             4 56     ,       789              ";
  CParameterParser test1(stringTest.getValue(), 4, ',');

  BOOST_CHECK(3 == test1.parseParameters());

  BOOST_CHECK(0 == strcmp(test1[0], "1  2 3"));
  BOOST_CHECK(0 == strcmp(test1[1], "4 56"));
  BOOST_CHECK(0 == strcmp(test1[2], "789"));
  BOOST_CHECK(0 == test1[3]);
  BOOST_CHECK(0 == test1[4]);

}

void testEmptyParameters(CParameterParser& paParser) {
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

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParameters){
  CIEC_STRING stringTest = ",123,456,,789,";
  CParameterParser test1(stringTest.getValue(), 7, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testEmptyParameters (test1);
  testEmptyParameters (test2);
  }

void testOtherSeparatorPresentInString(CParameterParser& paParser) {
  BOOST_CHECK(2 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "123, 4"));
  BOOST_CHECK(0 == strcmp(paParser[1], "6, 789"));
  BOOST_CHECK(0 == paParser[2]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorPresentInString){
  CIEC_STRING stringTest = " 123, 456, 789   ";
  CParameterParser test1(stringTest.getValue(), 2, '5');
  CParameterParser test2(stringTest.getValue(), '5');

  testOtherSeparatorPresentInString (test1);
  testOtherSeparatorPresentInString (test2);

  }

void testOtherSeparatorNonPresentInString(CParameterParser& paParser) {
  BOOST_CHECK(1 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "123, 456, 789"));
  BOOST_CHECK(0 == paParser[1]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorNonPresentInString){
  CIEC_STRING stringTest = " 123, 456, 789   ";
  CParameterParser test1(stringTest.getValue(), 1, ';');
  CParameterParser test2(stringTest.getValue(), ';');

  testOtherSeparatorNonPresentInString (test1);
  testOtherSeparatorNonPresentInString (test2);
  }

void testBlankSpaceSeparator(CParameterParser& paParser) {
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

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpaceSeparator){
  CIEC_STRING stringTest = " 123, 456, 789   ";
  CParameterParser test1(stringTest.getValue(), 8, ' ');
  CParameterParser test2(stringTest.getValue(), ' ');

  testBlankSpaceSeparator (test1);
  testBlankSpaceSeparator (test2);
  }

void testOnlyBlankSpace(CParameterParser& paParser) {
  BOOST_CHECK(1 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == paParser[1]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpace){
  CIEC_STRING stringTest = "   "; //3 Blank Spaces
  CParameterParser test1(stringTest.getValue(), 4, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testOnlyBlankSpace (test1);
  testOnlyBlankSpace (test2);

  }

void testEmptyParametersWithBlankSpace(CParameterParser& paParser) {
  BOOST_CHECK(2 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == strcmp(paParser[1], ""));
  BOOST_CHECK(0 == paParser[2]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpace){
  CIEC_STRING stringTest = " , ";
  CParameterParser test1(stringTest.getValue(), 4, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testEmptyParametersWithBlankSpace (test1);
  testEmptyParametersWithBlankSpace (test2);
   }

void testEmptyParametersWithBlankSpaceAndEndingInParameter(CParameterParser& paParser) {
  BOOST_CHECK(3 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == strcmp(paParser[1], ""));
  BOOST_CHECK(0 == strcmp(paParser[2], ""));
  BOOST_CHECK(0 == paParser[3]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter){
  CIEC_STRING stringTest = " , ,";
  CParameterParser test1(stringTest.getValue(), 4, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testEmptyParametersWithBlankSpaceAndEndingInParameter (test1);
  testEmptyParametersWithBlankSpaceAndEndingInParameter (test2);
   }

void testOnlySeparator(CParameterParser& paParser) {
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
  CParameterParser test1(stringTest.getValue(), 5, ',');
  CParameterParser test2(stringTest.getValue(), ',');

  testOnlySeparator (test1);
  testOnlySeparator (test2);

}

void testOnlyBlankSpaceSeparator(CParameterParser& paParser) {
  BOOST_CHECK(4 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], ""));
  BOOST_CHECK(0 == strcmp(paParser[1], ""));
  BOOST_CHECK(0 == strcmp(paParser[2], ""));
  BOOST_CHECK(0 == strcmp(paParser[3], ""));
  BOOST_CHECK(0 == paParser[4]);
  BOOST_CHECK(0 == paParser[5]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpaceSeparator){
  CIEC_STRING stringTest = "   "; //3 Blank Spaces
  CParameterParser test1(stringTest.getValue(), 5, ' ');
  CParameterParser test2(stringTest.getValue(), ' ');

  testOnlyBlankSpaceSeparator (test1);
  testOnlyBlankSpaceSeparator (test2);
  }

void testSerialCase1(CParameterParser& paParser) {
  BOOST_CHECK(6 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "COM1"));
  BOOST_CHECK(0 == strcmp(paParser[1], "19200"));
  BOOST_CHECK(0 == strcmp(paParser[2], "8"));
  BOOST_CHECK(0 == strcmp(paParser[3], "1"));
  BOOST_CHECK(0 == strcmp(paParser[4], "NONE"));
  BOOST_CHECK(0 == strcmp(paParser[5], "$n"));
  BOOST_CHECK(0 == paParser[6]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase1)
  {
    CIEC_STRING sParameterString = "COM1, 19200, 8, 1, NONE, $n";
  CParameterParser test1(sParameterString.getValue(), 6, ',');
  CParameterParser test2(sParameterString.getValue(), ',');

  testSerialCase1 (test1);
  testSerialCase1 (test2);
  }

void testSerialCase2(CParameterParser& paParser) {
  BOOST_CHECK(6 == paParser.parseParameters());

  BOOST_CHECK(0 == strcmp(paParser[0], "COM1"));
  BOOST_CHECK(0 == strcmp(paParser[1], "19200"));
  BOOST_CHECK(0 == strcmp(paParser[2], "8"));
  BOOST_CHECK(0 == strcmp(paParser[3], "1"));
  BOOST_CHECK(0 == strcmp(paParser[4], "NONE"));
  BOOST_CHECK(0 == strcmp(paParser[5], "$r$n"));
  BOOST_CHECK(0 == paParser[6]);
}

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase2)
  {
    CIEC_STRING sParameterString = "COM1, 19200, 8, 1, NONE, $r$n";
  CParameterParser test1(sParameterString.getValue(), 6, ',');
  CParameterParser test2(sParameterString.getValue(), ',');

  testSerialCase2 (test1);
  testSerialCase2 (test2);
  }

BOOST_AUTO_TEST_SUITE_END()
