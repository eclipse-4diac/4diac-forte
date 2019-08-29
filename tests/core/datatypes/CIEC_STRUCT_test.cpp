/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

/***********************************************************/
class CIEC_TestStruct1 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(TestStruct1)
    ;
  public:
    CIEC_TestStruct1();

    virtual ~CIEC_TestStruct1() {
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

const CStringDictionary::TStringId CIEC_TestStruct1::scm_unElementTypes[] = { g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdINT };
const CStringDictionary::TStringId CIEC_TestStruct1::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };

DEFINE_FIRMWARE_DATATYPE(TestStruct1, g_nStringIdTestStruct1)
CIEC_TestStruct1::CIEC_TestStruct1() :
    CIEC_STRUCT(g_nStringIdTestStruct1, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}
  /***********************************************************/
  class CIEC_TestStruct2 : public CIEC_STRUCT{
    DECLARE_FIRMWARE_DATATYPE(TestStruct2)
      ;
    public:
    CIEC_TestStruct2();

      virtual ~CIEC_TestStruct2(){
      }

    private:
      static const CStringDictionary::TStringId scm_unElementTypes[];
      static const CStringDictionary::TStringId scm_unElementNames[];
  };

  const CStringDictionary::TStringId CIEC_TestStruct2::scm_unElementTypes[] = { g_nStringIdREAL, g_nStringIdINT, g_nStringIdBOOL, g_nStringIdINT };
  const CStringDictionary::TStringId CIEC_TestStruct2::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3, g_nStringIdVal4 };
DEFINE_FIRMWARE_DATATYPE(TestStruct2, g_nStringIdTestStruct2)
CIEC_TestStruct2::CIEC_TestStruct2() :
    CIEC_STRUCT(g_nStringIdTestStruct2, 4, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}
  /***********************************************************/
  class CIEC_TestStruct3 : public CIEC_STRUCT{
    DECLARE_FIRMWARE_DATATYPE(TestStruct3);

    /* Struct:
     *   val1 : String[3]
     *   val3 : BOOL
     *   val3 : INT[1]
     */

    public:
    CIEC_TestStruct3();

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
    static CIEC_STRUCT::TStructInitialValues scmInitialValues[];
  };

  const CStringDictionary::TStringId CIEC_TestStruct3::scm_unElementTypes[] = { g_nStringIdARRAY, sizeOfFirstArray, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdARRAY, sizeOfSecondArray, g_nStringIdINT };
  const CStringDictionary::TStringId CIEC_TestStruct3::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };
CIEC_STRUCT::TStructInitialValues CIEC_TestStruct3::scmInitialValues[] = { 0, "true", "[9]" };
DEFINE_FIRMWARE_DATATYPE(TestStruct3, g_nStringIdTestStruct3)
CIEC_TestStruct3::CIEC_TestStruct3() :
    CIEC_STRUCT(g_nStringIdTestStruct3, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1, scmInitialValues) {
}

class CIEC_TestStruct4 : public CIEC_STRUCT { //undefined data types
  DECLARE_FIRMWARE_DATATYPE(TestStruct4)

    /* Struct:
     *   val1 : String[3]
     *   val3 : BOOL
     *   val3 : INT
     */

  public:
    CIEC_TestStruct4();

    virtual ~CIEC_TestStruct4() {
    }

    CIEC_STRING& val11() {
      return *static_cast<CIEC_STRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[0]))[0]);
    }

    CIEC_STRING& val12() {
      return *static_cast<CIEC_STRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[0]))[1]);
    }

    CIEC_BOOL& val2() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[1]);
    }

    CIEC_INT& val3() {
      return *static_cast<CIEC_INT*>(&getMembers()[1]);
    }

    static const unsigned int sizeOfFirstArray = 2;

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
    static CIEC_STRUCT::TStructInitialValues scmInitialValues[];
};

const CStringDictionary::TStringId CIEC_TestStruct4::scm_unElementTypes[] = { g_nStringIdARRAY, sizeOfFirstArray, g_nStringIdSTRING, g_nStringIdBOOL,
  g_nStringIdUNDEFINEDDATATYPE };
const CStringDictionary::TStringId CIEC_TestStruct4::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2, g_nStringIdVal3 };
CIEC_STRUCT::TStructInitialValues CIEC_TestStruct4::scmInitialValues[] = { "['a', 'b', '']", 0, "9" };
DEFINE_FIRMWARE_DATATYPE(TestStruct4, g_nStringIdTestStruct4)
CIEC_TestStruct4::CIEC_TestStruct4() :
    CIEC_STRUCT(g_nStringIdTestStruct4, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1, scmInitialValues) {
}

BOOST_AUTO_TEST_SUITE (CIEC_STRUCT_function_test)

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
    BOOST_CHECK_EQUAL(0, pa_stStruct.val12().length());
    BOOST_CHECK_EQUAL(0, pa_stStruct.val11().length());
  BOOST_CHECK_EQUAL(true, pa_stStruct.val2());
  BOOST_CHECK_EQUAL(9, pa_stStruct.val31());
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

  BOOST_CHECK_EQUAL(stStruct1.getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(stStruct2.getDataTypeID(), CIEC_ANY::e_STRUCT);
  BOOST_CHECK_EQUAL(stStruct3.getDataTypeID(), CIEC_ANY::e_STRUCT);

    //***************** Test struct 1 ********************************************************
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal1)->getDataTypeID(), CIEC_ANY::e_STRING);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_BOOL);
    BOOST_CHECK_EQUAL(stStruct1.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_INT);

  BOOST_CHECK_EQUAL(stStruct1.getMembers()[0].getDataTypeID(), CIEC_ANY::e_STRING);
  BOOST_CHECK_EQUAL(stStruct1.getMembers()[1].getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(stStruct1.getMembers()[2].getDataTypeID(), CIEC_ANY::e_INT);

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

  BOOST_CHECK_EQUAL(stStruct2.getMembers()[0].getDataTypeID(), CIEC_ANY::e_REAL);
  BOOST_CHECK_EQUAL(stStruct2.getMembers()[1].getDataTypeID(), CIEC_ANY::e_INT);
  BOOST_CHECK_EQUAL(stStruct2.getMembers()[2].getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(stStruct2.getMembers()[3].getDataTypeID(), CIEC_ANY::e_INT);

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
  BOOST_CHECK_EQUAL(stStruct3.getMembers()[0].getDataTypeID(), CIEC_ANY::e_ARRAY);
    for(size_t i = 0; i < CIEC_TestStruct3::sizeOfFirstArray; i++){
      BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(stStruct3.getMemberNamed(g_nStringIdVal1)))[i]->getDataTypeID(), CIEC_ANY::e_STRING);
    BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(&stStruct3.getMembers()[0]))[i]->getDataTypeID(), CIEC_ANY::e_STRING);
    }
    BOOST_CHECK_EQUAL(stStruct3.getMemberNamed(g_nStringIdVal2)->getDataTypeID(), CIEC_ANY::e_BOOL);
  BOOST_CHECK_EQUAL(stStruct3.getMembers()[1].getDataTypeID(), CIEC_ANY::e_BOOL);

    BOOST_CHECK_EQUAL(stStruct3.getMemberNamed(g_nStringIdVal3)->getDataTypeID(), CIEC_ANY::e_ARRAY);
    for(size_t i = 0; i < CIEC_TestStruct3::sizeOfSecondArray; i++){
      BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(stStruct3.getMemberNamed(g_nStringIdVal3)))[i]->getDataTypeID(), CIEC_ANY::e_INT);
    BOOST_CHECK_EQUAL((*static_cast<CIEC_ARRAY *>(&stStruct3.getMembers()[2]))[i]->getDataTypeID(), CIEC_ANY::e_INT);
    }

    checkTestStruct3_InitialValues(stStruct3);

    stStruct3.val11() = cTest;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(stStruct3.val12().length(), 0);
  BOOST_CHECK_EQUAL(true, stStruct3.val2());
  BOOST_CHECK_EQUAL(9, stStruct3.val31());

    stStruct3.val12() = cTest2;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
  BOOST_CHECK_EQUAL(true, stStruct3.val2());
  BOOST_CHECK_EQUAL(9, stStruct3.val31());

  stStruct3.val2() = false;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
  BOOST_CHECK_EQUAL(false, stStruct3.val2());
  BOOST_CHECK_EQUAL(9, stStruct3.val31());

    stStruct3.val31() = 24534;
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val11().getValue(), cTest), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct3.val12().getValue(), cTest2), 0);
  BOOST_CHECK_EQUAL(false, stStruct3.val2());
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

  CIEC_TestStruct1 stStruct3(stStruct1);
  checkTestStruct1_TestDataSet1 (stStruct3);

  CIEC_TestStruct1 stStruct4;
  stStruct4.setValue(stStruct1);
  checkTestStruct1_TestDataSet1 (stStruct4);

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

  CIEC_TestStruct2 stStruct3(stStruct1);
  checkTestStruct2_TestDataSet1 (stStruct3);

  CIEC_TestStruct2 stStruct4;
  stStruct4.setValue(stStruct1);
  checkTestStruct2_TestDataSet1 (stStruct4);
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

  CIEC_TestStruct3 stStruct3(stStruct1);
  checkTestStruct3_TestDataSet1 (stStruct3);

  CIEC_TestStruct3 stStruct4;
  stStruct4.setValue(stStruct1);
  checkTestStruct3_TestDataSet1 (stStruct4);

  CIEC_TestStruct2 stStructOtherType;
  stStruct4.setValue(stStructOtherType); //shouldn't change or break anything
  checkTestStruct3_TestDataSet1 (stStruct4);

  CIEC_INT notStructType;
  stStruct4.setValue(notStructType); //shouldn't change or break anything
  checkTestStruct3_TestDataSet1 (stStruct4);

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

    const char cTestString10[] = { "(Val1:='String1',Val2:=true)" }; //missing last value

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString10), strlen(cTestString10));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));
    stStruct1 = stStruct2;

    const char cTestString11[] = { "(Val1:='String1',Val3:=23451)" }; //missing value in the middle

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString11), strlen(cTestString11));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));
    stStruct1 = stStruct2;

    const char cTestString12[] = { "(Val2:=true,Val3:=23451)" }; //missing first value

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString12), strlen(cTestString12));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), ""), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));
    stStruct1 = stStruct2;

    const char cTestString13[] = { "(Val2:=true)" }; //missing two values

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString13), strlen(cTestString13));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), ""), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(0, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));
    stStruct1 = stStruct2;

    const char cTestString14[] = { "()" }; //missing all values
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString14), strlen(cTestString14));
    checkTestStruct1_InitialValues(stStruct1);

    //don't initialize structure for next tests

    BOOST_CHECK_EQUAL(stStruct1.fromString("(Val3:=23451)"), strlen("(Val3:=23451)"));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), ""), 0);
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3))));

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString13), strlen(cTestString13)); //"(Val2:=true)"
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), ""), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3)))); //keeps old value

    BOOST_CHECK_EQUAL(stStruct1.fromString("(Val1:='String1')"), strlen("(Val1:='String1')")); //"(Val1:='String1')"
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2)))); //keeps old value
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3)))); //keeps old value

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString14), strlen(cTestString14));
    BOOST_CHECK_EQUAL(strcmp((*static_cast<CIEC_STRING *>(stStruct1.getMemberNamed(g_nStringIdVal1))).getValue(), cTestFromString_String1), 0); //keeps old value
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2)))); //keeps old value
    BOOST_CHECK_EQUAL(23451, (*static_cast<CIEC_INT *>(stStruct1.getMemberNamed(g_nStringIdVal3)))); //keeps old value

    //negative tests: these strings should produce error responses
    const char cTestStringNIO1[] = { "(Val1:='String1', Val2:=true, Val3:=23451, seppi)" };
    const char cTestStringNIO2[] = { "(Val1:='String1', Val2:=1234, Val3:=23451)" };
    const char cTestStringNIO3[] = { "(Val1:='String1', Val2:=true, Val3:=23.3)" };
    const char cTestStringNIO4[] = { "(Val4:='String1', Val2:=true, Val3:=23.3)" };
    const char cTestStringNIO5[] = { "(Val2:='String1', Val2:=true, Val3:=23451)" };
    const char cTestStringNIO6[] = { "(Val1:='String1', Val3:=true, Val1:=23451)" };
    const char cTestStringNIO7[] = { "Val1:='String1',Val2:=true,Val3:=23451)" }; //missing opening (
    const char cTestStringNIO8[] = { "(Val1:='String1',Val2:=true,Val3:=23451" }; //missing closing )
    const char cTestStringNIO9[] = { "(Val1:='String1',Val2x:=true,Val3:=23451)" }; //wrong identifier
    const char cTestStringNIO10[] = { "(Val1:='String1',Val2=true,Val3:=23451)" }; //wrong assigner
    const char cTestStringNIO11[] = { "(Val1:='String1',Val2::true,Val3:=23451)" }; //wrong assigner 2
    const char cTestStringNIO12[] = { "(Val1:='String1',Val123456789012345678901234567890:=true,Val3:=23451)" }; //identifier too long
    const char cTestStringNIO13[] = { "(Val1:='String1',Val1):=true,Val3:=23451)" }; //identifier with closing )
    const char cTestStringNIO14[] = { "(Val1:='String1',Val1:=true,Val3" }; //identifier without value
    const char cTestStringNIO15[] = { "(Val1:='String1',Val1:=)true,Val3:=23451)" }; //closing ) after assigner
    const char cTestStringNIO16[] = { "(Val1:='String1',Val1:=true,Val3:=" }; //end of string after assigner

    stStruct1 = stStruct2;

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO1), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO2), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO3), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO4), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO5), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO6), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO7), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO8), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO9), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO10), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO11), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO12), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO13), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO14), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO15), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO16), -1);

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

  BOOST_AUTO_TEST_CASE(Struct_fromString_TestStruct3) {
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

    BOOST_CHECK_EQUAL(stStruct1.getASN1StructType(), 64 + 32 + 1); //CIEC_STRUCT::e_APPLICATION + CIEC_STRUCT::e_CONSTRUCTED + 1
    BOOST_CHECK_EQUAL(stStruct1.getStructSize(), 3);
    const CStringDictionary::TStringId* names = stStruct1.elementNames();
    BOOST_CHECK_EQUAL(names[0], g_nStringIdVal1);
    BOOST_CHECK_EQUAL(names[1], g_nStringIdVal2);
    BOOST_CHECK_EQUAL(names[2], g_nStringIdVal3);
    BOOST_CHECK_EQUAL(stStruct1.getStructTypeNameID(), g_nStringIdTestStruct3);

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

    const char cTestString10[] = { "(Val2:=TRUE,Val3:=[13])" }; //missing first value, without initializer, other two with initializer

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString10), strlen(cTestString10));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(true, stStruct1.val2());
    BOOST_CHECK_EQUAL(13, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString11[] = { "(Val1:=[\'String1\',\'String2\'],Val3:=[13])" }; //missing value in the middle, with initializer

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString11), strlen(cTestString11));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0);
    BOOST_CHECK_EQUAL(true, stStruct1.val2());
    BOOST_CHECK_EQUAL(13, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString12[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE)" }; //missing last value, with initializer

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString12), strlen(cTestString12));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0);
    BOOST_CHECK_EQUAL(true, stStruct1.val2());
    BOOST_CHECK_EQUAL(9, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString13[] = { "(Val1:=[\'String1\',\'String2\'])" }; //missing two values with initializer

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString13), strlen(cTestString13));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0);
    BOOST_CHECK_EQUAL(true, stStruct1.val2());
    BOOST_CHECK_EQUAL(9, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString14[] = { "(Val2:=FALSE)" }; //missing two values, one with and the other without initializer

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString14), strlen(cTestString14));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(false, stStruct1.val2());
    BOOST_CHECK_EQUAL(9, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString15[] = { "()" }; //missing all values

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString15), strlen(cTestString15));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), ""), 0);
    BOOST_CHECK_EQUAL(true, stStruct1.val2());
    BOOST_CHECK_EQUAL(9, stStruct1.val31());
    stStruct1 = stStruct2;

    const char cTestString16[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13],Val2:=FALSE)" }; //identifier duplicate. No error and take last value?
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString16), strlen(cTestString16));
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0);
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0);
    BOOST_CHECK_EQUAL(false, stStruct1.val2());
    BOOST_CHECK_EQUAL(13, stStruct1.val31());

    //don't initialize structure for next tests
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString12), strlen(cTestString12)); //"(Val1:=[\'String1\',\'String2\'],Val2:=TRUE)"
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0); //keeps old value
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0); //keeps old value
    BOOST_CHECK_EQUAL(true, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(13, stStruct1.val31()); //keeps old value

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString14), strlen(cTestString14)); //"(Val2:=FALSE)"
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0); //keeps old value
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0); //keeps old value
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2))));
    BOOST_CHECK_EQUAL(13, stStruct1.val31()); //keeps old value

    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestString15), strlen(cTestString15)); //"()"
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val11().getValue(), cTestFromString_String1), 0); //keeps old value
    BOOST_CHECK_EQUAL(strcmp(stStruct1.val12().getValue(), cTestFromString_String2), 0); //keeps old value
    BOOST_CHECK_EQUAL(false, (*static_cast<CIEC_BOOL *>(stStruct1.getMemberNamed(g_nStringIdVal2)))); //keeps old value
    BOOST_CHECK_EQUAL(13, stStruct1.val31()); //keeps old value

    const char cTestStringNIO1[] = { "(Val1:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13], seppi)" };
    const char cTestStringNIO2[] = { "(Hansi:=[\'String1\',\'String2\'],Val2:=TRUE,Val3:=[13])" };
    const char cTestStringNIO3[] = { "(Val1:='test',Val2:=TRUE,Val3:=[13])" };
    const char cTestStringNIO4[] = { "(Val1:=[\'String1\',\'String2\'] Val2:=23,Val3:=[13])" };
    const char cTestStringNIO5[] = { "(Val1:=15.65789,Val2:=TRUE,Val3:=13)" };

    stStruct1 = stStruct2;
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO1), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO2), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO3), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO4), -1);
    BOOST_CHECK_EQUAL(stStruct1.fromString(cTestStringNIO5), -1);
  }

  void toStringTest_testStruct1(const char* pa_acVal1, bool pa_nVal2, int pa_nVal3, const char* pa_acResult){
    CIEC_TestStruct1 stStruct;
  BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:='',Val2:=FALSE,Val3:=+32767)"));
    int nResultLenght = static_cast<int>(strlen(pa_acResult));
    char acBuffer[50];

    for(int i = (nResultLenght + 1); i < 50; i++){
      setDataTestStruct1(stStruct, pa_acVal1, pa_nVal2, pa_nVal3);
      BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
      BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
    BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:='',Val2:=FALSE,Val3:=+32767)") + strlen(pa_acVal1));
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
  BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:=123456789.123,Val2:=+32767,Val3:=FALSE,Val4:=+32767)"));
      int nResultLenght = static_cast<int>(strlen(pa_acResult));
      char acBuffer[60];

      for(int i = (nResultLenght + 1); i < 60; i++){
        setDataTestStruct2(stStruct, pa_fVal1, pa_nVal2, pa_bVal3, pa_nVal4);
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
        BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
    BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:=123456789.123,Val2:=+32767,Val3:=FALSE,Val4:=+32767)"));
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
  BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:=['',''],Val2:=FALSE,Val3:=[+32767])"));
      int nResultLenght = static_cast<int>(strlen(pa_acResult));
      char acBuffer[70];

      for(int i = (nResultLenght + 1); i < 70; i++){
        setDataTestStruct3(stStruct, pa_acVal11, pa_acVal12, pa_nVal2, pa_nVal3);
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), nResultLenght);
        BOOST_CHECK_EQUAL(strcmp(acBuffer, pa_acResult), 0);
    BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:=['',''],Val2:=FALSE,Val3:=[+32767])") + strlen(pa_acVal11) + strlen(pa_acVal12));
      }
      for(int i = 0; i <= nResultLenght; i++){
        BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, i), -1);
      }
    }

    BOOST_AUTO_TEST_CASE(Struct_toString_TestStruct3){
  CIEC_TestStruct3 stStruct;
  const char* initialString = "(Val1:=['',''],Val2:=TRUE,Val3:=[9])";
  char acBuffer[70];
  BOOST_CHECK_EQUAL(stStruct.toString(acBuffer, 70), strlen(initialString));
  BOOST_CHECK_EQUAL(strcmp(acBuffer, initialString), 0);
  BOOST_CHECK_EQUAL(stStruct.getToStringBufferSize(), sizeof("(Val1:=['',''],Val2:=FALSE,Val3:=[+32767])"));

      toStringTest_testStruct3("", "", false, 0, "(Val1:=['',''],Val2:=FALSE,Val3:=[0])");
      toStringTest_testStruct3("", "", true, 0, "(Val1:=['',''],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("Hansi Test", "", true, 0, "(Val1:=['Hansi Test',''],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("Hansi Test", "Hansi Test 2", true, 0, "(Val1:=['Hansi Test','Hansi Test 2'],Val2:=TRUE,Val3:=[0])");
      toStringTest_testStruct3("", "", true, 2345, "(Val1:=['',''],Val2:=TRUE,Val3:=[2345])");
    }

  void checkEmptyStruct(CIEC_STRUCT& paStruct) {
    BOOST_CHECK_EQUAL(paStruct.getASN1StructType(), 0);
    BOOST_CHECK_EQUAL(paStruct.getStructSize(), 0);
    const CStringDictionary::TStringId* names = paStruct.elementNames();
    BOOST_CHECK(0 == paStruct.elementNames());
    BOOST_CHECK_EQUAL(paStruct.getStructTypeNameID(), 0);
    BOOST_CHECK_EQUAL(paStruct.getDataTypeID(), CIEC_ANY::e_STRUCT);
    BOOST_CHECK(0 == paStruct.getMembers());
    BOOST_CHECK(0 == paStruct.getMemberNamed(0));

    char acBuffer[3];
    BOOST_CHECK_EQUAL(paStruct.getToStringBufferSize(), sizeof("()"));
    BOOST_CHECK_EQUAL(paStruct.toString(acBuffer, 3), 2);
    BOOST_CHECK_EQUAL(strcmp(acBuffer, "()"), 0);
  }

  BOOST_AUTO_TEST_CASE(Struct_EmptyStruct) {
    CIEC_STRUCT nTest(0, 0, 0, 0, 64 + 32 + 1); //CIEC_STRUCT::e_APPLICATION + CIEC_STRUCT::e_CONSTRUCTED + 1
    checkEmptyStruct(nTest);

    CIEC_STRUCT nTest1(nTest);
    checkEmptyStruct(nTest1);
  }

  BOOST_AUTO_TEST_CASE(Struct_UndefinedDataTypes) {
    CIEC_TestStruct4 nTest;
    checkEmptyStruct(nTest);
  }

  BOOST_AUTO_TEST_SUITE_END()
