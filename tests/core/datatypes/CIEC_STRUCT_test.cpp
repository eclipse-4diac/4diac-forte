/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_struct.h"
#include "../../../src/core/datatypes/forte_bool.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_real.h"
#include "../../../src/core/datatypes/forte_string.h"

#include "../../../src/core/typelib.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CIEC_STRUCT_test_gen.cpp"
#endif

BOOST_AUTO_TEST_SUITE(CIEC_STRUCT_function_test)

/***********************************************************/
  class CIEC_TestStruct1 : public CIEC_STRUCT{
    DECLARE_FIRMWARE_DATATYPE(TestStruct1)
      ;
    public:
      CIEC_TestStruct1() :

          CIEC_STRUCT(g_nStringIdTestStruct1, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1){
      }

      virtual ~CIEC_TestStruct1(){
      }

    private:
      static const CStringDictionary::TStringId scm_unElementTypes[];
      static const CStringDictionary::TStringId scm_unElementNames[];
  };

  const CStringDictionary::TStringId CIEC_TestStruct1::scm_unElementTypes[] = { g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdINT };
  const CStringDictionary::TStringId CIEC_TestStruct1::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };

  /***********************************************************/
  class CIEC_TestStruct2 : public CIEC_STRUCT{
    DECLARE_FIRMWARE_DATATYPE(TestStruct2)
      ;
    public:
      CIEC_TestStruct2() :
          CIEC_STRUCT(g_nStringIdTestStruct2, 4, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1){
      }

      virtual ~CIEC_TestStruct2(){
      }

    private:
      static const CStringDictionary::TStringId scm_unElementTypes[];
      static const CStringDictionary::TStringId scm_unElementNames[];
  };

  const CStringDictionary::TStringId CIEC_TestStruct2::scm_unElementTypes[] = { g_nStringIdREAL, g_nStringIdINT, g_nStringIdBOOL, g_nStringIdINT };
  const CStringDictionary::TStringId CIEC_TestStruct2::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3, g_nStringIdVal4 };

//*********************** TESTS *************************************************************************************************/
  void setDataTestStruct1(CIEC_TestStruct1 &pa_roStruct, const char* pa_acVal1, bool pa_nVal2, int pa_nVal3){
    (*(CIEC_STRING *) pa_roStruct.getMemberNamed(g_nStringIdVal1)) = pa_acVal1;
    (*(CIEC_BOOL *) pa_roStruct.getMemberNamed(g_nStringIdVal2)) = pa_nVal2;
    (*(CIEC_INT *) pa_roStruct.getMemberNamed(g_nStringIdVal3)) = static_cast<TForteInt16>(pa_nVal3);
  }

  const char cTestStringData[] = "Check string!";

  void setupTestStruct1_TestDataSet1(CIEC_TestStruct1 &pa_stStruct){
    setDataTestStruct1(pa_stStruct, cTestStringData, true, 24534);
  }

  void checkTestStruct1_InitialValues(CIEC_TestStruct1 &pa_stStruct){
    BOOST_CHECK_EQUAL((*(CIEC_STRING *)pa_stStruct.getMemberNamed(g_nStringIdVal1)).length(), 0);
    BOOST_CHECK_EQUAL(false, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
  }

  void checkTestStruct1_TestDataSet1(CIEC_TestStruct1 &pa_stStruct){
    BOOST_CHECK_EQUAL(strcmp((*(CIEC_STRING *)pa_stStruct.getMemberNamed(g_nStringIdVal1)).getValue(), cTestStringData), 0);
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(24534, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
  }

  void setDataTestStruct2(CIEC_TestStruct2&pa_roStruct, float pa_fVal1, int pa_nVal2, bool pa_bVal3, int pa_bVal4){
      (*(CIEC_REAL *) pa_roStruct.getMemberNamed(g_nStringIdVal1)) = pa_fVal1;
      (*(CIEC_INT *) pa_roStruct.getMemberNamed(g_nStringIdVal2)) = static_cast<TForteInt16>(pa_nVal2);
      (*(CIEC_BOOL *) pa_roStruct.getMemberNamed(g_nStringIdVal3)) = pa_bVal3;
      (*(CIEC_INT *) pa_roStruct.getMemberNamed(g_nStringIdVal4)) = static_cast<TForteInt16>(pa_bVal4);
    }

  void setupTestStruct2_TestDataSet1(CIEC_TestStruct2 &pa_stStruct){
    setDataTestStruct2(pa_stStruct, 3.1415f, 256, true, 31234);
  }

  void checkTestStruct2_InitialValues(CIEC_TestStruct2 &pa_stStruct){
    BOOST_CHECK_EQUAL(0.0f, (*(CIEC_REAL *)pa_stStruct.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(false, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal4)));
  }

  void checkTestStruct2_TestDataSet1(CIEC_TestStruct2 &pa_stStruct){
    BOOST_CHECK_EQUAL(3.1415f, (*(CIEC_REAL *)pa_stStruct.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(256, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(31234, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal4)));
  }

  BOOST_AUTO_TEST_CASE(Struct_element_access_test){
    CIEC_TestStruct1 stStruct1;
    CIEC_TestStruct2 stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_STRING);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_BOOL);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_INT);

    checkTestStruct1_InitialValues(stStruct1);

    char cTest[] = "Check string!";
    (*(CIEC_STRING *) stStruct1.getMemberNamed(g_nStringIdVal1)) = cTest;
    BOOST_CHECK_EQUAL(strcmp((*(CIEC_STRING *)stStruct1.getMemberNamed(g_nStringIdVal1)).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(false, (*(CIEC_BOOL *)stStruct1.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct1.getMemberNamed(g_nStringIdVal3)));

    (*(CIEC_BOOL *) stStruct1.getMemberNamed(g_nStringIdVal2)) = true;
    BOOST_CHECK_EQUAL(strcmp((*(CIEC_STRING *)stStruct1.getMemberNamed(g_nStringIdVal1)).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)stStruct1.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct1.getMemberNamed(g_nStringIdVal3)));

    (*(CIEC_INT *) stStruct1.getMemberNamed(g_nStringIdVal3)) = 24534;
    BOOST_CHECK_EQUAL(strcmp((*(CIEC_STRING *)stStruct1.getMemberNamed(g_nStringIdVal1)).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)stStruct1.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(24534, (*(CIEC_INT *)stStruct1.getMemberNamed(g_nStringIdVal3)));

    //***************** Test struct 2 ********************************************************
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_REAL);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_INT);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_BOOL);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal4)->getDataTypeID(), CIEC_ANY::e_INT);

    checkTestStruct2_InitialValues(stStruct2);

    (*(CIEC_REAL *) stStruct2.getMemberNamed(g_nStringIdVal1)) = 3.1415f;
    BOOST_CHECK_EQUAL(3.1415f, (*(CIEC_REAL *)stStruct2.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(false, (*(CIEC_BOOL *)stStruct2.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal4)));

    (*(CIEC_INT *) stStruct2.getMemberNamed(g_nStringIdVal2)) = 256;
    BOOST_CHECK_EQUAL(3.1415f, (*(CIEC_REAL *)stStruct2.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(256, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(false, (*(CIEC_BOOL *)stStruct2.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal4)));

    (*(CIEC_BOOL *) stStruct2.getMemberNamed(g_nStringIdVal3)) = true;
    BOOST_CHECK_EQUAL(3.1415f, (*(CIEC_REAL *)stStruct2.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(256, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)stStruct2.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(0, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal4)));

    (*(CIEC_INT *) stStruct2.getMemberNamed(g_nStringIdVal4)) = 31234;
    BOOST_CHECK_EQUAL(3.1415f, (*(CIEC_REAL *)stStruct2.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(256, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)stStruct2.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(31234, (*(CIEC_INT *)stStruct2.getMemberNamed(g_nStringIdVal4)));
  }

  BOOST_AUTO_TEST_CASE(Struct_copy_test_teststruct1){
    CIEC_TestStruct1 stStruct1;
    CIEC_TestStruct1 stStruct2;

    setupTestStruct1_TestDataSet1(stStruct1);
    checkTestStruct1_TestDataSet1(stStruct1);
    checkTestStruct1_InitialValues(stStruct2);

    //self assign
    stStruct1 = stStruct1;
    checkTestStruct1_TestDataSet1(stStruct1);

    //default initialized self assign
    stStruct2 = stStruct2;
    checkTestStruct1_InitialValues(stStruct2);

    //copy struct via assignment operator
    stStruct2 = stStruct1;
    checkTestStruct1_TestDataSet1(stStruct2);
  }

  BOOST_AUTO_TEST_CASE(Struct_copy_test_teststruct2){
    CIEC_TestStruct2 stStruct1;
    CIEC_TestStruct2 stStruct2;

    setupTestStruct2_TestDataSet1(stStruct1);
    checkTestStruct2_InitialValues(stStruct2);

    //self assign
    stStruct1 = stStruct1;
    checkTestStruct2_TestDataSet1(stStruct1);

    //default initialized self assign
    stStruct2 = stStruct2;
    checkTestStruct2_InitialValues(stStruct2);

    //copy struct via assignment operator
    stStruct2 = stStruct1;
    checkTestStruct2_TestDataSet1(stStruct2);
  }

  const char cTestFromString_String1[] = "String1";

  void checkTestStruct1_fromStringTestData(CIEC_TestStruct1 &pa_stStruct){
    BOOST_CHECK_EQUAL(strcmp((*(CIEC_STRING *)pa_stStruct.getMemberNamed(g_nStringIdVal1)).getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(23451, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
  }

  BOOST_AUTO_TEST_CASE(Struct_fromString_TestStruct1){
    CIEC_TestStruct1 stStruct1;
    CIEC_TestStruct1 stStruct2; //emtpy struct for resting the values after each parse

    const char cTestString1[] = { "(Val1:='String1',Val2:=true,Val3:=23451)" };
    const char cTestString2[] = { "( Val1:='String1', Val2:=true, Val3   :=23451)" };
    const char cTestString3[] = { "(Val1:='String1'  , Val2:=true, Val3:=  23451)" };
    const char cTestString4[] = { "(  Val1:=  'String1',Val2:=   true, Val3:=23451)" };
    const char cTestString5[] = { "(Val1:='String1', Val2 := true,Val3:=23451)" };
    const char cTestString6[] = { "(Val1:='String1', Val2:=true, Val3:=    23451    )" };
    const char cTestString7[] = { "(Val2:=true, Val1:='String1', Val3:= 23451)" };
    const char cTestString8[] = { "(Val2:=true, Val3:= 23451, Val1:='String1')" };
    const char cTestString9[] = { "(Val3:= 23451, Val1:='String1', Val2:=true)" };

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString1), strlen(cTestString1));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString2), strlen(cTestString2));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString3), strlen(cTestString3));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString4), strlen(cTestString4));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString5), strlen(cTestString5));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString6), strlen(cTestString6));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString7), strlen(cTestString7));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString8), strlen(cTestString8));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString9), strlen(cTestString9));
    checkTestStruct1_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    //negative tests: these strings should produce error responses
    const char cTestStringNIO1[] = { "(Val1:='String1', Val2:=true, Val3:=23451, seppi)" };
    const char cTestStringNIO2[] = { "(Val1:='String1', Val2:=1234, Val3:=23451)" };
    const char cTestStringNIO3[] = { "(Val1:='String1', Val2:=true, Val3:=23.3)" };
    const char cTestStringNIO4[] = { "(Val4:='String1', Val2:=true, Val3:=23.3)" };
    const char cTestStringNIO5[] = { "(Val2:='String1', Val2:=true, Val3:=23451)" };
    const char cTestStringNIO6[] = { "(Val1:='String1', Val3:=true, Val1:=23451)" };

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO1), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO2), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO3), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO4), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO5), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO6), -1);

  }

  void checkTestStruct2_fromStringTestData(CIEC_TestStruct2 &pa_stStruct){
    BOOST_CHECK_EQUAL(15.65789f, (*(CIEC_REAL *)pa_stStruct.getMemberNamed(g_nStringIdVal1)));
    BOOST_CHECK_EQUAL(32765, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal2)));
    BOOST_CHECK_EQUAL(true, (*(CIEC_BOOL *)pa_stStruct.getMemberNamed(g_nStringIdVal3)));
    BOOST_CHECK_EQUAL(13, (*(CIEC_INT *)pa_stStruct.getMemberNamed(g_nStringIdVal4)));
  }

  BOOST_AUTO_TEST_CASE(Struct_fromString_TestStruct2){
    CIEC_TestStruct2 stStruct1;
    CIEC_TestStruct2 stStruct2; //emtpy struct for resting the values after each parse

    const char cTestString1[] = { "(Val1:=15.65789,Val2:=32765,Val3:=TRUE, Val4:=13)" };
    const char cTestString2[] = { "(Val1 := 15.65789 , Val2:=     32765,Val3:= TRUE, Val4:=13)" };
    const char cTestString3[] = { "(Val1:=15.65789, Val2 := 32765 ,  Val3 :=    TRUE, Val4:=13   )" };
    const char cTestString4[] = { "( Val1:=15.65789 , Val2 := 32765,Val3:=TRUE, Val4:=13)" };
    const char cTestString5[] = { "(   Val1:=15.65789 , Val2:=32765,Val3:=TRUE, Val4:=13)" };
    const char cTestString6[] = { "(Val3:=TRUE, Val1:=15.65789,Val2:=32765,Val4:=13)" };
    const char cTestString7[] = { "(Val1:=15.65789,Val3:=TRUE, Val4:=13,Val2:=32765)" };
    const char cTestString8[] = { "(Val1:=15.65789,Val2:=32765,Val3:=TRUE, Val4:=13)" };
    const char cTestString9[] = { "(Val1:=15.65789, Val4:=13,Val3:=TRUE, Val2:=32765)" };

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString1), strlen(cTestString1));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString2), strlen(cTestString2));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString3), strlen(cTestString3));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString4), strlen(cTestString4));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString5), strlen(cTestString5));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString6), strlen(cTestString6));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString7), strlen(cTestString7));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString8), strlen(cTestString8));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString9), strlen(cTestString9));
    checkTestStruct2_fromStringTestData(stStruct1);
    stStruct1 = stStruct2;

    const char cTestStringNIO1[] = { "(Val1:=15.65789,Val2:=3,Val3:=TRUE, Val4:=13, seppi)" };
    const char cTestStringNIO2[] = { "(Hansi:=15.65789,Val2:=true,Val3:=TRUE, Val4:=13)" };
    const char cTestStringNIO3[] = { "(Val1:='test',Val2:=true,Val3:=TRUE, Val4:=13)" };
    const char cTestStringNIO4[] = { "(Val1:=15.65789 Val2:=23,Val3:=TRUE, Val4:=13)" };
    const char cTestStringNIO5[] = { "(Val1:=15.65789,Val2:=true,Val3:=TRUE, Val4:=13.13)" };

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO1), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO2), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO3), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO4), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO5), -1);
  }

  void toStringTest_testStruct1(const char* pa_acVal1, bool pa_nVal2, int pa_nVal3, const char* pa_acResult){
    CIEC_TestStruct1 stStruct;
    int nResultLenght = static_cast<int>(strlen(pa_acResult));
    char acBuffer[50];

    for(int i = (nResultLenght + 1); i < 50; i++){
      setDataTestStruct1(stStruct, pa_acVal1, pa_nVal2, pa_nVal3);
      BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
      BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
    }
    for(int i = 0; i <= nResultLenght; i++){
      BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), -1);
    }
  }

  BOOST_AUTO_TEST_CASE(Struct_toString_TestStruct1){
    toStringTest_testStruct1("", false, 0, "(Val1:='',Val2:=FALSE,Val3:=0)");
    toStringTest_testStruct1("", true, 0, "(Val1:='',Val2:=TRUE,Val3:=0)");
    toStringTest_testStruct1("Hansi Test", true, 0, "(Val1:='Hansi Test',Val2:=TRUE,Val3:=0)");
    toStringTest_testStruct1("", true, 2345, "(Val1:='',Val2:=TRUE,Val3:=2345)");
  }

  void toStringTest_testStruct2(float pa_fVal1, int pa_nVal2, bool pa_bVal3, int pa_nVal4, const char* pa_acResult){
      CIEC_TestStruct2 stStruct;
      int nResultLenght = static_cast<int>(strlen(pa_acResult));
      char acBuffer[60];

      for(int i = (nResultLenght + 1); i < 60; i++){
        setDataTestStruct2(stStruct, pa_fVal1, pa_nVal2, pa_bVal3, pa_nVal4);
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
        BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
      }
      for(int i = 0; i <= nResultLenght; i++){
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), -1);
      }
    }

  BOOST_AUTO_TEST_CASE(Struct_toString_TestStruct2){
    toStringTest_testStruct2(0.0f, 0, false, 0, "(Val1:=0,Val2:=0,Val3:=FALSE,Val4:=0)");
    toStringTest_testStruct2(0.0f, 0, true, 0, "(Val1:=0,Val2:=0,Val3:=TRUE,Val4:=0)");
    toStringTest_testStruct2(0.0f, 32767, false, -32768, "(Val1:=0,Val2:=32767,Val3:=FALSE,Val4:=-32768)");
    toStringTest_testStruct2(1345.13f, 32767, false, -32768, "(Val1:=1345.13,Val2:=32767,Val3:=FALSE,Val4:=-32768)");
    toStringTest_testStruct2(1345.13f, -32767, false, -32768, "(Val1:=1345.13,Val2:=-32767,Val3:=FALSE,Val4:=-32768)");

  }


  //
  BOOST_AUTO_TEST_SUITE_END()
