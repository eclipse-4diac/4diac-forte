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
#include "../../../src/core/datatypes/forte_array.h"

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

  /***********************************************************/
  class CIEC_TestStruct3 : public CIEC_STRUCT{
    DECLARE_FIRMWARE_DATATYPE(TestStruct3);

    /* Struct:
     *   val1 : String[3]
     *   val3 : BOOL
     *   val3 : INT[1]
     */

    public:
    CIEC_TestStruct3() :
          CIEC_STRUCT(g_nStringIdTestStruct3, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1){
      }

      virtual ~CIEC_TestStruct3(){
      }

      CIEC_STRING& val11(){
        return *static_cast<CIEC_STRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[0]))[0]);
      }

      CIEC_STRING& val12(){
        return *static_cast<CIEC_STRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[0]))[1]);
      }

      CIEC_BOOL& val2(){
        return *static_cast<CIEC_BOOL*>(&getMembers()[1]);
      }

      CIEC_INT& val31(){
        return *static_cast<CIEC_INT*>((*static_cast<CIEC_ARRAY *>(&getMembers()[2]))[0]);
      }


      static const unsigned int sizeOfFirstArray = 2;
      static const unsigned int sizeOfSecondArray = 1;

    private:
      static const CStringDictionary::TStringId scm_unElementTypes[];
      static const CStringDictionary::TStringId scm_unElementNames[];
  };

  const CStringDictionary::TStringId CIEC_TestStruct3::scm_unElementTypes[] = { g_nStringIdARRAY, sizeOfFirstArray, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdARRAY, sizeOfSecondArray, g_nStringIdINT };
  const CStringDictionary::TStringId CIEC_TestStruct3::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };

//*********************** TESTS *************************************************************************************************/
  void setDataTestStruct1(CIEC_TestStruct1 &paStruct, const char* paVal1, bool paVal2, int paVal3){
    (* static_cast<CIEC_STRING *>(paStruct.getMemberNamed(g_nStringIdVal1))) = paVal1;
    (* static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal2))) = paVal2;
    (* static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal3))) = static_cast<TForteInt16>(paVal3);
  }

  const char cTestStringData[] = "Check string!";
  const char cTestStringData2[] = "Check string 2!";

  void setupTestStruct1_TestDataSet1(CIEC_TestStruct1 &paStruct){
    setDataTestStruct1(paStruct, cTestStringData, true, 24534);
  }

  void checkTestStruct1_InitialValues(CIEC_TestStruct1 &paStruct){
    BOOST_CHECK_EQUAL((*static_cast<CIEC_STRING *>(paStruct.getMemberNamed(g_nStringIdVal1))).length(), 0);
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal3))));
  }

  void checkTestStruct1_TestDataSet1(CIEC_TestStruct1 &paStruct){
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(paStruct.getMemberNamed(g_nStringIdVal1))).getValue(), cTestStringData), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(24534, (*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal3))));
  }

  void setDataTestStruct2(CIEC_TestStruct2&paStruct, float paVal1, int paVal2, bool paVal3, int paVal4){
      (*static_cast<CIEC_REAL *>(paStruct.getMemberNamed(g_nStringIdVal1))) = paVal1;
      (*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal2))) = static_cast<TForteInt16>(paVal2);
      (*static_cast<CIEC_BOOL *>(paStruct.getMemberNamed(g_nStringIdVal3))) = paVal3;
      (*static_cast<CIEC_INT *>(paStruct.getMemberNamed(g_nStringIdVal4))) = static_cast<TForteInt16>(paVal4);
    }

  void setupTestStruct2_TestDataSet1(CIEC_TestStruct2 &paStruct){
    setDataTestStruct2(paStruct, 3.1415f, 256, true, 31234);
  }

  void checkTestStruct2_InitialValues(CIEC_TestStruct2 &pa_stStruct){
    BOOST_CHECK_EQUAL(0.0f, (*static_cast<CIEC_REAL *>(pa_stStruct.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(pa_stStruct.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal4))));
  }

  void checkTestStruct2_TestDataSet1(CIEC_TestStruct2 &pa_stStruct){
      BOOST_CHECK_EQUAL(3.1415f, (*static_cast<CIEC_REAL *>(pa_stStruct.getMemberNamed(g_nStringIdVal1))));
      BOOST_CHECK_EQUAL(256, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal2))));
      BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(pa_stStruct.getMemberNamed(g_nStringIdVal3))));
      BOOST_CHECK_EQUAL(31234, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal4))));
    }

  void setDataTestStruct3(CIEC_TestStruct3&paStruct, const char* paVal11, const char* paVal12, bool paVal2, int paVal31){
    paStruct.val11() = paVal11;
    paStruct.val12() = paVal12;
    paStruct.val2() = paVal2;
    paStruct.val31() = static_cast<TForteInt16>(paVal31);
  }

  void setupTestStruct3_TestDataSet1(CIEC_TestStruct3 &paStruct){
    setDataTestStruct3(paStruct, cTestStringData, cTestStringData2, true, 24534);
  }

  void checkTestStruct3_InitialValues(CIEC_TestStruct3 &pa_stStruct){
    BOOST_CHECK_EQUAL(0, pa_stStruct.val11().length());
    BOOST_CHECK_EQUAL(0, pa_stStruct.val12().length());
    BOOST_CHECK_EQUAL(false, pa_stStruct.val2());
    BOOST_CHECK_EQUAL(0, pa_stStruct.val31());
  }

  void checkTestStruct3_TestDataSet1(CIEC_TestStruct3 &pa_stStruct){
    BOOST_CHECK_EQUAL(strcmp(pa_stStruct.val11().getValue(), cTestStringData), 0);
    BOOST_CHECK_EQUAL(strcmp(pa_stStruct.val12().getValue(), cTestStringData2), 0);
    BOOST_CHECK_EQUAL(1, pa_stStruct.val2());
    BOOST_CHECK_EQUAL(24534, pa_stStruct.val31());
  }

  BOOST_AUTO_TEST_CASE(Struct_element_access_test){
    CIEC_TestStruct1 stStruct1;
    CIEC_TestStruct2 stStruct2;
    CIEC_TestStruct3 stStruct3;

    //***************** Test struct 1 ********************************************************
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_STRING);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_BOOL);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_INT);

    checkTestStruct1_InitialValues(stStruct1);

    char cTest[] = "Check string!";
    (*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))) = cTest;
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));

    (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))) = true;
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));

    (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))) = 24534;
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(24534, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));

    //***************** Test struct 2 ********************************************************
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_REAL);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_INT);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_BOOL);
    BOOST_CHECK_EQUAL(stStruct2.getMemberNamed(g_nStringIdVal4)->getDataTypeID(), CIEC_ANY::e_INT);

    checkTestStruct2_InitialValues(stStruct2);

    (*static_cast<CIEC_REAL *>(stStruct2.getMemberNamed(g_nStringIdVal1))) = 3.1415f;
    BOOST_CHECK_EQUAL(3.1415f, (*static_cast<CIEC_REAL *>(stStruct2.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct2.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal4))));

    (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal2))) = 256;
    BOOST_CHECK_EQUAL(3.1415f, (*static_cast<CIEC_REAL *>(stStruct2.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(256, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct2.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal4))));

    (*static_cast<CIEC_BOOL *>(stStruct2.getMemberNamed(g_nStringIdVal3))) = true;
    BOOST_CHECK_EQUAL(3.1415f, (*static_cast<CIEC_REAL *>(stStruct2.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(256, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct2.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal4))));

    (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal4))) = 31234;
    BOOST_CHECK_EQUAL(3.1415f, (*static_cast<CIEC_REAL *>(stStruct2.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(256, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct2.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(31234, (*static_cast<CIEC_INT *>(stStruct2.getMemberNamed(g_nStringIdVal4))));

    //***************** Test struct 3 ********************************************************
    char cTest2[] = "Check string 2!";
    BOOST_CHECK_EQUAL(stStruct3.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_ARRAY);
    for(size_t i = 0; i < CIEC_TestStruct3::sizeOfFirstArray; i++){
      BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(stStruct3.getMemberNamed(g_nStringIdVal1)))[i]->getDataTypeID(), CIEC_ANY::e_STRING);
    }
    BOOST_CHECK_EQUAL(stStruct3.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_BOOL);

    BOOST_CHECK_EQUAL(stStruct3.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_ARRAY);
    for(size_t i = 0; i < CIEC_TestStruct3::sizeOfSecondArray; i++){
      BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(stStruct3.getMemberNamed(g_nStringIdVal3)))[i]->getDataTypeID(), CIEC_ANY::e_INT);
    }

    checkTestStruct3_InitialValues(stStruct3);

    stStruct3.val11() = cTest;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(stStruct3.val12().length(), 0);
    BOOST_CHECK_EQUAL(false, stStruct3.val2());
    BOOST_CHECK_EQUAL(0, stStruct3.val31());

    stStruct3.val12() = cTest2;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
    BOOST_CHECK_EQUAL(false, stStruct3.val2());
    BOOST_CHECK_EQUAL(0, stStruct3.val31());

    stStruct3.val2() = true;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
    BOOST_CHECK_EQUAL(true, stStruct3.val2());
    BOOST_CHECK_EQUAL(0, stStruct3.val31());

    stStruct3.val31() = 24534;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
    BOOST_CHECK_EQUAL(true, stStruct3.val2());
    BOOST_CHECK_EQUAL(24534, stStruct3.val31());

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

  BOOST_AUTO_TEST_CASE(Struct_copy_test_teststruct3){
    CIEC_TestStruct3 stStruct1;
    CIEC_TestStruct3 stStruct2;

    setupTestStruct3_TestDataSet1(stStruct1);
    checkTestStruct3_InitialValues(stStruct2);

    //self assign
    stStruct1 = stStruct1;
    checkTestStruct3_TestDataSet1(stStruct1);

    //default initialized self assign
    stStruct2 = stStruct2;
    checkTestStruct3_InitialValues(stStruct2);

    //copy struct via assignment operator
    stStruct2 = stStruct1;
    checkTestStruct3_TestDataSet1(stStruct2);
  }

  const char cTestFromString_String1[] = "String1";
  const char cTestFromString_String2[] = "String2";

  void checkTestStruct1_fromStringTestData(CIEC_TestStruct1 &pa_stStruct){
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(pa_stStruct.getMemberNamed(g_nStringIdVal1))).getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(pa_stStruct.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal3))));
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
    BOOST_CHECK_EQUAL(15.65789f, (*static_cast<CIEC_REAL *>(pa_stStruct.getMemberNamed(g_nStringIdVal1))));
    BOOST_CHECK_EQUAL(32765, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(pa_stStruct.getMemberNamed(g_nStringIdVal3))));
    BOOST_CHECK_EQUAL(13, (*static_cast<CIEC_INT *>(pa_stStruct.getMemberNamed(g_nStringIdVal4))));
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

  void checkTestStruct3_fromStringTestData(CIEC_TestStruct3 &pa_stStruct){
    BOOST_CHECK_EQUAL(strcmp(pa_stStruct.val11().getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(strcmp(pa_stStruct.val12().getValue(), cTestFromString_String2), 0);
    BOOST_CHECK_EQUAL(true, pa_stStruct.val2());
    BOOST_CHECK_EQUAL(13, pa_stStruct.val31());
  }

  BOOST_AUTO_TEST_CASE(Struct_fromString_TestStruct3){
    CIEC_TestStruct3 stStruct1;
    CIEC_TestStruct3 stStruct2; //emtpy struct for resting the values after each parse

    const char cTestString1[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13])" };
    const char cTestString2[] = { "(Val1 := [\'String1\',\'String2\'] , Val2:=     TRUE, Val3:= [13])" };
    const char cTestString3[] = { "(Val1:=[\'String1\',\'String2\'], Val2 := TRUE ,  Val3 :=    [13]   )" };
    const char cTestString4[] = { "( Val1:=[\'String1\',\'String2\'] , Val2 := TRUE, Val3:=[13])" };
    const char cTestString5[] = { "(   Val1:=[\'String1\',\'String2\'] , Val2:=TRUE, Val3:=[13])" };
    const char cTestString6[] = { "(Val3:=[13], Val1:=[\'String1\',\'String2\'],Val2:=TRUE)" };
    const char cTestString7[] = { "(Val1:=[\'String1\',\'String2\'],Val3:=[13], Val2:=TRUE)" };
    const char cTestString8[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE, Val3:=[13])" };
    const char cTestString9[] = { "(Val1:=[\'String1\',\'String2\'], Val3:=[13], Val2:=TRUE)" };

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString1), strlen(cTestString1));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString2), strlen(cTestString2));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString3), strlen(cTestString3));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString4), strlen(cTestString4));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString5), strlen(cTestString5));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString6), strlen(cTestString6));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString7), strlen(cTestString7));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString8), strlen(cTestString8));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString9), strlen(cTestString9));
    checkTestStruct3_fromStringTestData (stStruct1);
    stStruct1 = stStruct2;

    const char cTestStringNIO1[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13], seppi)" };
    const char cTestStringNIO2[] = { "(Hansi:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13])" };
    const char cTestStringNIO3[] = { "(Val1:='test',Val2:=TRUE,Val3:=[13])" };
    const char cTestStringNIO4[] = { "(Val1:=[\'String1\',\'String2\'] Val2:=23,Val3:=[13])" };
    const char cTestStringNIO5[] = { "(Val1:=15.65789,Val2:=TRUE,Val3:=13)" }; //wrong val4

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

  void toStringTest_testStruct3(const char* pa_acVal11, const char* pa_acVal12, bool pa_nVal2, int pa_nVal3, const char* pa_acResult){
      CIEC_TestStruct3 stStruct;
      int nResultLenght = static_cast<int>(strlen(pa_acResult));
      char acBuffer[70];

      for(int i = (nResultLenght + 1); i < 70; i++){
        setDataTestStruct3(stStruct, pa_acVal11, pa_acVal12, pa_nVal2, pa_nVal3);
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
        BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
      }
      for(int i = 0; i <= nResultLenght; i++){
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), -1);
      }
    }

    BOOST_AUTO_TEST_CASE(Struct_toString_TestStruct3){
      toStringTest_testStruct3("", "", false, 0, "(Val1:=['',''],Val2:=FALSE,Val3:=[0])");
      toStringTest_testStruct3("", "", true, 0, "(Val1:=['',''],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("Hansi Test", "", true, 0, "(Val1:=['Hansi Test',''],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("Hansi Test", "Hansi Test 2", true, 0, "(Val1:=['Hansi Test','Hansi Test 2'],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("", "", true, 2345, "(Val1:=['',''],Val2:=TRUE,Val3:=[2345])");
    }

  //
  BOOST_AUTO_TEST_SUITE_END()
