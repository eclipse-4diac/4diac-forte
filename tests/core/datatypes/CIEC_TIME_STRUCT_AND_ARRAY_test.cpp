/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Fabio Gandolfi - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <string>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_time.h"
#include "../../../src/core/datatypes/forte_bool.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_struct.h"
#include "../../../src/core/datatypes/forte_array.h"
#include "../../../src/core/typelib.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CIEC_TIME_STRUCT_AND_ARRAY_test_gen.cpp"
#endif

class CIEC_TestStruct : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(TestStruct)
    ;
    /* Struct:
     *   val1 : BOOL
     *   val2 : INT
     *   val3 : TIME
     */

  public:
    CIEC_BOOL Var1;
    CIEC_INT Var2;
    CIEC_TIME Var3;

    CIEC_TestStruct() = default;

    size_t getStructSize() const override {
      return 3;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override {
      return g_nStringIdTestStruct;
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
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

const CStringDictionary::TStringId CIEC_TestStruct::scmElementTypes[] = { g_nStringIdBOOL, g_nStringIdINT, g_nStringIdTIME };
const CStringDictionary::TStringId CIEC_TestStruct::scmElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };

DEFINE_FIRMWARE_DATATYPE(TestStruct, g_nStringIdTestStruct)

BOOST_AUTO_TEST_SUITE (CIEC_TIME_STRUCT_AND_ARRAY_function_test) 

//STRUCT TESTS
void setDataTestStruct(CIEC_TestStruct &paStruct, bool paVal1, int paVal2, CIEC_TIME::TValueType paVal3){
  (* static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal1))) = CIEC_BOOL(paVal1);
  (* static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal2))) = CIEC_INT(static_cast<TForteInt16>(paVal2));
  (* static_cast<CIEC_TIME *>(paStruct.getMemberNamed(g_nStringIdVal3))) = CIEC_TIME(paVal3);
}

void setupTestStruct_TestDataSet1(CIEC_TestStruct &paStruct){
  setDataTestStruct(paStruct, true, 44, static_cast<CIEC_TIME::TValueType>(10));
}

void checkTestStruct_InitialValues(CIEC_TestStruct &paStruct){
  BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(paStruct.getMemberNamed(g_nStringIdVal3))));
}

void checkTestStruct_TestDataSet1(CIEC_TestStruct &paStruct){
  BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(44, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(10, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(paStruct.getMemberNamed(g_nStringIdVal3))));
}

BOOST_AUTO_TEST_CASE(TimeStruct_element_access_test){ 
  CIEC_TestStruct stStruct;
  BOOST_CHECK_EQUAL(stStruct.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(stStruct.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(stStruct.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_TIME);

  BOOST_CHECK_EQUAL(stStruct.Var1.getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(stStruct.Var2.getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(stStruct.Var3.getDataTypeID(), CIEC_ANY::e_TIME);

  checkTestStruct_InitialValues(stStruct);

  (*static_cast<CIEC_BOOL *>(stStruct.getMemberNamed(g_nStringIdVal1))) = CIEC_BOOL(true);
  (*static_cast<CIEC_INT *>(stStruct.getMemberNamed(g_nStringIdVal2))) = CIEC_INT(55);
  (*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))) = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(12));
  BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(55, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(stStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(12, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))));
}

BOOST_AUTO_TEST_CASE(TimeStruct_TIME_parser_test) {
  CIEC_TestStruct stStruct;
  checkTestStruct_InitialValues(stStruct);
  CIEC_TIME timeVar = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  timeVar.fromString("T#33ms");
  (*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))) = timeVar;
  BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(stStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(33000000, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))));

  timeVar.fromString("TIME#44h36m");
  (*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))) = timeVar;
  BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(stStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(160560000000000, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(stStruct.getMemberNamed(g_nStringIdVal3))));

  const char cTestString_1[] = { "(Val1:=TRUE,Val2:=11,Val3:=T#33ms)" };
  const char cTestString_2[] = { "(Val1:=TRUE,Val2:=11,Val3:=TIME#44h36m)" };
  const char cTestString_3[] = { "(Val1:=TRUE,Val2:=11,Val3:=T#)" };
  const char cTestString_4[] = { "(Val1:=TRUE,Val2:=11,Val3:=T#33" };
  const char cTestString_5[] = { "(Val1:=TRUE,Val2:=11,Val3:=33" };

  BOOST_CHECK_EQUAL(stStruct.fromString(cTestString_1), strlen(cTestString_1));
  BOOST_CHECK_EQUAL(stStruct.fromString(cTestString_2), strlen(cTestString_2));
  BOOST_CHECK_EQUAL(stStruct.fromString(cTestString_3), -1);
  BOOST_CHECK_EQUAL(stStruct.fromString(cTestString_4), -1);
  BOOST_CHECK_EQUAL(stStruct.fromString(cTestString_5), -1);
}

// ARRAY TESTS
void setupArrayOfStructTest_TestDataSet1(CIEC_TestStruct &paStruct) {
  setDataTestStruct(paStruct, true, 33, static_cast<CIEC_TIME::TValueType>(22));
}

void checkArrayOfStructTest_TestDataSet1(CIEC_TestStruct &paStruct) {
  BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal1))));
  BOOST_CHECK_EQUAL(33, static_cast<CIEC_INT::TValueType>(*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal2))));
  BOOST_CHECK_EQUAL(22, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(paStruct.getMemberNamed(g_nStringIdVal3))));
}

BOOST_AUTO_TEST_CASE(arrayOfStructs_access_test) {
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdTestStruct);

  BOOST_CHECK_EQUAL(nTest.size(), 3);
  BOOST_CHECK_EQUAL(nTest.getElementDataTypeID(), CIEC_ANY::e_STRUCT);

  BOOST_CHECK_EQUAL(nTest[0].getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(nTest[1].getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(nTest[2].getDataTypeID(), CIEC_ANY::e_STRUCT);

  BOOST_CHECK_EQUAL(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_TIME);

  char acBuffer[230];
  for(size_t i = 0; i < 3; i++) {
    CIEC_TestStruct &toTest = static_cast<CIEC_TestStruct&>(nTest[i]);
    setupArrayOfStructTest_TestDataSet1(toTest);
    BOOST_CHECK_EQUAL(toTest.toString(acBuffer, 230), sizeof("(Val1:=TRUE,Val2:=33,Val3:=T#22ns)") - 1);
    BOOST_CHECK_EQUAL(strcmp(acBuffer, "(Val1:=TRUE,Val2:=33,Val3:=T#22ns)"), 0);
  }

  BOOST_CHECK_EQUAL(nTest.toString(acBuffer, 230), sizeof("[(Val1:=TRUE,Val2:=33,Val3:=T#22ns),(Val1:=TRUE,Val2:=33,Val3:=T#22ns),(Val1:=TRUE,Val2:=33,Val3:=T#22ns)]") - 1);
  BOOST_CHECK_EQUAL(strcmp(acBuffer, "[(Val1:=TRUE,Val2:=33,Val3:=T#22ns),(Val1:=TRUE,Val2:=33,Val3:=T#22ns),(Val1:=TRUE,Val2:=33,Val3:=T#22ns)]"), 0);
}

BOOST_AUTO_TEST_CASE(arrayOfStructs_TIME_parser_test) {
  CIEC_ARRAY_DYNAMIC nTest(3, g_nStringIdTestStruct);

  CIEC_TIME timeVar = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  timeVar.fromString("T#33ms");
  for(size_t i = 0; i < 3; i++) { 
    (*static_cast<CIEC_TIME *>(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal3))) = timeVar;
    BOOST_CHECK_EQUAL(33000000, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal3))));
  }

  timeVar.fromString("TIME#44h36m");
  for(size_t i = 0; i < 3; i++) { 
    (*static_cast<CIEC_TIME *>(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal3))) = timeVar;
    BOOST_CHECK_EQUAL(160560000000000, static_cast<CIEC_TIME::TValueType>(*static_cast<CIEC_TIME *>(static_cast<CIEC_STRUCT &>(nTest[0]).getMemberNamed(g_nStringIdVal3))));
  }

  const char cTestString_1[] = { "[(Val1:=TRUE,Val2:=33,Val3:=T#44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44ms)]" };
  const char cTestString_2[] = { "[(Val1:=TRUE,Val2:=33,Val3:=TIME#44h36m),(Val1:=TRUE,Val2:=33,Val3:=TIME#44h36m),(Val1:=TRUE,Val2:=33,Val3:=TIME#44h36m)]" };
  const char cTestString_3[] = { "[(Val1:=TRUE,Val2:=33,Val3:=TIME#44h36m),(Val1:=TRUE,Val2:=33,Val3:=TIME#),(Val1:=TRUE,Val2:=33,Val3:=TIME#44h36m)]" };
  const char cTestString_4[] = { "[(Val1:=TRUE,Val2:=33,Val3:=44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44ms)]" };
  const char cTestString_5[] = { "[(Val1:=TRUE,Val2:=33,Val3:=44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44ms),(Val1:=TRUE,Val2:=33,Val3:=T#44)]" };

  BOOST_CHECK_EQUAL(nTest.fromString(cTestString_1), strlen(cTestString_1));
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString_2), strlen(cTestString_2));
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString_3), -1);
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString_4), -1);
  BOOST_CHECK_EQUAL(nTest.fromString(cTestString_5), -1);
}

BOOST_AUTO_TEST_SUITE_END()
