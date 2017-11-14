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

  BOOST_AUTO_TEST_CASE(ParameterParser_NullString){
    CParameterParser test2(0, 2);

    BOOST_CHECK(1 == test2.parseParameters());

    BOOST_CHECK(0 == strcmp(test2[0], ""));
    BOOST_CHECK(0 == test2[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SourceStringRemainsUntouched){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test2(emptyTest.getValue(), 2);

    BOOST_CHECK(2 == test2.parseParameters());
    BOOST_CHECK(0 == strcmp(emptyTest.getValue(), "123,456"));

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_InitializedToZero){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test2(emptyTest.getValue(), 2);

    BOOST_CHECK(0 == test2[0]);
    BOOST_CHECK(0 == test2[1]);
    BOOST_CHECK(0 == test2[2]);
    BOOST_CHECK(0 == test2[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsZeroArgumentsSpecified){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test0(emptyTest.getValue(), 0);

    BOOST_CHECK(0 == test0.parseParameters());

    BOOST_CHECK(0 == test0[0]);
    BOOST_CHECK(0 == test0[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsTooFewArgumentsSpecified){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test1(emptyTest.getValue(), 1);

    BOOST_CHECK(1 == test1.parseParameters());

    BOOST_CHECK(0 == strcmp(test1[0], "123"));
    BOOST_CHECK(0 == test1[1]);
    BOOST_CHECK(0 == test1[2]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsRightArgumentsSpecified){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test2(emptyTest.getValue(), 2);

    BOOST_CHECK(2 == test2.parseParameters());

    BOOST_CHECK(0 == strcmp(test2[0], "123"));
    BOOST_CHECK(0 == strcmp(test2[1], "456"));
    BOOST_CHECK(0 == test2[2]);
    BOOST_CHECK(0 == test2[3]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_RegularParamsTooManyArgumentsSpecified){
    CIEC_STRING emptyTest = "123,456";
    CParameterParser test3(emptyTest.getValue(), 3);

    BOOST_CHECK(2 == test3.parseParameters());

    BOOST_CHECK(0 == strcmp(test3[0], "123"));
    BOOST_CHECK(0 == strcmp(test3[1], "456"));
    BOOST_CHECK(0 == test3[3]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringZeroArgumentsSpecified){
    CIEC_STRING emptyTest = "";
    CParameterParser test0(emptyTest.getValue(), 0);

    BOOST_CHECK(0 == test0.parseParameters());

    BOOST_CHECK(0 == test0[0]);
    BOOST_CHECK(0 == test0[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringRightArgumentsSpecified){
    CIEC_STRING emptyTest = "";
    CParameterParser test1(emptyTest.getValue(), 1);

    BOOST_CHECK(1 == test1.parseParameters());

    BOOST_CHECK(0 == strcmp(test1[0], ""));
    BOOST_CHECK(0 == test1[1]);
    BOOST_CHECK(0 == test1[2]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyStringTooManyArgumentsSpecified){
    CIEC_STRING emptyTest = "";
    CParameterParser test2(emptyTest.getValue(), 2);

    BOOST_CHECK(1 == test2.parseParameters());

    BOOST_CHECK(0 == strcmp(test2[0], ""));
    BOOST_CHECK(0 == test2[1]);
    BOOST_CHECK(0 == test2[2]);
    BOOST_CHECK(0 == test2[3]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesZeroArgumentsSpecified){
    CIEC_STRING emptyTest = "  1  2 3 ,             4 56     ,       789              ";
    CParameterParser test0(emptyTest.getValue(), 0);

    BOOST_CHECK(0 == test0.parseParameters());

    BOOST_CHECK(0 == test0[0]);
    BOOST_CHECK(0 == test0[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesTooFewArgumentsSpecified){
    CIEC_STRING emptyTest = "  1  2 3 ,             4 56     ,       789              ";
    CParameterParser test2(emptyTest.getValue(), 2);

    BOOST_CHECK(2 == test2.parseParameters());

    BOOST_CHECK(0 == strcmp(test2[0], "1  2 3"));
    BOOST_CHECK(0 == strcmp(test2[1], "4 56"));
    BOOST_CHECK(0 == test2[2]);
    BOOST_CHECK(0 == test2[3]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesRightArgumentsSpecified){
    CIEC_STRING emptyTest = "  1  2 3 ,             4 56     ,       789              ";
    CParameterParser test3(emptyTest.getValue(), 3);

    BOOST_CHECK(3 == test3.parseParameters());

    BOOST_CHECK(0 == strcmp(test3[0], "1  2 3"));
    BOOST_CHECK(0 == strcmp(test3[1], "4 56"));
    BOOST_CHECK(0 == strcmp(test3[2], "789"));
    BOOST_CHECK(0 == test3[3]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpacesTooManyArgumentsSpecified){
    CIEC_STRING emptyTest = "  1  2 3 ,             4 56     ,       789              ";
    CParameterParser test4(emptyTest.getValue(), 4);

    BOOST_CHECK(3 == test4.parseParameters());

    BOOST_CHECK(0 == strcmp(test4[0], "1  2 3"));
    BOOST_CHECK(0 == strcmp(test4[1], "4 56"));
    BOOST_CHECK(0 == strcmp(test4[2], "789"));
    BOOST_CHECK(0 == test4[3]);
    BOOST_CHECK(0 == test4[4]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParameters){
    CIEC_STRING emptyTest = ",123,456,,789,";
    CParameterParser test7(emptyTest.getValue(), 7);

    BOOST_CHECK(6 == test7.parseParameters());

    BOOST_CHECK(0 == strcmp(test7[0], ""));
    BOOST_CHECK(0 == strcmp(test7[1], "123"));
    BOOST_CHECK(0 == strcmp(test7[2], "456"));
    BOOST_CHECK(0 == strcmp(test7[3], ""));
    BOOST_CHECK(0 == strcmp(test7[4], "789"));
    BOOST_CHECK(0 == strcmp(test7[5], ""));
    BOOST_CHECK(0 == test7[6]);
    BOOST_CHECK(0 == test7[7]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorPresentInString){
    CIEC_STRING emptyTest = " 123, 456, 789   ";
    CParameterParser test2(emptyTest.getValue(), 2, '5');

    BOOST_CHECK(2 == test2.parseParameters());

    BOOST_CHECK(0 == strcmp(test2[0], "123, 4"));
    BOOST_CHECK(0 == strcmp(test2[1], "6, 789"));
    BOOST_CHECK(0 == test2[2]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OtherSeparatorNonPresentInString){
    CIEC_STRING emptyTest = " 123, 456, 789   ";
    CParameterParser test1(emptyTest.getValue(), 1, ';');

    BOOST_CHECK(1 == test1.parseParameters());

    BOOST_CHECK(0 == strcmp(test1[0], "123, 456, 789"));
    BOOST_CHECK(0 == test1[1]);

  }

  BOOST_AUTO_TEST_CASE(ParameterParser_BlankSpaceSeparator){
    CIEC_STRING emptyTest = " 123, 456, 789   ";
    CParameterParser test8(emptyTest.getValue(), 8, ' ');

    BOOST_CHECK(7 == test8.parseParameters());

    BOOST_CHECK(0 == strcmp(test8[0], ""));
    BOOST_CHECK(0 == strcmp(test8[1], "123,"));
    BOOST_CHECK(0 == strcmp(test8[2], "456,"));
    BOOST_CHECK(0 == strcmp(test8[3], "789"));
    BOOST_CHECK(0 == strcmp(test8[4], ""));
    BOOST_CHECK(0 == strcmp(test8[5], ""));
    BOOST_CHECK(0 == strcmp(test8[6], ""));
    BOOST_CHECK(0 == test8[7]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpace){
    CIEC_STRING emptyTest = "   "; //3 Blank Spaces
    CParameterParser test4(emptyTest.getValue(), 4);

    BOOST_CHECK(1 == test4.parseParameters());

    BOOST_CHECK(0 == strcmp(test4[0], ""));
    BOOST_CHECK(0 == test4[1]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpace){
     CIEC_STRING emptyTest = " , ";
     CParameterParser test4(emptyTest.getValue(), 4);

     BOOST_CHECK(2 == test4.parseParameters());

     BOOST_CHECK(0 == strcmp(test4[0], ""));
     BOOST_CHECK(0 == strcmp(test4[1], ""));
     BOOST_CHECK(0 == test4[2]);
   }

  BOOST_AUTO_TEST_CASE(ParameterParser_EmptyParametersWithBlankSpaceAndEndingInParameter){
     CIEC_STRING emptyTest = " , ,";
     CParameterParser test4(emptyTest.getValue(), 4);

     BOOST_CHECK(3 == test4.parseParameters());

     BOOST_CHECK(0 == strcmp(test4[0], ""));
     BOOST_CHECK(0 == strcmp(test4[1], ""));
     BOOST_CHECK(0 == strcmp(test4[2], ""));
     BOOST_CHECK(0 == test4[3]);
   }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlySeparator){
    CIEC_STRING emptyTest = ",,,";
    CParameterParser test5(emptyTest.getValue(), 5);

    BOOST_CHECK(4 == test5.parseParameters());

    BOOST_CHECK(0 == strcmp(test5[0], ""));
    BOOST_CHECK(0 == strcmp(test5[1], ""));
    BOOST_CHECK(0 == strcmp(test5[2], ""));
    BOOST_CHECK(0 == strcmp(test5[3], ""));
    BOOST_CHECK(0 == test5[4]);
    BOOST_CHECK(0 == test5[5]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_OnlyBlankSpaceSeparator){
    CIEC_STRING emptyTest = "   "; //3 Blank Spaces
    CParameterParser test5(emptyTest.getValue(), 5, ' ');

    BOOST_CHECK(4 == test5.parseParameters());

    BOOST_CHECK(0 == strcmp(test5[0], ""));
    BOOST_CHECK(0 == strcmp(test5[1], ""));
    BOOST_CHECK(0 == strcmp(test5[2], ""));
    BOOST_CHECK(0 == strcmp(test5[3], ""));
    BOOST_CHECK(0 == test5[4]);
    BOOST_CHECK(0 == test5[5]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase1)
  {
    CIEC_STRING sParameterString = "COM1, 19200, 8, 1, NONE, $n";
    CParameterParser parser(sParameterString.getValue(), 6);

    BOOST_CHECK(6 == parser.parseParameters());

    BOOST_CHECK(0 == strcmp(parser[0], "COM1"));
    BOOST_CHECK(0 == strcmp(parser[1], "19200"));
    BOOST_CHECK(0 == strcmp(parser[2], "8"));
    BOOST_CHECK(0 == strcmp(parser[3], "1"));
    BOOST_CHECK(0 == strcmp(parser[4], "NONE"));
    BOOST_CHECK(0 == strcmp(parser[5], "$n"));
    BOOST_CHECK(0 == parser[6]);
  }

  BOOST_AUTO_TEST_CASE(ParameterParser_SerialCase2)
  {
    CIEC_STRING sParameterString = "COM1, 19200, 8, 1, NONE, $r$n";
    CParameterParser parser(sParameterString.getValue(), 6);

    BOOST_CHECK(6 == parser.parseParameters());

    BOOST_CHECK(0 == strcmp(parser[0], "COM1"));
    BOOST_CHECK(0 == strcmp(parser[1], "19200"));
    BOOST_CHECK(0 == strcmp(parser[2], "8"));
    BOOST_CHECK(0 == strcmp(parser[3], "1"));
    BOOST_CHECK(0 == strcmp(parser[4], "NONE"));
    BOOST_CHECK(0 == strcmp(parser[5], "$r$n"));
    BOOST_CHECK(0 == parser[6]);
  }


BOOST_AUTO_TEST_SUITE_END()

