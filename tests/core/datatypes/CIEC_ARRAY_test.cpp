/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, fortiss GmbH, Profactor, nxtControl
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Micheal Hofmann, Stanislav Meduna, Ingo Hegny - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_array.h"
#include "../../../src/core/datatypes/forte_bool.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_string.h"
#include "../../../src/core/datatypes/forte_wstring.h"
#include "../../../src/core/typelib.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CIEC_ARRAY_test_gen.cpp"
#endif

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)
BOOST_AUTO_TEST_CASE(Array_assignment_test_BOOL)
{
  CIEC_ARRAY nTest(3, g_nStringIdBOOL);

  BOOST_CHECK_EQUAL(nTest.size(), 3);
  BOOST_CHECK_EQUAL(nTest.getElementDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[0]->getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[1]->getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[2]->getDataTypeID(), CIEC_ANY::e_BOOL);

  static_cast<CIEC_BOOL &>(*nTest[0]) = true;
  static_cast<CIEC_BOOL &>(*nTest[1]) = false;
  static_cast<CIEC_BOOL &>(*nTest[2]) = true;

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[0]), true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[2]), true);

  static_cast<CIEC_BOOL &>(*nTest[0]) = false;
  static_cast<CIEC_BOOL &>(*nTest[1]) = false;
  static_cast<CIEC_BOOL &>(*nTest[2]) = true;

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[0]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[2]), true);

  static_cast<CIEC_BOOL &>(*nTest[0]) = true;
  static_cast<CIEC_BOOL &>(*nTest[1]) = false;
  static_cast<CIEC_BOOL &>(*nTest[2]) = false;

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[0]), true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nTest[2]), false);
}

BOOST_AUTO_TEST_CASE(Array_assignment_test_INT)
{
  CIEC_ARRAY nTest(5, g_nStringIdINT);

  BOOST_CHECK_EQUAL(nTest.size(), 5);

  BOOST_CHECK_EQUAL(nTest[0]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[1]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[2]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[3]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[4]->getDataTypeID(), CIEC_ANY::e_INT);

  static_cast<CIEC_INT &>(*nTest[0]) = 1;
  static_cast<CIEC_INT &>(*nTest[1]) = -32259;
  static_cast<CIEC_INT &>(*nTest[2]) = 256;
  static_cast<CIEC_INT &>(*nTest[3]) = -32259;
  static_cast<CIEC_INT &>(*nTest[4]) = 256;

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 256);
  BOOST_CHECK((0 == nTest[5]));
}

BOOST_AUTO_TEST_CASE(Array_assignment_test_array)
{
  CIEC_ARRAY nTest(5, g_nStringIdINT);
  char acBuffer[30];

  BOOST_CHECK_EQUAL(nTest.fromString("[1,2,3,4,5]"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[1, 2,3 , 4 ,5]"), 15);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[  1,    2,3    , 4,5  ]"), 24);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2]"), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2,4]"), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2,4,7,8]"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[0]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[1]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[2]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest[4]), 7);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10,20,30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10.0,20,30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10,20,test,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("wrong string"), -1);

}


BOOST_AUTO_TEST_CASE(Array_copy_test){
  CIEC_ARRAY nTest1(5, g_nStringIdINT);
  CIEC_ARRAY nTest2(5, g_nStringIdINT);

  //TODO think on implementing array assignment
//  BOOST_CHECK_EQUAL(nTest1.fromString("[1,2,3,4,5]"), true);
//  nTest2 = nTest1;
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[0]), 1);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[1]), 2);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[2]), 3);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[3]), 4);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[4]), 5);
//
//  BOOST_CHECK_EQUAL(nTest1.fromString("[5,4,2,3,1]"), true);
//  ntest2 = nTest1;
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[0]), 5);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[1]), 4);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[2]), 2);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[3]), 3);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest2[4]), 1);

  BOOST_CHECK_EQUAL(nTest2.fromString("[1,2,3,4,5]"), 11);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[0]), 5);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[1]), 4);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[2]), 2);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[3]), 3);
//  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[4]), 1);

  nTest1.setValue(nTest2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[4]), 5);

  BOOST_CHECK_EQUAL(nTest2.fromString("[5,4,2,3,1]"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[1]), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[2]), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[3]), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*nTest1[4]), 5);
}


BOOST_AUTO_TEST_CASE(Configure_test){
  CIEC_ARRAY *pTest = static_cast<CIEC_ARRAY *>(CTypeLib::createDataTypeInstance(g_nStringIdARRAY, 0));

  pTest->setup(8, g_nStringIdINT);

  BOOST_CHECK_EQUAL(pTest->size(), 8);

  BOOST_CHECK_EQUAL((*pTest)[0]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[1]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[2]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[3]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[4]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[5]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[6]->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[7]->getDataTypeID(), CIEC_ANY::e_INT);

  static_cast<CIEC_INT &>(*(*pTest)[0]) = 1;
  static_cast<CIEC_INT &>(*(*pTest)[1]) = -32259;
  static_cast<CIEC_INT &>(*(*pTest)[2]) = 256;
  static_cast<CIEC_INT &>(*(*pTest)[4]) = -32259;
  static_cast<CIEC_INT &>(*(*pTest)[7]) = 256;

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*(*pTest)[0]), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*(*pTest)[1]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*(*pTest)[2]), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*(*pTest)[4]), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT &>(*(*pTest)[7]), 256);

  pTest->setup(15, g_nStringIdSTRING);
  BOOST_CHECK_EQUAL(pTest->size(), 15);

  BOOST_CHECK_EQUAL((*pTest)[0]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[1]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[2]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[3]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[4]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[5]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[6]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[7]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[8]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[9]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[10]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[11]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[12]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[13]->getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[14]->getDataTypeID(), CIEC_ANY::e_STRING);

  static_cast<CIEC_STRING &>(*(*pTest)[0]) = "Hansi";
  BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>(*(*pTest)[0]).length(), 5);
  BOOST_CHECK_EQUAL(strcmp("Hansi", static_cast<CIEC_STRING &>(*(*pTest)[0]).getValue()), 0);

  delete pTest;
}

BOOST_AUTO_TEST_CASE(Array_fromString_StringArrayTest)
{
  CIEC_ARRAY nTest(3, g_nStringIdSTRING);
  const char cTestString1[] = {"[\'String 1\',\'String 2\',\'String 3\']"};
  const char cTestString2[] = {"[\'String 1\']"};
  const char cTestString2fromStringResult[] = {"[\'String 1\',\'\',\'\']"};
  const char cTestString3[] = {"[\'String 10\',\'String 20\',\'String 30\',\'String 4\',\'String 5\']"};
  const char cTestString3fromStringResult[] = {"[\'String 10\',\'String 20\',\'String 30\']"};
  const char cTestString4[] = {"[\'String 1\',\'String 2\',\'String 3]"};
  const char cTestString5[] = {"[\'String $$1\',\'String $\'2\',\'String $\"3\']"};
  const char cTestString6[] = {"[  \'String 1\',\'String 2\',\'String 3\']"};
  const char cTestString7[] = {"[\'String 1\'  ,\'String 2\',\'String 3\']"};
  const char cTestString8[] = {"[  \'String 1\'  ,\'String 2\',\'String 3\']"};
  const char cTestString9[] = {"[\'String 1\',  \'String 2\',\'String 3\']"};
  const char cTestString10[] = {"[\'String 1\',\'String 2\'  ,\'String 3\']"};
  const char cTestString11[] = {"[\'String 1\',  \'String 2\'  ,\'String 3\']"};
  const char cTestString12[] = {"[  \'String 1\'  ,  \'String 2\'  ,  \'String 3\'   ]"};
  const char cTestString13[] = {"[  \' String 1 \'  ,  \' String 2 \'  ,  \' String 3 \'   ]"};

  char acBuffer[50];

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString1), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString2), strlen(cTestString2));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString2fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString2fromStringResult), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString3), strlen(cTestString3));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 10"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 20"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 30"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString3fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString3fromStringResult), 0);

  CIEC_ARRAY nTest2(3, g_nStringIdSTRING);
  BOOST_CHECK_EQUAL(nTest2.fromString(cTestString4), -1);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest2[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest2[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest2[2]).getValue(), ""), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString5), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String $1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String \'2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String \"3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString5), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString6), strlen(cTestString6));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString7), strlen(cTestString7));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString8), strlen(cTestString8));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString9), strlen(cTestString9));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString10), strlen(cTestString10));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString11), strlen(cTestString11));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString12), strlen(cTestString12));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString13), strlen(cTestString13));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[0]).getValue(), " String 1 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[1]).getValue(), " String 2 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nTest[2]).getValue(), " String 3 "), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1) + 6); //length stays as in string 1 + the empty spaces in strings
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[\' String 1 \',\' String 2 \',\' String 3 \']"), 0);


}

BOOST_AUTO_TEST_CASE(Array_fromString_WStringArrayTest)
{
  CIEC_ARRAY nTest(3, g_nStringIdWSTRING);
  const char cTestString1[] = {"[\"String 1\",\"String 2\",\"String 3\"]"};
  const char cTestString2[] = {"[\"String 1\"]"};
  const char cTestString2fromStringResult[] = {"[\"String 1\",\"\",\"\"]"};
  const char cTestString3[] = {"[\"String 10\",\"String 20\",\"String 30\",\"String 4\",\"String 5\"]"};
  const char cTestString3fromStringResult[] = {"[\"String 10\",\"String 20\",\"String 30\"]"};
  const char cTestString4[] = {"[\"String 1\",\"String 2\",\"String 3]"};
  const char cTestString5[] = {"[\"String $$1\",\"String $\'2\",\"String $\"3\"]"};
  const char cTestString6[] = {"[  \"String 1\",\"String 2\",\"String 3\"]"};
  const char cTestString7[] = {"[\"String 1\"  ,\"String 2\",\"String 3\"]"};
  const char cTestString8[] = {"[  \"String 1\"  ,\"String 2\",\"String 3\"]"};
  const char cTestString9[] = {"[\"String 1\",  \"String 2\",\"String 3\"]"};
  const char cTestString10[] = {"[\"String 1\",\"String 2\"  ,\"String 3\"]"};
  const char cTestString11[] = {"[\"String 1\",  \"String 2\"  ,\"String 3\"]"};
  const char cTestString12[] = {"[  \"String 1\"  ,  \"String 2\"  ,  \"String 3\"   ]"};
  const char cTestString13[] = {"[  \" String 1 \"  ,  \" String 2 \"  ,  \" String 3 \"   ]"};

  char acBuffer[50];

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString1), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString2), strlen(cTestString2));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString2fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString2fromStringResult), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString3), strlen(cTestString3));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 10"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 20"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 30"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString3fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString3fromStringResult), 0);

  CIEC_ARRAY nTest2(3, g_nStringIdWSTRING);
  BOOST_CHECK_EQUAL(nTest2.fromString(cTestString4), -1);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest2[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest2[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest2[2]).getValue(), ""), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString5), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String $1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String \'2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String \"3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString5), 0);

  //*---
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString6), strlen(cTestString6));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString7), strlen(cTestString7));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString8), strlen(cTestString8));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString9), strlen(cTestString9));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString10), strlen(cTestString10));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString11), strlen(cTestString11));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString12), strlen(cTestString12));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString13), strlen(cTestString13));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[0]).getValue(), " String 1 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[1]).getValue(), " String 2 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(*nTest[2]).getValue(), " String 3 "), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1) + 6);  //length stays as in string 1 + the empty spaces in strings
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[\" String 1 \",\" String 2 \",\" String 3 \"]"), 0);
}

BOOST_AUTO_TEST_SUITE_END()
