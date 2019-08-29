/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss, nxtControl and Profactor
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Michael Hofmann, Stanislav Meduna - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <algorithm>

#include "fbdkasn1layer_serdeserdata.h"

#include "../../../src/core/cominfra/fbdkasn1layer.h"
#include "../../../src/core/cominfra/commfb.h"
#include "../../../src/stdfblib/ita/EMB_RES.h"

#ifdef FORTE_USE_REAL_DATATYPE
#include <boost/test/floating_point_comparison.hpp>

#include "../../../src/core/datatypes/forte_real.h"
#endif

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

#ifdef FORTE_USE_64BIT_DATATYPES
#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
#include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_LREAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbdkasn1layerdeser_test_gen.cpp"
#endif

class CDeserTestMockCommFB : public forte::com_infra::CCommFB{
  public:
    CDeserTestMockCommFB(TForteUInt8 pa_nNumRD, const CStringDictionary::TStringId * const pa_aunDODataTypeNames) :
        forte::com_infra::CCommFB(CStringDictionary::scm_nInvalidStringId, &m_oResource, forte::com_infra::e_Publisher),
        m_oMockFBInterface(){

      m_oMockFBInterface.m_aunEINames = 0;
      m_oMockFBInterface.m_anEIWith = 0;
      m_oMockFBInterface.m_anEIWithIndexes = 0;
      m_oMockFBInterface.m_nNumEOs = 0;
      m_oMockFBInterface.m_aunEONames = 0;
      m_oMockFBInterface.m_anEOWith = 0;
      m_oMockFBInterface.m_anEOWithIndexes = 0;
      m_oMockFBInterface.m_nNumDIs = 2;
      m_oMockFBInterface.m_aunDINames = 0;
      m_oMockFBInterface.m_aunDIDataTypeNames = scm_anInputTypes;
      m_oMockFBInterface.m_nNumDOs = static_cast<TForteUInt8>(pa_nNumRD + 2U);
      m_oMockFBInterface.m_aunDONames = 0;
      m_oMockFBInterface.m_aunDODataTypeNames = pa_aunDODataTypeNames;

      m_acFBConnData = new TForteByte[genFBConnDataSize(0, 2, pa_nNumRD + 2)];
      m_acFBVarsData = new TForteByte[genFBVarsDataSize(2, pa_nNumRD + 2)];

      setupFBInterface(&m_oMockFBInterface, m_acFBConnData, m_acFBVarsData);
    }

    virtual ~CDeserTestMockCommFB(){
      freeAllData();
      delete[] m_acFBConnData;
      delete[] m_acFBVarsData;
      m_pstInterfaceSpec = 0;
    }

  private:
    static const CStringDictionary::TStringId scm_anInputTypes[];
    static EMB_RES m_oResource;

    SFBInterfaceSpec m_oMockFBInterface;
    TForteByte *m_acFBConnData;
    TForteByte *m_acFBVarsData;

};

const CStringDictionary::TStringId CDeserTestMockCommFB::scm_anInputTypes[] = { g_nStringIdBOOL, g_nStringIdBOOL };
EMB_RES CDeserTestMockCommFB::m_oResource(CStringDictionary::scm_nInvalidStringId, 0);

BOOST_AUTO_TEST_SUITE(fbdkasn1layer_deserialize_test)

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_BOOL){

    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_BOOL &nVal(*static_cast<CIEC_BOOL *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize));
    BOOST_CHECK_EQUAL(false, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(&cg_abBoolTrue, cg_unBoolSerSize));
    BOOST_CHECK_EQUAL(true, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BOOL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_BYTE){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBYTE };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_BYTE &nVal(*static_cast<CIEC_BYTE *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abByte0, cg_unByteSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abByte12, cg_unByteSerSize));
    BOOST_CHECK_EQUAL(12, nVal);
    BOOST_CHECK_EQUAL(12, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abByte128, cg_unByteSerSize));
    BOOST_CHECK_EQUAL(128, nVal);
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abByte255, cg_unByteSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BYTE){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBYTE };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_WORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_WORD &nVal(*static_cast<CIEC_WORD *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWord0, cg_unWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWord255, cg_unWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWord256, cg_unWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWord65535, cg_unWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWord40396, cg_unWordSerSize));
    BOOST_CHECK_EQUAL(40396, nVal);
    BOOST_CHECK_EQUAL(40396, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_WORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_DWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_DWORD &nVal(*static_cast<CIEC_DWORD *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord0, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord255, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord256, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord65535, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord65536, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);
    BOOST_CHECK_EQUAL(65536, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord4294967295, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(4294967295UL, nVal);
    BOOST_CHECK_EQUAL(4294967295UL, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize));
    BOOST_CHECK_EQUAL(690586453, nVal);
    BOOST_CHECK_EQUAL(690586453, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_DWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_LWORD &nVal(*static_cast<CIEC_LWORD *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord0, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord255, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(255, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord256, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(256, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord65535, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord65536, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord4294967295, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(4294967295ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(4294967296ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord18446744073709551615, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(18446744073709551615ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLWord3544923647067170873, cg_unLWordSerSize));
    BOOST_CHECK_EQUAL(3544923647067170873ULL, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LWORD){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLWORD };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }
#endif

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_USINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_USINT &nVal(*static_cast<CIEC_USINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUSInt0, cg_unUSIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUSInt12, cg_unUSIntSerSize));
    BOOST_CHECK_EQUAL(12, nVal);
    BOOST_CHECK_EQUAL(12, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize));
    BOOST_CHECK_EQUAL(128, nVal);
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUSInt255, cg_unUSIntSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_USINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_UINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_UINT &nVal(*static_cast<CIEC_UINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUInt0, cg_unUIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUInt255, cg_unUIntSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUInt256, cg_unUIntSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUInt65535, cg_unUIntSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize));
    BOOST_CHECK_EQUAL(40396, nVal);
    BOOST_CHECK_EQUAL(40396, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_UINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_UDINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_UDINT &nVal(*static_cast<CIEC_UDINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt0, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt255, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(255, nVal);
    BOOST_CHECK_EQUAL(255, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt256, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(256, nVal);
    BOOST_CHECK_EQUAL(256, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt65535, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);
    BOOST_CHECK_EQUAL(65535, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt65536, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);
    BOOST_CHECK_EQUAL(65536, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt4294967295, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(4294967295UL, nVal);
    BOOST_CHECK_EQUAL(4294967295UL, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize));
    BOOST_CHECK_EQUAL(690586453, nVal);
    BOOST_CHECK_EQUAL(690586453, *((CIEC_ANY::TLargestUIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_UDINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_ULINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdULINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_ULINT &nVal(*static_cast<CIEC_ULINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt0, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt255, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(255, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt256, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(256, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt65535, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(65535, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt65536, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(65536, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt4294967295, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(4294967295ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt4294967296, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(4294967296ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt18446744073709551615, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(18446744073709551615ULL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize));
    BOOST_CHECK_EQUAL(3544923647067170873ULL, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_ULINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdULINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }
#endif

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_SINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_SINT &nVal(*static_cast<CIEC_SINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abSInt0, cg_unSIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abSIntm128, cg_unSIntSerSize));
    BOOST_CHECK_EQUAL(-128, nVal);
    BOOST_CHECK_EQUAL(-128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abSInt127, cg_unSIntSerSize));
    BOOST_CHECK_EQUAL(127, nVal);
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize));
    BOOST_CHECK_EQUAL(-90, nVal);
    BOOST_CHECK_EQUAL(-90, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abSInt90, cg_unSIntSerSize));
    BOOST_CHECK_EQUAL(90, nVal);
    BOOST_CHECK_EQUAL(90, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_SINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_INT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_INT &nVal(*static_cast<CIEC_INT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abInt0, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abIntm128, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(-128, nVal);
    BOOST_CHECK_EQUAL(-128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abIntm129, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(-129, nVal);
    BOOST_CHECK_EQUAL(-129, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abInt127, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(127, nVal);
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abInt128, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(128, nVal);
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abIntm32768, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(-32768, nVal);
    BOOST_CHECK_EQUAL(-32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abInt32767, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(32767, nVal);
    BOOST_CHECK_EQUAL(32767, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abIntm10934, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(-10934, nVal);
    BOOST_CHECK_EQUAL(-10934, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abInt10934, cg_unIntSerSize));
    BOOST_CHECK_EQUAL(10934, nVal);
    BOOST_CHECK_EQUAL(10934, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_INT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_DINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_DINT &nVal(*static_cast<CIEC_DINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt0, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);
    BOOST_CHECK_EQUAL(0, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm128, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(static_cast<TForteInt32>(-128), nVal.operator TForteInt32());

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm129, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(-129, nVal);
    BOOST_CHECK_EQUAL(-129, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt127, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(127, nVal);
    BOOST_CHECK_EQUAL(127, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt128, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(128, nVal);
    BOOST_CHECK_EQUAL(128, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm32768, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(-32768, nVal);
    BOOST_CHECK_EQUAL(-32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm32769, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(-32769, nVal);
    BOOST_CHECK_EQUAL(-32769, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt32767, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(32767, nVal);
    BOOST_CHECK_EQUAL(32767, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt32768, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(32768, nVal);
    BOOST_CHECK_EQUAL(32768, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm2147483648, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(-2147483648L, nVal);

    //the test didn't work correctly when not using this helper var
    TForteInt32 buf = -2147483648L;
    BOOST_CHECK_EQUAL(buf, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt2147483647, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(2147483647L, nVal);
    BOOST_CHECK_EQUAL(2147483647L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDIntm800058586, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(-800058586L, nVal);
    BOOST_CHECK_EQUAL(-800058586L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize));
    BOOST_CHECK_EQUAL(800058586L, nVal);
    BOOST_CHECK_EQUAL(800058586L, *((CIEC_ANY::TLargestIntValueType * ) nVal.getDataPtr()));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_DINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_LINT &nVal(*static_cast<CIEC_LINT *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt0, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm128, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-128, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm129, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-129, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt127, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(127, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt128, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(128, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm32768, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-32768, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm32769, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-32769, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt32767, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(32767, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt32768, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(32768, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm2147483648, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-2147483648LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm2147483649, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-2147483649LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt2147483647, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(2147483647LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt2147483648, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(2147483648LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm9223372036854775808, cg_unLIntSerSize));
#ifndef __INT64_C
    BOOST_CHECK_EQUAL((-9223372036854775807LL - 1), nVal);
#else
    BOOST_CHECK_EQUAL(__INT64_C(-9223372036854775807LL - 1), nVal);
#endif

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt9223372036854775807, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(9223372036854775807LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(-800058586LL, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLInt800058586, cg_unLIntSerSize));
    BOOST_CHECK_EQUAL(800058586LL, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LINT){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

#endif

#ifdef FORTE_USE_REAL_DATATYPE
  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_REAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_REAL &nVal(*static_cast<CIEC_REAL *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abReal0, cg_unRealSerSize));
    BOOST_CHECK_EQUAL(0.0f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abReal2_2874e6, cg_unRealSerSize));
    BOOST_CHECK_EQUAL(2.2874e6f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abRealm6_2587em4, cg_unRealSerSize));
    BOOST_CHECK_EQUAL(-6.2587e-4f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize));
    BOOST_CHECK_EQUAL(1.0E-37f, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abReal36_0, cg_unRealSerSize));
    BOOST_CHECK_EQUAL(36.0f, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_REAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

#ifdef FORTE_USE_64BIT_DATATYPES

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_LREAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_LREAL &nVal(*static_cast<CIEC_LREAL *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLReal0, cg_unLRealSerSize));
    BOOST_CHECK_EQUAL(0.0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLReal2_28743e6, cg_unLRealSerSize));
    BOOST_CHECK_EQUAL(2.28743e6, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize));
    BOOST_CHECK_EQUAL(-6.2587e-4, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLReal1_0Em37, cg_unLRealSerSize));
    BOOST_CHECK_EQUAL(1.0E-37, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abLReal36_0, cg_unLRealSerSize));
    BOOST_CHECK_EQUAL(36.0, nVal);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_LREAL){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLREAL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }
#endif
#endif

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_STRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_STRING &nVal(*static_cast<CIEC_STRING *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abStringEmpty, cg_unStringEmptySerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 10);
    BOOST_CHECK_EQUAL(strcmp(nVal.getValue(), "HalloWorld"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_STRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_WSTRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_WSTRING &nVal(*static_cast<CIEC_WSTRING *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWStringEmpty, cg_unWStringEmptySerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 10);
    BOOST_CHECK_EQUAL(strcmp(nVal.getValue(), "HalloWorld"), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abWStringNihongo, cg_unWStringNihongoSerSize));
    BOOST_CHECK_EQUAL(nVal.length(), 9);
    BOOST_CHECK_EQUAL(strcmp(nVal.getValue(), (const char * ) cg_abWStringNihongoUTF8), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_WSTRING){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdWSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_TIME){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_TIME &nVal(*static_cast<CIEC_TIME *>(nTestFB.getRDs()));
    char acStrBuf[20];

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abTime0, cg_unTimeSerSize));
    BOOST_CHECK_EQUAL(0, nVal);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abTime3000ms, cg_unTimeSerSize));
    nVal.toString(acStrBuf, 20);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3000ms"), 0);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize));
    nVal.toString(acStrBuf, 20);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3022ms"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_TIME){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_MultiDatas){

    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdWORD, g_nStringIdSTRING, g_nStringIdINT, g_nStringIdBOOL, g_nStringIdTIME };
    CDeserTestMockCommFB nTestFB(6, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    CIEC_TIME &oTimeVal1(*static_cast<CIEC_TIME *>(nTestFB.getRDs()));
    CIEC_WORD &oWordVal(*static_cast<CIEC_WORD *>(nTestFB.getRDs() + 1));
    CIEC_STRING &oStringVal(*static_cast<CIEC_STRING *>(nTestFB.getRDs() + 2));
    CIEC_INT &oIntVal(*static_cast<CIEC_INT *>(nTestFB.getRDs() + 3));
    CIEC_BOOL &oBoolVal(*static_cast<CIEC_BOOL *>(nTestFB.getRDs() + 4));
    CIEC_TIME &oTimeVal2(*static_cast<CIEC_TIME *>(nTestFB.getRDs() + 5));

    unsigned int nSerSize = cg_unTimeSerSize + cg_unWordSerSize + cg_unStringHalloWorldSerSize + cg_unIntSerSize + cg_unBoolSerSize + cg_unTimeSerSize;
    TForteByte anGoodResult[] = { 0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0, 0x52, 0x9D, 0xCC, 0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd', 0x43, 0xD5, 0x4A, 0x41, 0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0 };

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(anGoodResult, nSerSize));
    char acStrBuf[20];
    oTimeVal1.toString(acStrBuf, 20);

    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3000ms"), 0);
    BOOST_CHECK_EQUAL(oWordVal, 40396);
    BOOST_CHECK_EQUAL(strcmp(oStringVal.getValue(), "HalloWorld"), 0);
    BOOST_CHECK_EQUAL(oIntVal, -10934);
    BOOST_CHECK_EQUAL(oBoolVal, true);
    oTimeVal2.toString(acStrBuf, 20);
    BOOST_CHECK_EQUAL(strcmp(acStrBuf, "T#3022ms"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Test_ARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 5, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);
    CIEC_ARRAY &nVal(*static_cast<CIEC_ARRAY *>(nTestFB.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abArrayBool10011, cg_unBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[4]), true);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[0]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[1]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nVal[4]), false);

    CStringDictionary::TStringId anType7[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 7, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB7(1, anType7);
    forte::com_infra::CFBDKASN1ComLayer nTestee7(0, &nTestFB7);
    CIEC_ARRAY &nBoolArray7(*static_cast<CIEC_ARRAY *>(nTestFB7.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee7.recvData(cg_abArrayBool10011, cg_unBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[2]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[3]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[4]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[5]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray7[6]), false);

    CStringDictionary::TStringId anType3[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 3, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB3(1, anType3);
    forte::com_infra::CFBDKASN1ComLayer nTestee3(0, &nTestFB3);
    CIEC_ARRAY &nBoolArray3(*static_cast<CIEC_ARRAY *>(nTestFB3.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee3.recvData(cg_abArrayBool10011, cg_unBOOL5SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray3[0]), true);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray3[1]), false);
    BOOST_CHECK_EQUAL(static_cast<CIEC_BOOL &>(*nBoolArray3[2]), false);

    CStringDictionary::TStringId anType4[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 4, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB4(1, anType4);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt(0, &nTestFB4);
    CIEC_ARRAY &nSIntArray(*static_cast<CIEC_ARRAY *>(nTestFB4.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt.recvData(cg_abArraySINTm128_127_0_m90, cg_unSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[0]), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[1]), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[2]), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[3]), -90);

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[0]), -90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[1]), 90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[2]), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray[3]), 0);

    CStringDictionary::TStringId anTypeSIntArray7[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 7, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFBSIntArray7(1, anTypeSIntArray7);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt7(0, &nTestFBSIntArray7);
    CIEC_ARRAY &nSIntArray7(*static_cast<CIEC_ARRAY *>(nTestFBSIntArray7.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt7.recvData(cg_abArraySINTm128_127_0_m90, cg_unSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[0]), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[1]), 127);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[2]), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[3]), -90);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[4]), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[5]), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray7[6]), 0);

    CStringDictionary::TStringId anTypeSInt2[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 2, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFBSInt2(1, anTypeSInt2);
    forte::com_infra::CFBDKASN1ComLayer nTesteeSInt2(0, &nTestFBSInt2);
    CIEC_ARRAY &nSIntArray2(*static_cast<CIEC_ARRAY *>(nTestFBSInt2.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeSInt2.recvData(cg_abArraySINTm128_127_0_m90, cg_unSINT4SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray2[0]), -128);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT &>(*nSIntArray2[1]), 127);

    CStringDictionary::TStringId anTypeString[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 2, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFBString(1, anTypeString);
    forte::com_infra::CFBDKASN1ComLayer nTesteeString(0, &nTestFBString);
    CIEC_ARRAY &nStringArray(*static_cast<CIEC_ARRAY *>(nTestFBString.getRDs()));

    BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTesteeString.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize));
    BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>(*nStringArray[0]).length(), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_STRING &>(*nStringArray[1]).length(), 10);
    BOOST_CHECK_EQUAL(strcmp(static_cast<CIEC_STRING &>(*nStringArray[1]).getValue(), "HalloWorld"), 0);
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_BOOLARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 5, g_nStringIdBOOL };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_SINTARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 4, g_nStringIdSINT };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayStringEmptyHalloWorld, cg_unString2SerSize)));
  }

  BOOST_AUTO_TEST_CASE(Single_Deserialize_Negative_Test_STRINGARRAY){
    CStringDictionary::TStringId anType[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdARRAY, 2, g_nStringIdSTRING };
    CDeserTestMockCommFB nTestFB(1, anType);
    forte::com_infra::CFBDKASN1ComLayer nTestee(0, &nTestFB);

    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(&cg_abBoolFalse, cg_unBoolSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abByte128, cg_unByteSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWord40396, cg_unWordSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDWord690586453, cg_unDWordSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLWord4294967296, cg_unLWordSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUSInt128, cg_unUSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUInt40396, cg_unUIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abUDInt690586453, cg_unUDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abULInt3544923647067170873, cg_unULIntSerSize)));
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abSIntm90, cg_unSIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abIntm10934, cg_unIntSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abDInt800058586, cg_unDIntSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLIntm800058586, cg_unLIntSerSize)));
#endif
#ifdef FORTE_USE_REAL_DATATYPE
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abReal1_0Em37, cg_unRealSerSize)));
#ifdef FORTE_USE_64BIT_DATATYPES
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abLRealm6_2587em4, cg_unLRealSerSize)));
#endif
#endif
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abStringHalloWorld, cg_unStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abWStringHalloWorld, cg_unWStringHalloWorldSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abTime3s22ms, cg_unTimeSerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArrayBool01010, cg_unBOOL5SerSize)));
    BOOST_CHECK((forte::com_infra::e_ProcessDataOk != nTestee.recvData(cg_abArraySINTm90_90_127_0, cg_unSINT4SerSize)));
  }

  BOOST_AUTO_TEST_SUITE_END()

