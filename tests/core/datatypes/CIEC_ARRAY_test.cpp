/*******************************************************************************
 * Copyright (c) 2011 - 2015 ACIN, fortiss GmbH, Profactor, nxtControl
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Micheal Hofmann, Stanislav Meduna, Ingo Hegny - initial API and implementation and/or initial documentation
 *   Martin Jobst - add tests for repeat syntax
 *                - add equals tests
 *                - add tests for collapsing identical consecutive elements
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_array.h"
#include "../../../src/core/datatypes/forte_bool.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_string.h"
#include "../../../src/core/datatypes/forte_wstring.h"
#include "../../../src/core/typelib.h"
#include "../../../src/core/datatypes/forte_struct.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CIEC_ARRAY_test_gen.cpp"
#endif

using namespace std::string_literals;

class CIEC_ArrayOfStructTest : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ArrayOfStructTest)
    ;

    /* Struct:
     *   val1 : String[2]
     *   val2 : BOOL
     *   val3 : INT[1]
     */

  public:
    CIEC_ARRAY_FIXED<CIEC_STRING, 0, 1> Var1;
    CIEC_BOOL Var2;
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 0> Var3;

    CIEC_ArrayOfStructTest() :
            Var1(), Var2(false), Var3() {};

    size_t getStructSize() const override {
      return 3;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override {
      return g_nStringIdArrayOfStructTest;
    }

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }

  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

const CStringDictionary::TStringId CIEC_ArrayOfStructTest::scmElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };

DEFINE_FIRMWARE_DATATYPE(ArrayOfStructTest, g_nStringIdArrayOfStructTest);

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)
BOOST_AUTO_TEST_CASE(Array_assignment_test_BOOL)
{
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdBOOL);

  BOOST_CHECK_EQUAL(nTest.size(), 3);
  BOOST_CHECK_EQUAL(nTest.getElementDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[0].getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[1].getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(nTest[2].getDataTypeID(), CIEC_ANY::e_BOOL);

  static_cast<CIEC_BOOL &>(nTest[0]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nTest[1]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nTest[2]) = CIEC_BOOL(true);

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[0]), true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[2]), true);

  static_cast<CIEC_BOOL &>(nTest[0]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nTest[1]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nTest[2]) = CIEC_BOOL(true);

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[0]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[2]), true);

  static_cast<CIEC_BOOL &>(nTest[0]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nTest[1]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nTest[2]) = CIEC_BOOL(false);

  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[0]), true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[1]), false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nTest[2]), false);
  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[false,false,false]")); //use max length of BOOL
}

BOOST_AUTO_TEST_CASE(Array_assignment_test_INT)
{
  CIEC_ARRAY_DYNAMIC nTest(5, g_nStringIdINT);

  BOOST_CHECK_EQUAL(nTest.size(), 5);

  BOOST_CHECK_EQUAL(nTest[0].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[1].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[2].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[3].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[4].getDataTypeID(), CIEC_ANY::e_INT);

  static_cast<CIEC_INT &>(nTest[0]) = CIEC_INT(1);
  static_cast<CIEC_INT &>(nTest[1]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(nTest[2]) = CIEC_INT(256);
  static_cast<CIEC_INT &>(nTest[3]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(nTest[4]) = CIEC_INT(256);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 256);
  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[+32767,+32767,+32767,+32767,+32767]")); //use max length of INT
}

BOOST_AUTO_TEST_CASE(Array_assignment_test_INT_with_IEC_type_indices)
{
  CIEC_ARRAY_DYNAMIC nTest(5, g_nStringIdINT);

  BOOST_CHECK_EQUAL(nTest.size(), 5);

  BOOST_CHECK_EQUAL(nTest[CIEC_USINT(0)].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[CIEC_USINT(1)].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[CIEC_USINT(2)].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[CIEC_USINT(3)].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(nTest[CIEC_USINT(4)].getDataTypeID(), CIEC_ANY::e_INT);

  static_cast<CIEC_INT &>(nTest[CIEC_USINT(0)]) = CIEC_INT(1);
  static_cast<CIEC_INT &>(nTest[CIEC_USINT(1)]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(nTest[CIEC_USINT(2)]) = CIEC_INT(256);
  static_cast<CIEC_INT &>(nTest[CIEC_USINT(3)]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>(nTest[CIEC_USINT(4)]) = CIEC_INT(256);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[CIEC_USINT(0)])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[CIEC_USINT(1)])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[CIEC_USINT(2)])), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[CIEC_USINT(3)])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[CIEC_USINT(4)])), 256);
  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[+32767,+32767,+32767,+32767,+32767]")); // use max length of INT
}

BOOST_AUTO_TEST_CASE(Array_assignment_test_array)
{
  CIEC_ARRAY_DYNAMIC nTest(5, g_nStringIdINT);
  char acBuffer[30];

  BOOST_CHECK_EQUAL(nTest.fromString("[1,2,3,4,5]"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[1, 2,3 , 4 ,5]"), 15);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[  1,    2,3    , 4,5  ]"), 24);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 5);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 30), 11);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[1,2,3,4,5]"), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2]"), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2,4]"), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 0);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2,4,7,8]"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 7);

  BOOST_CHECK_EQUAL(nTest.fromString("[1,2(2,3)]"), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 3);

  BOOST_CHECK_EQUAL(nTest.fromString("[2 ( 1, 2, 3 )]"), 15);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 2);

  BOOST_CHECK_EQUAL(nTest.fromString("[1, 3 ( 2, 3 )]"), 15);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[3])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest[4])), 3);

  BOOST_CHECK_EQUAL(nTest.fromString("[3,1,2"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("[10,20(30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("[10,20(,30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10,20,30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10.0,20,30,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("10,20,test,40,50"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("wrong string"), -1);

  CIEC_ARRAY_DYNAMIC nLargeTest(256, g_nStringIdINT);
  BOOST_CHECK_EQUAL(nLargeTest.toString(acBuffer, 30), 8);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[256(0)]"), 0);

  nLargeTest[17].setValue(CIEC_INT(4));
  BOOST_CHECK_EQUAL(nLargeTest.toString(acBuffer, 30), 16);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[17(0),4,238(0)]"), 0);
}


BOOST_AUTO_TEST_CASE(Array_copy_test){
  CIEC_ARRAY_DYNAMIC nTest1(5, g_nStringIdINT);
  CIEC_ARRAY_DYNAMIC nTest2(5, g_nStringIdINT);

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
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[4])), 5);

  BOOST_CHECK_EQUAL(nTest2.fromString("[5,4,2,3,1]"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[4])), 5);

  CIEC_INT intTest(5);
  nTest1.setValue(intTest); //try to assign non-array. Shouldn't change or break anything
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[1])), 2);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[2])), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[3])), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[4])), 5);
}

BOOST_AUTO_TEST_CASE(Array_equality_test){
  CIEC_ARRAY_DYNAMIC nTest1(5, g_nStringIdINT);
  CIEC_ARRAY_DYNAMIC nTest2(5, g_nStringIdINT);

  BOOST_CHECK(nTest1.equals(nTest2));
  BOOST_CHECK(!nTest1.equals(CIEC_INT(0)));

  BOOST_CHECK_EQUAL(nTest2.fromString("[1,2,3,4,5]"), 11);
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest1.setValue(nTest2);
  BOOST_CHECK(nTest1.equals(nTest2));

  BOOST_CHECK_EQUAL(nTest1.fromString("[5,4,2,3,1]"), 11);
  BOOST_CHECK(!nTest1.equals(nTest2));
  BOOST_CHECK_EQUAL(nTest2.fromString("[5,4,2,3,1]"), 11);
  BOOST_CHECK(nTest1.equals(nTest2));
}

BOOST_AUTO_TEST_CASE(Configure_test){
  CIEC_ARRAY_DYNAMIC *pTest = static_cast<CIEC_ARRAY_DYNAMIC *>(CTypeLib::createDataTypeInstance(g_nStringIdARRAY, nullptr));

  pTest->setup(8, g_nStringIdINT);

  BOOST_CHECK_EQUAL(pTest->size(), 8);

  BOOST_CHECK_EQUAL((*pTest)[0].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[1].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[2].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[3].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[4].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[5].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[6].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL((*pTest)[7].getDataTypeID(), CIEC_ANY::e_INT);

  static_cast<CIEC_INT &>((*pTest)[0]) = CIEC_INT(1);
  static_cast<CIEC_INT &>((*pTest)[1]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>((*pTest)[2]) = CIEC_INT(256);
  static_cast<CIEC_INT &>((*pTest)[4]) = CIEC_INT(-32259);
  static_cast<CIEC_INT &>((*pTest)[7]) = CIEC_INT(256);

  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>((*pTest)[0])), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>((*pTest)[1])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>((*pTest)[2])), 256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>((*pTest)[4])), -32259);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>((*pTest)[7])), 256);

  pTest->setup(15, g_nStringIdSTRING);
  BOOST_CHECK_EQUAL(pTest->size(), 15);

  BOOST_CHECK_EQUAL((*pTest)[0].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[1].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[2].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[3].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[4].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[5].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[6].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[7].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[8].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[9].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[10].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[11].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[12].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[13].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL((*pTest)[14].getDataTypeID(), CIEC_ANY::e_STRING);

  static_cast<CIEC_STRING &>((*pTest)[0]) = "Hansi"_STRING;
  BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>((*pTest)[0]).length(), 5);
  BOOST_TEST("Hansi"s == static_cast<CIEC_STRING &>((*pTest)[0]).getStorage());

  delete pTest;
}

BOOST_AUTO_TEST_CASE(Array_fromString_StringArrayTest)
{
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdSTRING);
  const char cTestString1[] = {"[\'String 1\',\'String 2\',\'String 3\']"};
  const char cTestString2[] = {"[\'String 1\']"};
  const char cTestString2fromStringResult[] = {"[\'String 1\',\'\',\'\']"};
  const char cTestString3[] = {"[\'String 10\',\'String 20\',\'String 30\',\'String 4\',\'String 5\']"};
  const char cTestString3fromStringResult[] = {"[\'String 10\',\'String 20\',\'String 30\']"};
  const char cTestString4[] = {"[\'String 1\',\'String 2\',\'String 3]"};
  const char cTestString5[] = {"[\'String $$1\',\'String $\'2\',\'String \"3\']"};
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
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString2), strlen(cTestString2));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == ""s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == ""s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString2fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString2fromStringResult), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString3), strlen(cTestString3));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 10"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 20"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 30"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString3fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString3fromStringResult), 0);

  CIEC_ARRAY_DYNAMIC nTest2(3, g_nStringIdSTRING);
  BOOST_CHECK_EQUAL(nTest2.fromString(cTestString4), -1);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest2[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest2[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest2[2]).getStorage() == ""s);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString5), strlen(cTestString5));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String $1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String \'2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String \"3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString5));
  BOOST_TEST(acBuffer == cTestString5);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString6), strlen(cTestString6));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString7), strlen(cTestString7));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString8), strlen(cTestString8));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString9), strlen(cTestString9));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString10), strlen(cTestString10));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString11), strlen(cTestString11));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString12), strlen(cTestString12));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == "String 1"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == "String 2"s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == "String 3"s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString13), strlen(cTestString13));
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[0]).getStorage() == " String 1 "s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[1]).getStorage() == " String 2 "s);
  BOOST_TEST(static_cast<CIEC_STRING &>(nTest[2]).getStorage() == " String 3 "s);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1) + 6); //length stays as in string 1 + the empty spaces in strings
  BOOST_TEST(acBuffer == "[\' String 1 \',\' String 2 \',\' String 3 \']");
  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[\' String 1 \',\' String 2 \',\' String 3 \']"));

}

BOOST_AUTO_TEST_CASE(Array_fromString_WStringArrayTest)
{
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdWSTRING);
  const char cTestString1[] = {"[\"String 1\",\"String 2\",\"String 3\"]"};
  const char cTestString2[] = {"[\"String 1\"]"};
  const char cTestString2fromStringResult[] = {"[\"String 1\",\"\",\"\"]"};
  const char cTestString3[] = {"[\"String 10\",\"String 20\",\"String 30\",\"String 4\",\"String 5\"]"};
  const char cTestString3fromStringResult[] = {"[\"String 10\",\"String 20\",\"String 30\"]"};
  const char cTestString4[] = {"[\"String 1\",\"String 2\",\"String 3]"};
  const char cTestString5[] = {"[\"String $$1\",\"String \'2\",\"String $\"3\"]"};
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
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString2), strlen(cTestString2));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), ""), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString2fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString2fromStringResult), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString3), strlen(cTestString3));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 10"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 20"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 30"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString3fromStringResult));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString3fromStringResult), 0);

  CIEC_ARRAY_DYNAMIC nTest2(3, g_nStringIdWSTRING);
  BOOST_CHECK_EQUAL(nTest2.fromString(cTestString4), -1);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest2[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest2[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest2[2]).getValue(), ""), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString5), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String $1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String \'2"), 0);
  BOOST_TEST(static_cast<CIEC_WSTRING &>(nTest[2]).getValue() == "String \"3");
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString5));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString5), 0);

  //*---
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString6), strlen(cTestString6));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString7), strlen(cTestString7));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString8), strlen(cTestString8));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString9), strlen(cTestString9));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString10), strlen(cTestString10));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString11), strlen(cTestString11));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1)); //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString12), strlen(cTestString12));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), "String 1"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), "String 2"), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), "String 3"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1));  //length stays as in string 1
  BOOST_CHECK_EQUAL(strcmp(acBuffer, cTestString1), 0);

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString13), strlen(cTestString13));
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[0]).getValue(), " String 1 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[1]).getValue(), " String 2 "), 0);
  BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_WSTRING &>(nTest[2]).getValue(), " String 3 "), 0);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 2), -1);
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 50), strlen(cTestString1) + 6);  //length stays as in string 1 + the empty spaces in strings
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[\" String 1 \",\" String 2 \",\" String 3 \"]"), 0);
  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[\" String 1 \",\" String 2 \",\" String 3 \"]"));
}

  void checkEmptyArray(CIEC_ARRAY_DYNAMIC& paEmptyArray) {
    char acBuffer[30];

    BOOST_CHECK_EQUAL(paEmptyArray.size(), 0);
    BOOST_CHECK_EQUAL(paEmptyArray.getElementDataTypeID(), CIEC_ANY::e_ANY);
    BOOST_CHECK_EQUAL(paEmptyArray.getToStringBufferSize(), sizeof("[]"));
    BOOST_CHECK_EQUAL(paEmptyArray.toString(acBuffer, 30), 2);

    CIEC_ARRAY_DYNAMIC nTest1(1, g_nStringIdINT);
    BOOST_CHECK_EQUAL(nTest1.fromString("[2]"), 3);
    paEmptyArray.setValue(nTest1); //shouldn't change or break anything

    BOOST_CHECK_EQUAL(paEmptyArray.size(), 0);
    BOOST_CHECK_EQUAL(paEmptyArray.getElementDataTypeID(), CIEC_ANY::e_ANY);
    BOOST_CHECK_EQUAL(paEmptyArray.getToStringBufferSize(), sizeof("[]"));
    BOOST_CHECK_EQUAL(paEmptyArray.toString(acBuffer, 30), 2);

    nTest1.setValue(paEmptyArray); //shouldn't change or break anything

    BOOST_CHECK_EQUAL(nTest1.size(), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(nTest1[0])), 2);
    BOOST_CHECK_EQUAL(nTest1.getElementDataTypeID(), CIEC_ANY::e_INT);
  }

BOOST_AUTO_TEST_CASE(Array_emptyArray)
{
  CIEC_ARRAY_DYNAMIC nTest(0, g_nStringIdINT);
    checkEmptyArray(nTest);
}

const std::string cTestStringData = "Check string!"s;
const std::string cTestStringData2 = "Check string 2!"s;

void checkArrayOfStructTest_InitialValues(CIEC_ArrayOfStructTest &paStruct) {
  BOOST_CHECK_EQUAL(0, paStruct.Var1[0].length());
  BOOST_CHECK_EQUAL(0, paStruct.Var1[1].length());
  BOOST_CHECK_EQUAL(false, paStruct.Var2);
  BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(paStruct.Var3[0]));
}

void setDataArrayOfStructTest(CIEC_ArrayOfStructTest &paStruct, const std::string paVal11, const std::string paVal12, bool paVal2, int paVal31) {
  paStruct.Var1[0] = CIEC_STRING(std::move(paVal11));
  paStruct.Var1[1] = CIEC_STRING(std::move(paVal12));
  paStruct.Var2 = CIEC_BOOL(paVal2);
  paStruct.Var3[0] = CIEC_INT(static_cast<TForteInt16>(paVal31));
}

void setupArrayOfStructTest_TestDataSet1(CIEC_ArrayOfStructTest &paStruct) {
  setDataArrayOfStructTest(paStruct, cTestStringData, cTestStringData2, true, 24534);
}

void checkArrayOfStructTest_TestDataSet1(CIEC_ArrayOfStructTest &paStruct) {
  BOOST_TEST(paStruct.Var1[0].getStorage() == cTestStringData);
  BOOST_TEST(paStruct.Var1[1].getStorage() == cTestStringData2);
  BOOST_CHECK_EQUAL(1, paStruct.Var2);
  BOOST_CHECK_EQUAL(24534, static_cast<CIEC_INT::TValueType>(paStruct.Var3[0]));
}

BOOST_AUTO_TEST_CASE(Array_arrayOfStructs)
{
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdArrayOfStructTest);

  char acBuffer[230];

  BOOST_CHECK_EQUAL(nTest.size(), 3);
  BOOST_CHECK_EQUAL(nTest.getElementDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(nTest[0].getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(nTest[1].getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(nTest[2].getDataTypeID(), CIEC_ANY::e_STRUCT);

  for(size_t i = 0; i < 3; i++) {
    CIEC_ArrayOfStructTest &toTest = static_cast<CIEC_ArrayOfStructTest&>(nTest[i]);
    checkArrayOfStructTest_InitialValues(toTest);
    BOOST_CHECK_EQUAL(toTest.getToStringBufferSize(), sizeof("(Val1:=['',''],Val2:=FALSE,Val3:=[+32767])"));
    BOOST_CHECK_EQUAL(toTest.toString(acBuffer, 230), sizeof("(Val1:=['',''],Val2:=FALSE,Val3:=[0])") - 1);
    BOOST_CHECK_EQUAL(strcmp(acBuffer, "(Val1:=['',''],Val2:=FALSE,Val3:=[0])"), 0);
  }

  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[(Val1:=['',''],Val2:=FALSE,Val3:=[+32767]),(Val1:=['',''],Val2:=FALSE,Val3:=[+32767]),(Val1:=['',''],Val2:=FALSE,Val3:=[+32767])]"));
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 230), sizeof("[(Val1:=['',''],Val2:=FALSE,Val3:=[0]),(Val1:=['',''],Val2:=FALSE,Val3:=[0]),(Val1:=['',''],Val2:=FALSE,Val3:=[0])]") - 1);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[(Val1:=['',''],Val2:=FALSE,Val3:=[0]),(Val1:=['',''],Val2:=FALSE,Val3:=[0]),(Val1:=['',''],Val2:=FALSE,Val3:=[0])]"), 0);

  for(size_t i = 0; i < 3; i++) {
    CIEC_ArrayOfStructTest &toTest = static_cast<CIEC_ArrayOfStructTest&>(nTest[i]);
    setupArrayOfStructTest_TestDataSet1(toTest);
    checkArrayOfStructTest_TestDataSet1(toTest);
    BOOST_CHECK_EQUAL(toTest.getToStringBufferSize(), sizeof("(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767])"));
    BOOST_CHECK_EQUAL(toTest.toString(acBuffer, 230), sizeof("(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])") - 1);
    BOOST_CHECK_EQUAL(strcmp(acBuffer, "(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])"), 0);
  }

  BOOST_CHECK_EQUAL(nTest.getToStringBufferSize(), sizeof("[(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767]),(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767]),(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767])]"));
  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 230), sizeof("[(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])]") - 1);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])]"), 0);

  CIEC_ARRAY_DYNAMIC nTest1(3, g_nStringIdArrayOfStructTest);
  (void) nTest1.fromString("[(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])]");

  for(size_t i = 0; i < 3; i++) {
    CIEC_ArrayOfStructTest &toTest = static_cast<CIEC_ArrayOfStructTest&>(nTest1[i]);
    setupArrayOfStructTest_TestDataSet1(toTest);
    checkArrayOfStructTest_TestDataSet1(toTest);
    BOOST_CHECK_EQUAL(toTest.getToStringBufferSize(), sizeof("(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767])"));
    BOOST_CHECK_EQUAL(toTest.toString(acBuffer, 230), sizeof("(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])") - 1);
    BOOST_CHECK_EQUAL(strcmp(acBuffer, "(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])"), 0);
  }

  BOOST_CHECK_EQUAL(nTest1.getToStringBufferSize(), sizeof("[(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767]),(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767]),(Val1:=['Check string!','Check string 2!'],Val2:=FALSE,Val3:=[+32767])]"));
  BOOST_CHECK_EQUAL(nTest1.toString(acBuffer, 230), sizeof("[(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])]") - 1);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534]),(Val1:=['Check string!','Check string 2!'],Val2:=TRUE,Val3:=[24534])]"), 0);

}

  BOOST_AUTO_TEST_CASE(Array_arrayOfUndefined) {
    CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdUNDEFINEDDATATYPE);
    checkEmptyArray(nTest);
  }


BOOST_AUTO_TEST_SUITE_END()
