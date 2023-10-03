/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss, nxtControl and Profactor
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Michael Hofmann, Stanislav Meduna - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <algorithm>

#include "forte_boost_output_support.h"

#include "fbdkasn1layer_serdeserdata.h"

#include "../../../src/core/cominfra/fbdkasn1layer.h"
#include "../../../src/core/cominfra/commfb.h"
#include "../../../src/stdfblib/ita/EMB_RES.h"

#include <boost/test/tools/floating_point_comparison.hpp>

#include "../../../src/core/datatypes/forte_real.h"

//BOOLEAN
#include "../../../src/core/datatypes/forte_bool.h"
//BIT-Datatypes
#include "../../../src/core/datatypes/forte_byte.h"
#include "../../../src/core/datatypes/forte_word.h"
#include "../../../src/core/datatypes/forte_dword.h"
//INT-Datatypes
#include "../../../src/core/datatypes/forte_sint.h"
#include "../../../src/core/datatypes/forte_usint.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_uint.h"
#include "../../../src/core/datatypes/forte_dint.h"
#include "../../../src/core/datatypes/forte_udint.h"
//STRING-Datatypes
#include "../../../src/core/datatypes/forte_string.h"
#include "../../../src/core/datatypes/forte_wstring.h"

#include "../../../src/core/datatypes/forte_time.h"

#include "../../../src/core/datatypes/forte_array.h"

#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"

#include "../../../src/core/datatypes/forte_lreal.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbdkasn1layerdeser_test_gen.cpp"
#endif

using namespace std::string_literals;

class CDeserTestMockCommFB : public forte::com_infra::CCommFB{
  public:
    CDeserTestMockCommFB(TForteUInt8 paNumRD, const CStringDictionary::TStringId * const paDODataTypeNames) :
        forte::com_infra::CCommFB(CStringDictionary::scmInvalidStringId, &mResource, forte::com_infra::e_Publisher),
        mMockFBInterface(){

      mMockFBInterface.mEINames = nullptr;
      mMockFBInterface.mEIWith = nullptr;
      mMockFBInterface.mEIWithIndexes = nullptr;
      mMockFBInterface.mNumEOs = 0;
      mMockFBInterface.mEONames = nullptr;
      mMockFBInterface.mEOWith = nullptr;
      mMockFBInterface.mEOWithIndexes = nullptr;
      mMockFBInterface.mNumDIs = 2;
      mMockFBInterface.mDINames = nullptr;
      mMockFBInterface.mDIDataTypeNames = scmInputTypes;
      mMockFBInterface.mNumDOs = paNumRD + 2U;
      mMockFBInterface.mDONames = nullptr;
      mMockFBInterface.mDODataTypeNames = paDODataTypeNames;

      setupFBInterface(&mMockFBInterface);
    }

    virtual ~CDeserTestMockCommFB(){
      freeAllData();
      mInterfaceSpec = nullptr;
    }

  private:
    static const CStringDictionary::TStringId scmInputTypes[];
    static EMB_RES mResource;

    SFBInterfaceSpec mMockFBInterface;
};

const CStringDictionary::TStringId CDeserTestMockCommFB::scmInputTypes[] = { g_nStringIdBOOL, g_nStringIdBOOL };
EMB_RES CDeserTestMockCommFB::mResource(CStringDictionary::scmInvalidStringId, nullptr);

BOOST_AUTO_TEST_SUITE(fbdkasn1layer_deserialize_test)

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_BOOL){

    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_BOOL &nVal(*static_cast<CIEC_BOOL *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(&cgBoolFalse, cgBoolSerSize));
    BOOST_CHECK_EQUAL(false, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(&cgBoolTrue, cgBoolSerSize));
    BOOST_CHECK_EQUAL(true, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BOOL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_BYTE){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBYTE };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_BYTE &nVal(*static_cast<CIEC_BYTE *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgByte0, cgByteSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgByte12, cgByteSerSize));
    BOOST_CHECK_EQUAL(12, nVal);
    BOOST_CHECK_EQUAL(12, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgByte128, cgByteSerSize));
    BOOST_CHECK_EQUAL(128, nVal);
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgByte255, cgByteSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BYTE){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBYTE };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_WORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_WORD &nVal(*static_cast<CIEC_WORD *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWord0, cgWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWord255, cgWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWord256, cgWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWord65535, cgWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWord40396, cgWordSerSize));
    BOOST_CHECK_EQUAL(40396, nVal);
    BOOST_CHECK_EQUAL(40396, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_WORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_DWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_DWORD &nVal(*static_cast<CIEC_DWORD *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord0, cgDWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord255, cgDWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord256, cgDWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord65535, cgDWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord65536, cgDWordSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);
    BOOST_CHECK_EQUAL(65536, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord4294967295, cgDWordSerSize));
    BOOST_CHECK_EQUAL(4294967295UL, nVal);
    BOOST_CHECK_EQUAL(4294967295UL, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDWord690586453, cgDWordSerSize));
    BOOST_CHECK_EQUAL(690586453, nVal);
    BOOST_CHECK_EQUAL(690586453, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_DWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_LWORD &nVal(*static_cast<CIEC_LWORD *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord0, cgLWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord255, cgLWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord256, cgLWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord65535, cgLWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord65536, cgLWordSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord4294967295, cgLWordSerSize));
    BOOST_CHECK_EQUAL(4294967295ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord4294967296, cgLWordSerSize));
    BOOST_CHECK_EQUAL(4294967296ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord18446744073709551615, cgLWordSerSize));
    BOOST_CHECK_EQUAL(18446744073709551615ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLWord3544923647067170873, cgLWordSerSize));
    BOOST_CHECK_EQUAL(3544923647067170873ULL, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_USINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_USINT &nVal(*static_cast<CIEC_USINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUSInt0, cgUSIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_USINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUSInt12, cgUSIntSerSize));
    BOOST_CHECK_EQUAL(12, static_cast<CIEC_USINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(12, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUSInt128, cgUSIntSerSize));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_USINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUSInt255, cgUSIntSerSize));
    BOOST_CHECK_EQUAL(255, static_cast<CIEC_USINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_USINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_UINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_UINT &nVal(*static_cast<CIEC_UINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUInt0, cgUIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_UINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUInt255, cgUIntSerSize));
    BOOST_CHECK_EQUAL(255, static_cast<CIEC_UINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUInt256, cgUIntSerSize));
    BOOST_CHECK_EQUAL(256, static_cast<CIEC_UINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUInt65535, cgUIntSerSize));
    BOOST_CHECK_EQUAL(65535, static_cast<CIEC_UINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUInt40396, cgUIntSerSize));
    BOOST_CHECK_EQUAL(40396, static_cast<CIEC_UINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(40396, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_UINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_UDINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_UDINT &nVal(*static_cast<CIEC_UDINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt0, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt255, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(255, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt256, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(256, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt65535, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(65535, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt65536, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(65536, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(65536, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt4294967295, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(4294967295UL, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(4294967295UL, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgUDInt690586453, cgUDIntSerSize));
    BOOST_CHECK_EQUAL(690586453, static_cast<CIEC_UDINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(690586453, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_UDINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_ULINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdULINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_ULINT &nVal(*static_cast<CIEC_ULINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt0, cgULIntSerSize));
    BOOST_CHECK_EQUAL(0U, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt255, cgULIntSerSize));
    BOOST_CHECK_EQUAL(255U, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt256, cgULIntSerSize));
    BOOST_CHECK_EQUAL(256U, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt65535, cgULIntSerSize));
    BOOST_CHECK_EQUAL(65535U, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt65536, cgULIntSerSize));
    BOOST_CHECK_EQUAL(65536U, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt4294967295, cgULIntSerSize));
    BOOST_CHECK_EQUAL(4294967295ULL, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt4294967296, cgULIntSerSize));
    BOOST_CHECK_EQUAL(4294967296ULL, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt18446744073709551615, cgULIntSerSize));
    BOOST_CHECK_EQUAL(18446744073709551615ULL, static_cast<CIEC_ULINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize));
    BOOST_CHECK_EQUAL(3544923647067170873ULL, static_cast<CIEC_ULINT::TValueType>(nVal));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_ULINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdULINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_SINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_SINT &nVal(*static_cast<CIEC_SINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgSInt0, cgSIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_SINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgSIntm128, cgSIntSerSize));
    BOOST_CHECK_EQUAL(-128, static_cast<CIEC_SINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgSInt127, cgSIntSerSize));
    BOOST_CHECK_EQUAL(127, static_cast<CIEC_SINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgSIntm90, cgSIntSerSize));
    BOOST_CHECK_EQUAL(-90, static_cast<CIEC_SINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-90, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgSInt90, cgSIntSerSize));
    BOOST_CHECK_EQUAL(90, static_cast<CIEC_SINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(90, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_SINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_INT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_INT &nVal(*static_cast<CIEC_INT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgInt0, cgIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgIntm128, cgIntSerSize));
    BOOST_CHECK_EQUAL(-128, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgIntm129, cgIntSerSize));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-129, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgInt127, cgIntSerSize));
    BOOST_CHECK_EQUAL(127, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgInt128, cgIntSerSize));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgIntm32768, cgIntSerSize));
    BOOST_CHECK_EQUAL(-32768, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgInt32767, cgIntSerSize));
    BOOST_CHECK_EQUAL(32767, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(32767, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgIntm10934, cgIntSerSize));
    BOOST_CHECK_EQUAL(-10934, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-10934, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgInt10934, cgIntSerSize));
    BOOST_CHECK_EQUAL(10934, static_cast<CIEC_INT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(10934, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_INT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_DINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_DINT &nVal(*static_cast<CIEC_DINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt0, cgDIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm128, cgDIntSerSize));
    BOOST_CHECK_EQUAL(static_cast<TForteInt32>(-128), nVal.operator TForteInt32());

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm129, cgDIntSerSize));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-129, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt127, cgDIntSerSize));
    BOOST_CHECK_EQUAL(127, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt128, cgDIntSerSize));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm32768, cgDIntSerSize));
    BOOST_CHECK_EQUAL(-32768, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm32769, cgDIntSerSize));
    BOOST_CHECK_EQUAL(-32769, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-32769, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt32767, cgDIntSerSize));
    BOOST_CHECK_EQUAL(32767, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(32767, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt32768, cgDIntSerSize));
    BOOST_CHECK_EQUAL(32768, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm2147483648, cgDIntSerSize));
    BOOST_CHECK_EQUAL(-2147483648L, static_cast<CIEC_DINT::TValueType>(nVal));

    //the test didn't work correctly when not using this helper var
    TForteInt32 buf = -2147483648L;
    BOOST_CHECK_EQUAL(buf, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt2147483647, cgDIntSerSize));
    BOOST_CHECK_EQUAL(2147483647L, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(2147483647L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDIntm800058586, cgDIntSerSize));
    BOOST_CHECK_EQUAL(-800058586L, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(-800058586L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgDInt800058586, cgDIntSerSize));
    BOOST_CHECK_EQUAL(800058586L, static_cast<CIEC_DINT::TValueType>(nVal));
    BOOST_CHECK_EQUAL(800058586L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_DINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_LINT &nVal(*static_cast<CIEC_LINT *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt0, cgLIntSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm128, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-128, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm129, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-129, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt127, cgLIntSerSize));
    BOOST_CHECK_EQUAL(127, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt128, cgLIntSerSize));
    BOOST_CHECK_EQUAL(128, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm32768, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-32768, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm32769, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-32769, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt32767, cgLIntSerSize));
    BOOST_CHECK_EQUAL(32767, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt32768, cgLIntSerSize));
    BOOST_CHECK_EQUAL(32768, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm2147483648, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-2147483648LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm2147483649, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-2147483649LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt2147483647, cgLIntSerSize));
    BOOST_CHECK_EQUAL(2147483647LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt2147483648, cgLIntSerSize));
    BOOST_CHECK_EQUAL(2147483648LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm9223372036854775808, cgLIntSerSize));
#ifndef __INT64_C
    BOOST_CHECK_EQUAL((-9223372036854775807LL - 1), static_cast<CIEC_LINT::TValueType>(nVal));
#else
    BOOST_CHECK_EQUAL(__INT64_C(-9223372036854775807LL - 1), static_cast<CIEC_LINT::TValueType>(nVal));
#endif

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt9223372036854775807, cgLIntSerSize));
    BOOST_CHECK_EQUAL(9223372036854775807LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLIntm800058586, cgLIntSerSize));
    BOOST_CHECK_EQUAL(-800058586LL, static_cast<CIEC_LINT::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLInt800058586, cgLIntSerSize));
    BOOST_CHECK_EQUAL(800058586LL, static_cast<CIEC_LINT::TValueType>(nVal));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_REAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_REAL &nVal(*static_cast<CIEC_REAL *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgReal0, cgRealSerSize));
    BOOST_CHECK_EQUAL(0.0f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgReal2_2874e6, cgRealSerSize));
    BOOST_CHECK_EQUAL(2.2874e6f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgRealm6_2587em4, cgRealSerSize));
    BOOST_CHECK_EQUAL(-6.2587e-4f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgReal1_0Em37, cgRealSerSize));
    BOOST_CHECK_EQUAL(1.0E-37f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgReal36_0, cgRealSerSize));
    BOOST_CHECK_EQUAL(36.0f, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_REAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LREAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_LREAL &nVal(*static_cast<CIEC_LREAL *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLReal0, cgLRealSerSize));
    BOOST_CHECK_EQUAL(0.0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLReal2_28743e6, cgLRealSerSize));
    BOOST_CHECK_EQUAL(2.28743e6, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize));
    BOOST_CHECK_EQUAL(-6.2587e-4, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLReal1_0Em37, cgLRealSerSize));
    BOOST_CHECK_EQUAL(1.0E-37, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgLReal36_0, cgLRealSerSize));
    BOOST_CHECK_EQUAL(36.0, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LREAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_STRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_STRING &nVal(*static_cast<CIEC_STRING *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgStringEmpty, cgStringEmptySerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 10);
    BOOST_TEST(nVal.getStorage() == "HalloWorld");
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_STRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_WSTRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_WSTRING &nVal(*static_cast<CIEC_WSTRING *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWStringEmpty, cgWStringEmptySerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 10);
    BOOST_TEST(nVal.getValue() == "HalloWorld");

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgWStringNihongo, cgWStringNihongoSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 9);
    BOOST_TEST(nVal.getValue() == reinterpret_cast<const char *>(cgWStringNihongoUTF8));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_WSTRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_TIME){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_TIME &nVal(*static_cast<CIEC_TIME *>(nTestFB.getRDs()[0]));
    char acStrBuf[9];

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgTime0, cgTimeSerSize));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_TIME::TValueType>(nVal));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgTime3000ms, cgTimeSerSize));
    nVal.toString(acStrBuf, 9);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3s"), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgTime3s22ms, cgTimeSerSize));
    nVal.toString(acStrBuf, 9);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3s22ms"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_TIME){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_MultiDatas){

    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdWORD, g_nStringIdSTRING, g_nStringIdINT, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(6, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    CIEC_TIME &oTimeVal1(*static_cast<CIEC_TIME *>(nTestFB.getRDs()[0]));
    CIEC_WORD &oWordVal(*static_cast<CIEC_WORD *>(nTestFB.getRDs()[1]));
    CIEC_STRING &oStringVal(*static_cast<CIEC_STRING *>(nTestFB.getRDs()[2]));
    CIEC_INT &oIntVal(*static_cast<CIEC_INT *>(nTestFB.getRDs()[3]));
    CIEC_BOOL &oBoolVal(*static_cast<CIEC_BOOL *>(nTestFB.getRDs()[4]));
    CIEC_TIME &oTimeVal2(*static_cast<CIEC_TIME *>(nTestFB.getRDs()[5]));

    unsigned int nSerSize = cgTimeSerSize + cgWordSerSize + cgStringHalloWorldSerSize + cgIntSerSize + cgBoolSerSize + cgTimeSerSize;
    TForteByte anGoodResult[] = { 0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0, 0x52, 0x9D, 0xCC, 0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd', 0x43, 0xD5, 0x4A, 0x41, 0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0 };

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(anGoodResult, nSerSize));
    char acStrBuf[9];
    oTimeVal1.toString(acStrBuf, 9);

    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3s"), 0);
    BOOST_CHECK_EQUAL(oWordVal, 40396);
    BOOST_TEST(oStringVal.getStorage() == "HalloWorld"s);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(oIntVal), -10934);
    BOOST_CHECK_EQUAL(oBoolVal, true);
    oTimeVal2.toString(acStrBuf, 9);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3s22ms"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_ARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 5, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);
    CIEC_ARRAY &nVal(*static_cast<CIEC_ARRAY *>(nTestFB.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgArrayBool10011, cgBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[4]), true);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[0]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[1]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nVal[4]), false);

    CStringDictionary::TStringId anType7[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 7, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB7(1, anType7);
    forte::com_infra::CFBDKASN1ComLayer nTestee7(nullptr, &nTestFB7);
    CIEC_ARRAY &nBoolArray7(*static_cast<CIEC_ARRAY *>(nTestFB7.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee7.recvData(cgArrayBool10011, cgBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[4]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[5]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray7[6]), false);

    CStringDictionary::TStringId anType3[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 3, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB3(1, anType3);
    forte::com_infra::CFBDKASN1ComLayer nTestee3(nullptr, &nTestFB3);
    CIEC_ARRAY &nBoolArray3(*static_cast<CIEC_ARRAY *>(nTestFB3.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee3.recvData(cgArrayBool10011, cgBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray3[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray3[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(nBoolArray3[2]), false);

    CStringDictionary::TStringId anType4[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 4, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB4(1, anType4);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt(nullptr, &nTestFB4);
    CIEC_ARRAY &nSIntArray(*static_cast<CIEC_ARRAY *>(nTestFB4.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt.recvData(cgArraySINTm128_127_0_m90, cgSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[0])), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[1])), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[2])), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[3])), -90);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[0])), -90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[1])), 90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[2])), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray[3])), 0);

    CStringDictionary::TStringId anTypeSIntArray7[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 7, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFBSIntArray7(1, anTypeSIntArray7);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt7(nullptr, &nTestFBSIntArray7);
    CIEC_ARRAY &nSIntArray7(*static_cast<CIEC_ARRAY *>(nTestFBSIntArray7.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt7.recvData(cgArraySINTm128_127_0_m90, cgSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[0])), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[1])), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[2])), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[3])), -90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[4])), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[5])), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray7[6])), 0);

    CStringDictionary::TStringId anTypeSInt2[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 2, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFBSInt2(1, anTypeSInt2);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt2(nullptr, &nTestFBSInt2);
    CIEC_ARRAY &nSIntArray2(*static_cast<CIEC_ARRAY *>(nTestFBSInt2.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt2.recvData(cgArraySINTm128_127_0_m90, cgSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray2[0])), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(static_cast<CIEC_SINT &>(nSIntArray2[1])), 127);

    CStringDictionary::TStringId anTypeString[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 2, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFBString(1, anTypeString);
    forte::com_infra::CFBDKASN1ComLayer nTesteeString(nullptr, &nTestFBString);
    CIEC_ARRAY &nStringArray(*static_cast<CIEC_ARRAY *>(nTestFBString.getRDs()[0]));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeString.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>(nStringArray[0]).length(), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>(nStringArray[1]).length(), 10);
    BOOST_TEST(static_cast<CIEC_STRING &>(nStringArray[1]).getStorage() == "HalloWorld"s);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BOOLARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 5, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_SINTARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 4, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayStringEmptyHalloWorld, cgString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_STRINGARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 0, 2, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(nullptr, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cgBoolFalse, cgBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgByte128, cgByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWord40396, cgWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDWord690586453, cgDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLWord4294967296, cgLWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUSInt128, cgUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUInt40396, cgUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgUDInt690586453, cgUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgULInt3544923647067170873, cgULIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgSIntm90, cgSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgIntm10934, cgIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgDInt800058586, cgDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLIntm800058586, cgLIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgReal1_0Em37, cgRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgLRealm6_2587em4, cgLRealSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgStringHalloWorld, cgStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgWStringHalloWorld, cgWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgTime3s22ms, cgTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArrayBool01010, cgBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cgArraySINTm90_90_127_0, cgSINT4SerSize)));
  }

  BOOST_AUTO_TEST_SUITE_END()

