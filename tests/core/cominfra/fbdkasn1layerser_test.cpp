/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss, nxtControl and Profactor
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Michael Hofmann, Stanislav Meduna - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <algorithm>

#include "fbdkasn1layer_serdeserdata.h"

#include "../../../src/core/cominfra/fbdkasn1layer.h"

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
#include "fbdkasn1layerser_test_gen.cpp"
#endif


class CFBDKASN1ComLayerTestMock: public forte::com_infra::CFBDKASN1ComLayer{
  public:
    CFBDKASN1ComLayerTestMock() : forte::com_infra::CFBDKASN1ComLayer(0, 0){
      m_poBottomLayer = &m_oTestLayer;
    }

    ~CFBDKASN1ComLayerTestMock(){
      m_poBottomLayer = 0;
    }

    void *getSendDataPtr() {
      return m_oTestLayer.m_poData;
    }
    unsigned int getSendDataSize(){
      return m_oTestLayer.m_unSize;
    }

  private:
   class TestMockBottomLayer : public forte::com_infra::CComLayer{
     public:
     TestMockBottomLayer() : forte::com_infra::CComLayer(0,0){
       m_poData = 0;
       m_poAllocData = 0;
       m_unSize = 0;
     }

     ~TestMockBottomLayer(){
       if (0 != m_poAllocData) {
         delete[] m_poAllocData;
       }
     }

      forte::com_infra::EComResponse sendData(void *pa_pvData, unsigned int pa_unSize){
        if (0 != m_poAllocData) {
          delete[] m_poAllocData;
        }
        m_poAllocData = new TForteByte[pa_unSize];
        if (0 != m_poAllocData) {
          m_poData = m_poAllocData;
          memcpy(m_poData,pa_pvData,pa_unSize);
          //m_poData = pa_pvData;
          m_unSize = pa_unSize;
          return forte::com_infra::e_ProcessDataOk;
        } else {
          return forte::com_infra::e_ProcessDataSendFailed;
        }
       }

      virtual void closeConnection() {}
      virtual forte::com_infra::EComResponse recvData(const void *, unsigned int ) {
        return forte::com_infra::e_ProcessDataOk;
      }
      virtual forte::com_infra::EComResponse openConnection(char *){
        return forte::com_infra::e_ProcessDataOk;
      }

      TForteByte *m_poData;
      TForteByte *m_poAllocData;
       unsigned int m_unSize;
   };

   TestMockBottomLayer m_oTestLayer;
};



BOOST_AUTO_TEST_SUITE(fbdkasn1layer_serialize_test)

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_BOOL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_BOOL nBool;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nBool;

  nBool= false;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nBool, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unBoolSerSize);
  BOOST_CHECK_EQUAL(cg_abBoolFalse, *((TForteByte *)nTestee.getSendDataPtr()));

  nBool = true;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nBool, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unBoolSerSize);
  BOOST_CHECK_EQUAL(cg_abBoolTrue, *((TForteByte *)nTestee.getSendDataPtr()));

  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(static_cast<TForteByte *>(0), 0, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(static_cast<TForteByte *>(0), 0, nBool), -1);
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_BYTE){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_BYTE nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unByteSerSize);
  BOOST_CHECK(std::equal(cg_abByte0, cg_abByte0 + cg_unByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 12;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unByteSerSize);
  BOOST_CHECK(std::equal(cg_abByte12, cg_abByte12 + cg_unByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unByteSerSize);
  BOOST_CHECK(std::equal(cg_abByte128, cg_abByte128 + cg_unByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unByteSerSize);
  BOOST_CHECK(std::equal(cg_abByte255, cg_abByte255 + cg_unByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_WORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_WORD nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWordSerSize);
  BOOST_CHECK(std::equal(cg_abWord0, cg_abWord0 + cg_unWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWordSerSize);
  BOOST_CHECK(std::equal(cg_abWord255, cg_abWord255 + cg_unWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWordSerSize);
  BOOST_CHECK(std::equal(cg_abWord256, cg_abWord256 + cg_unWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWordSerSize);
  BOOST_CHECK(std::equal(cg_abWord65535, cg_abWord65535 + cg_unWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 40396;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWordSerSize);
  BOOST_CHECK(std::equal(cg_abWord40396, cg_abWord40396 + cg_unWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_DWORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_DWORD nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord0, cg_abDWord0 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord255, cg_abDWord255 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord256, cg_abDWord256 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord65535, cg_abDWord65535 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65536;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord65536, cg_abDWord65536 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967295UL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord4294967295, cg_abDWord4294967295 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 690586453;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDWordSerSize);
  BOOST_CHECK(std::equal(cg_abDWord690586453, cg_abDWord690586453 + cg_unDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


#ifdef FORTE_USE_64BIT_DATATYPES
//LWORD
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LWORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LWORD nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord0, cg_abLWord0 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord255, cg_abLWord255 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord256, cg_abLWord256 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord65535, cg_abLWord65535 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65536;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord65536, cg_abLWord65536 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967295LL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord4294967295, cg_abLWord4294967295 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967296LL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord4294967296, cg_abLWord4294967296 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 18446744073709551615ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord18446744073709551615, cg_abLWord18446744073709551615 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 18446744073709551615ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLWordSerSize);
  BOOST_CHECK(std::equal(cg_abLWord18446744073709551615, cg_abLWord18446744073709551615 + cg_unLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}
#endif

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_USINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_USINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUSIntSerSize);
  BOOST_CHECK(std::equal(cg_abUSInt0, cg_abUSInt0 + cg_unUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 12;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUSIntSerSize);
  BOOST_CHECK(std::equal(cg_abUSInt12, cg_abUSInt12 + cg_unUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUSIntSerSize);
  BOOST_CHECK(std::equal(cg_abUSInt128, cg_abUSInt128 + cg_unUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUSIntSerSize);
  BOOST_CHECK(std::equal(cg_abUSInt255, cg_abUSInt255 + cg_unUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_UINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_UINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUIntSerSize);
  BOOST_CHECK(std::equal(cg_abUInt0, cg_abUInt0 + cg_unUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUIntSerSize);
  BOOST_CHECK(std::equal(cg_abUInt255, cg_abUInt255 + cg_unUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUIntSerSize);
  BOOST_CHECK(std::equal(cg_abUInt256, cg_abUInt256 + cg_unUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUIntSerSize);
  BOOST_CHECK(std::equal(cg_abUInt65535, cg_abUInt65535 + cg_unUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 40396;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUIntSerSize);
  BOOST_CHECK(std::equal(cg_abUInt40396, cg_abUInt40396 + cg_unUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_UDINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_UDINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt0, cg_abUDInt0 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt255, cg_abUDInt255 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt256, cg_abUDInt256 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt65535, cg_abUDInt65535 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65536;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt65536, cg_abUDInt65536 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967295UL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt4294967295, cg_abUDInt4294967295 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 690586453;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unUDIntSerSize);
  BOOST_CHECK(std::equal(cg_abUDInt690586453, cg_abUDInt690586453 + cg_unUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


#ifdef FORTE_USE_64BIT_DATATYPES
//LWORD
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_ULINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_ULINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt0, cg_abULInt0 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 255;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt255, cg_abULInt255 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= 256;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt256, cg_abULInt256 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= 65535;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt65535, cg_abULInt65535 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 65536;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt65536, cg_abULInt65536 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967295ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt4294967295, cg_abULInt4294967295 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 4294967296ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt4294967296, cg_abULInt4294967296 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 18446744073709551615ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt18446744073709551615, cg_abULInt18446744073709551615 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= 18446744073709551615ULL;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unULIntSerSize);
  BOOST_CHECK(std::equal(cg_abULInt18446744073709551615, cg_abULInt18446744073709551615 + cg_unULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}
#endif


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_SINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_SINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSIntSerSize);
  BOOST_CHECK(std::equal(cg_abSInt0, cg_abSInt0 + cg_unSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSIntSerSize);
  BOOST_CHECK(std::equal(cg_abSIntm128, cg_abSIntm128 + cg_unSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 127;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSIntSerSize);
  BOOST_CHECK(std::equal(cg_abSInt127, cg_abSInt127 + cg_unSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -90;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSIntSerSize);
  BOOST_CHECK(std::equal(cg_abSIntm90, cg_abSIntm90 + cg_unSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 90;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSIntSerSize);
  BOOST_CHECK(std::equal(cg_abSInt90, cg_abSInt90 + cg_unSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_INT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_INT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abInt0, cg_abInt0 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abIntm128, cg_abIntm128 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -129;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abIntm129, cg_abIntm129 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 127;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abInt127, cg_abInt127 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abInt128, cg_abInt128 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -32768;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abIntm32768, cg_abIntm32768 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 32767;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abInt32767, cg_abInt32767 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -10934;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abIntm10934, cg_abIntm10934 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 10934;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unIntSerSize);
  BOOST_CHECK(std::equal(cg_abInt10934, cg_abInt10934 + cg_unIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_DINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_DINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= 0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt0, cg_abDInt0 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm128, cg_abDIntm128 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -129;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm129, cg_abDIntm129 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 127;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt127, cg_abDInt127 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 128;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt128, cg_abDInt128 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -32768;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm32768, cg_abDIntm32768 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -32769;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm32769, cg_abDIntm32769 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 32767;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt32767, cg_abDInt32767 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 32768;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt32768, cg_abDInt32768 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -2147483648L;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm2147483648, cg_abDIntm2147483648 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 2147483647L;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt2147483647, cg_abDInt2147483647 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -800058586;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDIntm800058586, cg_abDIntm800058586 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 800058586;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unDIntSerSize);
  BOOST_CHECK(std::equal(cg_abDInt800058586, cg_abDInt800058586 + cg_unDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

#ifdef FORTE_USE_64BIT_DATATYPES
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= 0;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt0, cg_abLInt0 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -128;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm128, cg_abLIntm128 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -129;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm129, cg_abLIntm129 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 127;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt127, cg_abLInt127 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 128;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt128, cg_abLInt128 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -32768;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm32768, cg_abLIntm32768 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -32769;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm32769, cg_abLIntm32769 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 32767;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt32767, cg_abLInt32767 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 32768;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt32768, cg_abLInt32768 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -2147483648LL;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm2147483648, cg_abLIntm2147483648 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -2147483649LL;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm2147483649, cg_abLIntm2147483649 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 2147483647;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt2147483647, cg_abLInt2147483647 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 2147483648LL;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt2147483648, cg_abLInt2147483648 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -9223372036854775807LL - 1LL;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm9223372036854775808, cg_abLIntm9223372036854775808 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 9223372036854775807LL;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt9223372036854775807, cg_abLInt9223372036854775807 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = -800058586;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLIntm800058586, cg_abLIntm800058586 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = 800058586;
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLIntSerSize);
   BOOST_CHECK(std::equal(cg_abLInt800058586, cg_abLInt800058586 + cg_unLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

#endif

#ifdef FORTE_USE_REAL_DATATYPE


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_REAL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_REAL nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unRealSerSize);
  BOOST_CHECK(std::equal(cg_abReal0, cg_abReal0 + cg_unRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 2.2874e6f;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unRealSerSize);
  BOOST_CHECK(std::equal(cg_abReal2_2874e6, cg_abReal2_2874e6 + cg_unRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -6.2587e-4f;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unRealSerSize);
  BOOST_CHECK(std::equal(cg_abRealm6_2587em4, cg_abRealm6_2587em4 + cg_unRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 1.0E-37f;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unRealSerSize);
  BOOST_CHECK(std::equal(cg_abReal1_0Em37, cg_abReal1_0Em37 + cg_unRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 36.0f;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unRealSerSize);
  BOOST_CHECK(std::equal(cg_abReal36_0, cg_abReal36_0 + cg_unRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

#ifdef FORTE_USE_64BIT_DATATYPES
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LREAL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LREAL nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLRealSerSize);
  BOOST_CHECK(std::equal(cg_abLReal0, cg_abLReal0 + cg_unLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 2.28743e6;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLRealSerSize);
  BOOST_CHECK(std::equal(cg_abLReal2_28743e6, cg_abLReal2_28743e6 + cg_unLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = -6.2587e-4;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLRealSerSize);
  BOOST_CHECK(std::equal(cg_abLRealm6_2587em4, cg_abLRealm6_2587em4 + cg_unLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 1.0E-37;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLRealSerSize);
  BOOST_CHECK(std::equal(cg_abLReal1_0Em37, cg_abLReal1_0Em37 + cg_unLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = 36.0;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unLRealSerSize);
  BOOST_CHECK(std::equal(cg_abLReal36_0, cg_abLReal36_0 + cg_unLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

#endif
#endif

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_STRING){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_STRING nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unStringEmptySerSize);
  BOOST_CHECK(std::equal(cg_abStringEmpty, cg_abStringEmpty + cg_unStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = "HalloWorld";
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unStringHalloWorldSerSize);
  BOOST_CHECK(std::equal(cg_abStringHalloWorld, cg_abStringHalloWorld + cg_unStringHalloWorldSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  TForteByte acSecondSmallBuf[12];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSecondSmallBuf, 12, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSecondSmallBuf, 12, nVal), -1);
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_WSTRING){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_WSTRING nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWStringEmptySerSize);
  BOOST_CHECK(std::equal(cg_abWStringEmpty, cg_abWStringEmpty + cg_unWStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = "HalloWorld";
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWStringHalloWorldSerSize);
  BOOST_CHECK(std::equal(cg_abWStringHalloWorld, cg_abWStringHalloWorld + cg_unWStringHalloWorldSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  TForteByte acSecondSmallBuf[3];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSecondSmallBuf, cg_unWStringHalloWorldSerSize-1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSecondSmallBuf, cg_unWStringHalloWorldSerSize-1, nVal), -1);

  nVal = (const char *) cg_abWStringNihongoUTF8;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unWStringNihongoSerSize);
  BOOST_CHECK(std::equal(cg_abWStringNihongo, cg_abWStringNihongo + cg_unWStringNihongoSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_TIME){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_TIME nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unTimeSerSize);
  BOOST_CHECK(std::equal(cg_abTime0, cg_abTime0 + cg_unStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal.fromString("T#3000ms");
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unTimeSerSize);
  BOOST_CHECK(std::equal(cg_abTime3000ms, cg_abTime3000ms + cg_unTimeSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal.fromString("T#3s22ms");
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unTimeSerSize);
  BOOST_CHECK(std::equal(cg_abTime3s22ms, cg_abTime3s22ms + cg_unTimeSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_MultiDatas){
  CFBDKASN1ComLayerTestMock nTestee;

  CIEC_TIME *poTimeVal1;
  CIEC_WORD *poWordVal;
  CIEC_STRING *poStringVal;
  CIEC_INT *poIntVal;
  CIEC_BOOL *poBoolVal;
  CIEC_TIME *poTimeVal2;

  TForteByte aoData[sizeof(CIEC_ANY) * 6];
  CIEC_ANY *aoArray = reinterpret_cast<CIEC_ANY *>(aoData);

  TIEC_ANYPtr poArray[6];

  poArray[0] = poTimeVal1 = new(reinterpret_cast<TForteByte *>(aoArray))CIEC_TIME();
  poArray[1] = poWordVal = new(reinterpret_cast<TForteByte *>(aoArray + 1))CIEC_WORD();
  poArray[2] = poStringVal = new(reinterpret_cast<TForteByte *>(aoArray + 2))CIEC_STRING();
  poArray[3] = poIntVal = new(reinterpret_cast<TForteByte *>(aoArray  + 3))CIEC_INT();
  poArray[4] = poBoolVal = new(reinterpret_cast<TForteByte *>(aoArray + 4))CIEC_BOOL();
  poArray[5] = poTimeVal2 = new(reinterpret_cast<TForteByte *>(aoArray + 5))CIEC_TIME();

  poTimeVal1->fromString("T#3000ms");
  *poWordVal = 40396;
  *poStringVal = "HalloWorld";
  *poIntVal = -10934;
  *poBoolVal = true;
  poTimeVal2->fromString("T#3s22ms");

  const unsigned int nSerSize = cg_unTimeSerSize + cg_unWordSerSize + cg_unStringHalloWorldSerSize + cg_unIntSerSize + cg_unBoolSerSize + cg_unTimeSerSize;
  TForteByte anGoodResult[] = {0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0, 0x52, 0x9D, 0xCC, 0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd', 0x43, 0xD5, 0x4A, 0x41, 0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0};

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(aoArray, 6));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), nSerSize);
  BOOST_CHECK(std::equal(anGoodResult, anGoodResult + nSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  TForteByte acSmallBuf[nSerSize - 1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, nSerSize - 2, const_cast<TConstIEC_ANYPtr *>(poArray), 6), -1);

  //free memory
  for(int i = 0; i < 6; ++i){
    poArray[i]->~CIEC_ANY();
  }
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_ARRAY){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_ARRAY nVal(5, g_nStringIdBOOL);
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;


  static_cast<CIEC_BOOL &>(*nVal[0]) = true;
  static_cast<CIEC_BOOL &>(*nVal[1]) = false;
  static_cast<CIEC_BOOL &>(*nVal[2]) = false;
  static_cast<CIEC_BOOL &>(*nVal[3]) = true;
  static_cast<CIEC_BOOL &>(*nVal[4]) = true;

  TForteByte acSmallBuf[7];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 3, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 7, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 3, nVal), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 7, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unBOOL5SerSize);
  BOOST_CHECK(std::equal(cg_abArrayBool10011, cg_abArrayBool10011 + cg_unBOOL5SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  static_cast<CIEC_BOOL &>(*nVal[0]) = false;
  static_cast<CIEC_BOOL &>(*nVal[1]) = true;
  static_cast<CIEC_BOOL &>(*nVal[2]) = false;
  static_cast<CIEC_BOOL &>(*nVal[3]) = true;
  static_cast<CIEC_BOOL &>(*nVal[4]) = false;

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nVal, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unBOOL5SerSize);
  BOOST_CHECK(std::equal(cg_abArrayBool01010, cg_abArrayBool01010 + cg_unBOOL5SerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  CIEC_ARRAY nSIntArray(4, g_nStringIdSINT);

  static_cast<CIEC_SINT &>(*nSIntArray[0]) = -128;
  static_cast<CIEC_SINT &>(*nSIntArray[1]) = 127;
  static_cast<CIEC_SINT &>(*nSIntArray[2]) = 0;
  static_cast<CIEC_SINT &>(*nSIntArray[3]) = -90;

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nSIntArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSINT4SerSize);
  BOOST_CHECK(std::equal(cg_abArraySINTm128_127_0_m90, cg_abArraySINTm128_127_0_m90 + cg_unSINT4SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  static_cast<CIEC_SINT &>(*nSIntArray[0]) = -90;
  static_cast<CIEC_SINT &>(*nSIntArray[1]) = 90;
  static_cast<CIEC_SINT &>(*nSIntArray[2]) = 127;
  static_cast<CIEC_SINT &>(*nSIntArray[3]) = 0;

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nSIntArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unSINT4SerSize);
  BOOST_CHECK(std::equal(cg_abArraySINTm90_90_127_0, cg_abArraySINTm90_90_127_0 + cg_unSINT4SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  CIEC_ARRAY nStringArray(2, g_nStringIdSTRING);

  static_cast<CIEC_STRING &>(*nStringArray[1]) = "HalloWorld";
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(&nStringArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cg_unString2SerSize);
  BOOST_CHECK(std::equal(cg_abArrayStringEmptyHalloWorld, cg_abArrayStringEmptyHalloWorld + cg_unString2SerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_SUITE_END()
