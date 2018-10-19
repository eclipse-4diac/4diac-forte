/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtester.h"
#include "../../modules/utils/GET_STRUCT_VALUE.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GET_STRUCT_VALUE_tester_gen.cpp"
#endif

class CIEC_GET_STRUCT_VALUE_Struct_test1 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test1)

    /* Struct:
     *   Val1 : String[3]
     *   Val2 : INT
     */

  public:
    CIEC_GET_STRUCT_VALUE_Struct_test1() :
        CIEC_STRUCT(g_nStringIdGET_STRUCT_VALUE_Struct_test1, 2, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
    }

    virtual ~CIEC_GET_STRUCT_VALUE_Struct_test1() {
    }

    static const unsigned int sizeOfFirstArray = 2;

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test1::scm_unElementTypes[] = { g_nStringIdARRAY, sizeOfFirstArray, g_nStringIdSTRING,
  g_nStringIdINT };
const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test1::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2 };

DEFINE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test1, g_nStringIdGET_STRUCT_VALUE_Struct_test1)

class CIEC_GET_STRUCT_VALUE_Struct_test2 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test2)

    /* Struct:
     *   Val1 : INT
     *   Val2 : CIEC_GET_STRUCT_VALUE_Struct_test1
     */

  public:
    CIEC_GET_STRUCT_VALUE_Struct_test2() :
        CIEC_STRUCT(g_nStringIdGET_STRUCT_VALUE_Struct_test2, 2, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
    }

    virtual ~CIEC_GET_STRUCT_VALUE_Struct_test2() {
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test2::scm_unElementTypes[] = { g_nStringIdINT, g_nStringIdGET_STRUCT_VALUE_Struct_test1 };
const CStringDictionary::TStringId CIEC_GET_STRUCT_VALUE_Struct_test2::scm_unElementNames[] = { g_nStringIdVal1, g_nStringIdVal2 };

DEFINE_FIRMWARE_DATATYPE(GET_STRUCT_VALUE_Struct_test2, g_nStringIdGET_STRUCT_VALUE_Struct_test2)

class GET_STRUCT_VALUE_tester_generic : public CFBTester {
  protected:
    CIEC_STRING mMember;
    CIEC_BOOL mQO;

    virtual void executeAllTests() = 0;

    explicit GET_STRUCT_VALUE_tester_generic(CResource* mTestResource, CIEC_ANY *paIN_STRUCT, CIEC_ANY *paOUT) :
        CFBTester(mTestResource) {
      SETUP_INPUTDATA(paIN_STRUCT, &mMember);
      SETUP_OUTPUTDATA(&mQO, paOUT);
    }
};

class GET_STRUCT_VALUE_tester_MAIN : public GET_STRUCT_VALUE_tester_generic {
  DECLARE_FB_TESTER(GET_STRUCT_VALUE_tester_MAIN)
  private:
    explicit GET_STRUCT_VALUE_tester_MAIN(CResource* mTestResource) :
        GET_STRUCT_VALUE_tester_generic(mTestResource, &mIn_struct, &mOut) {
    }

    virtual void executeAllTests() {
      mIn_struct.fromString("(Val1:=1,Val2:=(Val1:=['strin1','string2',''], Val2:=2))");
      evaluateTestResult(testCase_firstLevel(), "First Level");
      evaluateTestResult(testCase_secondLevel(), "Second Level");
      evaluateTestResult(testCase_firstLevelWrongName(), "First Level Wrong Name");
      evaluateTestResult(testCase_secondLevelWrongName(), "Second Level Wrong Name");
      evaluateTestResult(testCase_accessNonStruct(), "Access non struct");
      evaluateTestResult(testCase_wrongOutputType(), "Wrong output type");
    }

    /***********************************************************************************/
    bool testCase_firstLevel() {
      mMember = "Val1";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 1 && mOut == 1);
    }

    bool testCase_secondLevel() {
      mMember = "Val2.Val2";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 1 && mOut == 2);
    }

    bool testCase_firstLevelWrongName() {
      mMember = "xVal1";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 0);
    }

    bool testCase_secondLevelWrongName() {
      mMember = "Val2.xVal2";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 0);
    }

    bool testCase_accessNonStruct() {
      mMember = "Val1.Val1";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 0);
    }

    bool testCase_wrongOutputType() {
      mMember = "Val2.Val1";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 0);
    }

    CIEC_GET_STRUCT_VALUE_Struct_test2 mIn_struct;
    CIEC_INT mOut;
};

DEFINE_FB_TESTER(GET_STRUCT_VALUE_tester_MAIN, g_nStringIdGET_STRUCT_VALUE);

class GET_STRUCT_VALUE_tester_WRONG_INPUT_TYPE : public GET_STRUCT_VALUE_tester_generic {
  DECLARE_FB_TESTER(GET_STRUCT_VALUE_tester_WRONG_INPUT_TYPE)
  private:
    explicit GET_STRUCT_VALUE_tester_WRONG_INPUT_TYPE(CResource* mTestResource) :
        GET_STRUCT_VALUE_tester_generic(mTestResource, &mIn_struct, &mOut) {
    }

    virtual void executeAllTests() {
      mIn_struct = 1;
      evaluateTestResult(testCase_wrongInputType(), "Wrong input type");
    }

    /***********************************************************************************/
    bool testCase_wrongInputType() {
      mMember = "Val1";
      triggerEvent(0);
      return (checkForSingleOutputEventOccurence(0) && mQO == 0);
    }

    CIEC_INT mIn_struct;
    CIEC_INT mOut;
};

DEFINE_FB_TESTER(GET_STRUCT_VALUE_tester_WRONG_INPUT_TYPE, g_nStringIdGET_STRUCT_VALUE);
