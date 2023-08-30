/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss, nxtControl and Profactor
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
 *   Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <algorithm>

#include "forte_boost_output_support.h"

#include "fbdkasn1layer_serdeserdata.h"

#include "../../../src/core/cominfra/fbdkasn1layer.h"

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
#include "fbdkasn1layerser_test_gen.cpp"
#endif


class CFBDKASN1ComLayerTestMock: public forte::com_infra::CFBDKASN1ComLayer{
  public:
    CFBDKASN1ComLayerTestMock() : forte::com_infra::CFBDKASN1ComLayer(nullptr, nullptr){
      mBottomLayer = &mTestLayer;
    }

    ~CFBDKASN1ComLayerTestMock(){
      mBottomLayer = nullptr;
    }

    void *getSendDataPtr() {
      return mTestLayer.mData;
    }
    unsigned int getSendDataSize(){
      return mTestLayer.mSize;
    }

  private:
   class TestMockBottomLayer : public forte::com_infra::CComLayer{
     public:
     TestMockBottomLayer() : forte::com_infra::CComLayer(nullptr,nullptr){
       mData = nullptr;
       mAllocData = nullptr;
       mSize = 0;
     }

     ~TestMockBottomLayer(){
       if (nullptr != mAllocData) {
         delete[] mAllocData;
       }
     }

      forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize){
        if (nullptr != mAllocData) {
          delete[] mAllocData;
        }
        mAllocData = new TForteByte[paSize];
        if (nullptr != mAllocData) {
          mData = mAllocData;
          memcpy(mData,paData,paSize);
          //mData = paData;
          mSize = paSize;
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

      TForteByte *mData;
      TForteByte *mAllocData;
       unsigned int mSize;
   };

   TestMockBottomLayer mTestLayer;
};



BOOST_AUTO_TEST_SUITE(fbdkasn1layer_serialize_test)

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_BOOL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_BOOL nBool;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nBool;

  nBool = CIEC_BOOL(false);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgBoolSerSize);
  BOOST_CHECK_EQUAL(cgBoolFalse, *((TForteByte *)nTestee.getSendDataPtr()));

  nBool = CIEC_BOOL(true);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgBoolSerSize);
  BOOST_CHECK_EQUAL(cgBoolTrue, *((TForteByte *)nTestee.getSendDataPtr()));

  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(static_cast<TForteByte *>(nullptr), 0, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(static_cast<TForteByte *>(nullptr), 0, nBool), -1);
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_BYTE){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_BYTE nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= CIEC_BYTE(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgByteSerSize);
  BOOST_CHECK(std::equal(cgByte0, cgByte0 + cgByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_BYTE(12);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgByteSerSize);
  BOOST_CHECK(std::equal(cgByte12, cgByte12 + cgByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_BYTE(128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgByteSerSize);
  BOOST_CHECK(std::equal(cgByte128, cgByte128 + cgByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_BYTE(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgByteSerSize);
  BOOST_CHECK(std::equal(cgByte255, cgByte255 + cgByteSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_WORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_WORD nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= CIEC_WORD(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWordSerSize);
  BOOST_CHECK(std::equal(cgWord0, cgWord0 + cgWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_WORD(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWordSerSize);
  BOOST_CHECK(std::equal(cgWord255, cgWord255 + cgWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_WORD(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWordSerSize);
  BOOST_CHECK(std::equal(cgWord256, cgWord256 + cgWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_WORD(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWordSerSize);
  BOOST_CHECK(std::equal(cgWord65535, cgWord65535 + cgWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_WORD(40396);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWordSerSize);
  BOOST_CHECK(std::equal(cgWord40396, cgWord40396 + cgWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_DWORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_DWORD nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= CIEC_DWORD(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord0, cgDWord0 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord255, cgDWord255 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord256, cgDWord256 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord65535, cgDWord65535 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(65536);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord65536, cgDWord65536 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(4294967295UL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord4294967295, cgDWord4294967295 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_DWORD(690586453);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDWordSerSize);
  BOOST_CHECK(std::equal(cgDWord690586453, cgDWord690586453 + cgDWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

//LWORD
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LWORD){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LWORD nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= CIEC_LWORD(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord0, cgLWord0 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord255, cgLWord255 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord256, cgLWord256 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord65535, cgLWord65535 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(65536);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord65536, cgLWord65536 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(4294967295LL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord4294967295, cgLWord4294967295 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(4294967296LL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord4294967296, cgLWord4294967296 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(18446744073709551615ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord18446744073709551615, cgLWord18446744073709551615 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_LWORD(18446744073709551615ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLWordSerSize);
  BOOST_CHECK(std::equal(cgLWord18446744073709551615, cgLWord18446744073709551615 + cgLWordSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_USINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_USINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= CIEC_USINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUSIntSerSize);
  BOOST_CHECK(std::equal(cgUSInt0, cgUSInt0 + cgUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_USINT(12);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUSIntSerSize);
  BOOST_CHECK(std::equal(cgUSInt12, cgUSInt12 + cgUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_USINT(128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUSIntSerSize);
  BOOST_CHECK(std::equal(cgUSInt128, cgUSInt128 + cgUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_USINT(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUSIntSerSize);
  BOOST_CHECK(std::equal(cgUSInt255, cgUSInt255 + cgUSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_UINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_UINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= CIEC_UINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUIntSerSize);
  BOOST_CHECK(std::equal(cgUInt0, cgUInt0 + cgUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UINT(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUIntSerSize);
  BOOST_CHECK(std::equal(cgUInt255, cgUInt255 + cgUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UINT(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUIntSerSize);
  BOOST_CHECK(std::equal(cgUInt256, cgUInt256 + cgUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= CIEC_UINT(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUIntSerSize);
  BOOST_CHECK(std::equal(cgUInt65535, cgUInt65535 + cgUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UINT(40396);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUIntSerSize);
  BOOST_CHECK(std::equal(cgUInt40396, cgUInt40396 + cgUIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_UDINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_UDINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= CIEC_UDINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt0, cgUDInt0 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt255, cgUDInt255 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt256, cgUDInt256 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt65535, cgUDInt65535 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(65536);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt65536, cgUDInt65536 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(4294967295UL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt4294967295, cgUDInt4294967295 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_UDINT(690586453);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgUDIntSerSize);
  BOOST_CHECK(std::equal(cgUDInt690586453, cgUDInt690586453 + cgUDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

//LWORD
BOOST_AUTO_TEST_CASE(Single_Serialize_Test_ULINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_ULINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= CIEC_ULINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt0, cgULInt0 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(255);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt255, cgULInt255 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= CIEC_ULINT(256);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt256, cgULInt256 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  nVal= CIEC_ULINT(65535);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt65535, cgULInt65535 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(65536);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt65536, cgULInt65536 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(4294967295ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt4294967295, cgULInt4294967295 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(4294967296ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt4294967296, cgULInt4294967296 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(18446744073709551615ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt18446744073709551615, cgULInt18446744073709551615 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal= CIEC_ULINT(18446744073709551615ULL);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgULIntSerSize);
  BOOST_CHECK(std::equal(cgULInt18446744073709551615, cgULInt18446744073709551615 + cgULIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}


BOOST_AUTO_TEST_CASE(Single_Serialize_Test_SINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_SINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);

  nVal= CIEC_SINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSIntSerSize);
  BOOST_CHECK(std::equal(cgSInt0, cgSInt0 + cgSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_SINT(-128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSIntSerSize);
  BOOST_CHECK(std::equal(cgSIntm128, cgSIntm128 + cgSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_SINT(127);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSIntSerSize);
  BOOST_CHECK(std::equal(cgSInt127, cgSInt127 + cgSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_SINT(-90);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSIntSerSize);
  BOOST_CHECK(std::equal(cgSIntm90, cgSIntm90 + cgSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_SINT(90);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSIntSerSize);
  BOOST_CHECK(std::equal(cgSInt90, cgSInt90 + cgSIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_INT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_INT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  nVal= CIEC_INT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgInt0, cgInt0 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(-128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgIntm128, cgIntm128 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(-129);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgIntm129, cgIntm129 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(127);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgInt127, cgInt127 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgInt128, cgInt128 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(-32768);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgIntm32768, cgIntm32768 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(32767);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgInt32767, cgInt32767 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(-10934);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgIntm10934, cgIntm10934 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_INT(10934);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgIntSerSize);
  BOOST_CHECK(std::equal(cgInt10934, cgInt10934 + cgIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_DINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_DINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  nVal= CIEC_DINT(0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt0, cgDInt0 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm128, cgDIntm128 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-129);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm129, cgDIntm129 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(127);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt127, cgDInt127 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(128);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt128, cgDInt128 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-32768);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm32768, cgDIntm32768 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-32769);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm32769, cgDIntm32769 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(32767);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt32767, cgDInt32767 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(32768);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt32768, cgDInt32768 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-2147483648L);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm2147483648, cgDIntm2147483648 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(2147483647L);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt2147483647, cgDInt2147483647 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(-800058586);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDIntm800058586, cgDIntm800058586 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_DINT(800058586);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgDIntSerSize);
  BOOST_CHECK(std::equal(cgDInt800058586, cgDInt800058586 + cgDIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LINT){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LINT nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  nVal= CIEC_LINT(0);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt0, cgLInt0 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-128);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm128, cgLIntm128 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-129);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm129, cgLIntm129 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(127);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt127, cgLInt127 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(128);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt128, cgLInt128 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-32768);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm32768, cgLIntm32768 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-32769);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm32769, cgLIntm32769 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(32767);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt32767, cgLInt32767 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(32768);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt32768, cgLInt32768 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-2147483648LL);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm2147483648, cgLIntm2147483648 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-2147483649LL);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm2147483649, cgLIntm2147483649 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(2147483647);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt2147483647, cgLInt2147483647 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(2147483648LL);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt2147483648, cgLInt2147483648 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-9223372036854775807LL - 1LL);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm9223372036854775808, cgLIntm9223372036854775808 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(9223372036854775807LL);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt9223372036854775807, cgLInt9223372036854775807 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(-800058586);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLIntm800058586, cgLIntm800058586 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

   nVal = CIEC_LINT(800058586);
   BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
   BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLIntSerSize);
   BOOST_CHECK(std::equal(cgLInt800058586, cgLInt800058586 + cgLIntSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_REAL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_REAL nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[4];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 4, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 4, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgRealSerSize);
  BOOST_CHECK(std::equal(cgReal0, cgReal0 + cgRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_REAL(2.2874e6f);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgRealSerSize);
  BOOST_CHECK(std::equal(cgReal2_2874e6, cgReal2_2874e6 + cgRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_REAL(-6.2587e-4f);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgRealSerSize);
  BOOST_CHECK(std::equal(cgRealm6_2587em4, cgRealm6_2587em4 + cgRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_REAL(1.0E-37f);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgRealSerSize);
  BOOST_CHECK(std::equal(cgReal1_0Em37, cgReal1_0Em37 + cgRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_REAL(36.0f);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgRealSerSize);
  BOOST_CHECK(std::equal(cgReal36_0, cgReal36_0 + cgRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_LREAL){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_LREAL nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLRealSerSize);
  BOOST_CHECK(std::equal(cgLReal0, cgLReal0 + cgLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_LREAL(2.28743e6);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLRealSerSize);
  BOOST_CHECK(std::equal(cgLReal2_28743e6, cgLReal2_28743e6 + cgLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_LREAL(-6.2587e-4);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLRealSerSize);
  BOOST_CHECK(std::equal(cgLRealm6_2587em4, cgLRealm6_2587em4 + cgLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_LREAL(1.0E-37);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLRealSerSize);
  BOOST_CHECK(std::equal(cgLReal1_0Em37, cgLReal1_0Em37 + cgLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_LREAL(36.0);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgLRealSerSize);
  BOOST_CHECK(std::equal(cgLReal36_0, cgLReal36_0 + cgLRealSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_STRING){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_STRING nVal;
  TIEC_ANYPtr poArray[1];
    poArray[0] = &nVal;

  TForteByte acSmallBuf[2];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 2, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 2, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgStringEmptySerSize);
  BOOST_CHECK(std::equal(cgStringEmpty, cgStringEmpty + cgStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = "HalloWorld"_STRING;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgStringHalloWorldSerSize);
  BOOST_CHECK(std::equal(cgStringHalloWorld, cgStringHalloWorld + cgStringHalloWorldSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

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

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWStringEmptySerSize);
  BOOST_CHECK(std::equal(cgWStringEmpty, cgWStringEmpty + cgWStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal = CIEC_WSTRING("HalloWorld");
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWStringHalloWorldSerSize);
  BOOST_CHECK(std::equal(cgWStringHalloWorld, cgWStringHalloWorld + cgWStringHalloWorldSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  TForteByte acSecondSmallBuf[3];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSecondSmallBuf, cgWStringHalloWorldSerSize-1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSecondSmallBuf, cgWStringHalloWorldSerSize-1, nVal), -1);

  nVal = CIEC_WSTRING((const char *) cgWStringNihongoUTF8);
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgWStringNihongoSerSize);
  BOOST_CHECK(std::equal(cgWStringNihongo, cgWStringNihongo + cgWStringNihongoSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_TIME){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_TIME nVal;
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;

  TForteByte acSmallBuf[8];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 8, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 8, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgTimeSerSize);
  BOOST_CHECK(std::equal(cgTime0, cgTime0 + cgStringEmptySerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal.fromString("T#3000ms");
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgTimeSerSize);
  BOOST_CHECK(std::equal(cgTime3000ms, cgTime3000ms + cgTimeSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  nVal.fromString("T#3s22ms");
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgTimeSerSize);
  BOOST_CHECK(std::equal(cgTime3s22ms, cgTime3s22ms + cgTimeSerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_MultiDatas){
  CFBDKASN1ComLayerTestMock nTestee;

  CIEC_TIME *poTimeVal1;
  CIEC_WORD *poWordVal;
  CIEC_STRING *poStringVal;
  CIEC_INT *poIntVal;
  CIEC_BOOL *poBoolVal;
  CIEC_TIME *poTimeVal2;

  TIEC_ANYPtr poArray[6];

  poArray[0] = poTimeVal1 = new CIEC_TIME();
  poArray[1] = poWordVal = new CIEC_WORD();
  poArray[2] = poStringVal = new CIEC_STRING();
  poArray[3] = poIntVal = new CIEC_INT();
  poArray[4] = poBoolVal = new CIEC_BOOL();
  poArray[5] = poTimeVal2 = new CIEC_TIME();

  poTimeVal1->fromString("T#3000ms");
  *poWordVal = CIEC_WORD(40396);
  *poStringVal = "HalloWorld"_STRING;
  *poIntVal = CIEC_INT(-10934);
  *poBoolVal = CIEC_BOOL(true);
  poTimeVal2->fromString("T#3s22ms");

  const unsigned int nSerSize = cgTimeSerSize + cgWordSerSize + cgStringHalloWorldSerSize + cgIntSerSize + cgBoolSerSize + cgTimeSerSize;
  TForteByte anGoodResult[] = {0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0, 0x52, 0x9D, 0xCC, 0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd', 0x43, 0xD5, 0x4A, 0x41, 0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0};

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 6));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), nSerSize);
  BOOST_CHECK(std::equal(anGoodResult, anGoodResult + nSerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  TForteByte acSmallBuf[nSerSize - 1];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, nSerSize - 2, const_cast<TConstIEC_ANYPtr *>(poArray), 6), -1);
  delete poTimeVal1;
  delete poWordVal;
  delete poStringVal;
  delete poIntVal;
  delete poBoolVal;
  delete poTimeVal2;
}

BOOST_AUTO_TEST_CASE(Single_Serialize_Test_ARRAY){
  CFBDKASN1ComLayerTestMock nTestee;
  CIEC_ARRAY_DYNAMIC nVal(5, g_nStringIdBOOL);
  TIEC_ANYPtr poArray[1];
  poArray[0] = &nVal;


  static_cast<CIEC_BOOL &>(nVal[0]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nVal[1]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nVal[2]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nVal[3]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nVal[4]) = CIEC_BOOL(true);

  TForteByte acSmallBuf[7];
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 1, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 3, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPointArray(acSmallBuf, 7, const_cast<TConstIEC_ANYPtr *>(poArray), 1), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 1, nVal), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 3, nVal), -1);
  BOOST_CHECK_EQUAL(nTestee.serializeDataPoint(acSmallBuf, 7, nVal), -1);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgBOOL5SerSize);
  BOOST_CHECK(std::equal(cgArrayBool10011, cgArrayBool10011 + cgBOOL5SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  static_cast<CIEC_BOOL &>(nVal[0]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nVal[1]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nVal[2]) = CIEC_BOOL(false);
  static_cast<CIEC_BOOL &>(nVal[3]) = CIEC_BOOL(true);
  static_cast<CIEC_BOOL &>(nVal[4]) = CIEC_BOOL(false);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgBOOL5SerSize);
  BOOST_CHECK(std::equal(cgArrayBool01010, cgArrayBool01010 + cgBOOL5SerSize, ((TForteByte *)nTestee.getSendDataPtr())));


  CIEC_ARRAY_DYNAMIC nSIntArray(4, g_nStringIdSINT);
  poArray[0] = &nSIntArray;

  static_cast<CIEC_SINT &>(nSIntArray[0]) = CIEC_SINT(-128);
  static_cast<CIEC_SINT &>(nSIntArray[1]) = CIEC_SINT(127);
  static_cast<CIEC_SINT &>(nSIntArray[2]) = CIEC_SINT(0);
  static_cast<CIEC_SINT &>(nSIntArray[3]) = CIEC_SINT(-90);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSINT4SerSize);
  BOOST_CHECK(std::equal(cgArraySINTm128_127_0_m90, cgArraySINTm128_127_0_m90 + cgSINT4SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  static_cast<CIEC_SINT &>(nSIntArray[0]) = CIEC_SINT(-90);
  static_cast<CIEC_SINT &>(nSIntArray[1]) = CIEC_SINT(90);
  static_cast<CIEC_SINT &>(nSIntArray[2]) = CIEC_SINT(127);
  static_cast<CIEC_SINT &>(nSIntArray[3]) = CIEC_SINT(0);

  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgSINT4SerSize);
  BOOST_CHECK(std::equal(cgArraySINTm90_90_127_0, cgArraySINTm90_90_127_0 + cgSINT4SerSize, ((TForteByte *)nTestee.getSendDataPtr())));

  CIEC_ARRAY_DYNAMIC nStringArray(2, g_nStringIdSTRING);
  poArray[0] = &nStringArray;

  static_cast<CIEC_STRING &>(nStringArray[1]) = "HalloWorld"_STRING;
  BOOST_CHECK_EQUAL(forte::com_infra::e_ProcessDataOk, nTestee.sendData(poArray, 1));
  BOOST_CHECK_EQUAL(nTestee.getSendDataSize(), cgString2SerSize);
  BOOST_CHECK(std::equal(cgArrayStringEmptyHalloWorld, cgArrayStringEmptyHalloWorld + cgString2SerSize, ((TForteByte *)nTestee.getSendDataPtr())));
}

BOOST_AUTO_TEST_SUITE_END()
