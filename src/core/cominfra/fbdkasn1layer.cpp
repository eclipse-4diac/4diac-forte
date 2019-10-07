/*******************************************************************************
 * Copyright (c) 2010-2013 TU Wien ACIN, Profactor GmbH, nxtControl GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Michael Hofmann, Stanislav Meduna,
 *    Martin Melik Merkumians
 *      - initial implementation, rework communication infrastructure, bug fixes
 *******************************************************************************/
#include "fbdkasn1layer.h"
#include "basecommfb.h"
#include "../../arch/timerha.h"
#include "../../arch/devlog.h"
#include <fortenew.h>

using namespace forte::com_infra;

CFBDKASN1ComLayer::CFBDKASN1ComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poComFB) :
  CComLayer(pa_poUpperLayer, pa_poComFB), mStatSerBuf(0), mStatSerBufSize(0), mDeserBuf(0), mDeserBufSize(0), mDeserBufPos(0), mDIPos(0), mDOPos(0){

  if(0 != pa_poComFB){
    unsigned int sdNum = pa_poComFB->getNumSD();
    CIEC_ANY *apoSDs = pa_poComFB->getSDs();
    for(unsigned int i = 0; i < sdNum; ++i){
      if(apoSDs != 0){
        TForteByte typeSize = csm_aDataTags[apoSDs->getDataTypeID()][1];
        if(typeSize != 255){
          mStatSerBufSize += typeSize + 1;
        }
        ++apoSDs;
      }
    }

    unsigned int rdNum = pa_poComFB->getNumRD();
    CIEC_ANY *apoRDs = pa_poComFB->getRDs();
    for(unsigned int i = 0; i < rdNum; ++i){
      if(0 != apoRDs){
        TForteByte typeSize = csm_aDataTags[apoRDs->getDataTypeID()][1];
        if(typeSize != 255){
          mDeserBufSize += typeSize + 1;
        }
        ++apoRDs;
      }
    }

    mStatSerBuf = new TForteByte[mStatSerBufSize];
    mDeserBuf = new TForteByte[mDeserBufSize];
  }
}

CFBDKASN1ComLayer::~CFBDKASN1ComLayer(){
  delete[] mStatSerBuf;
  delete[] mDeserBuf;
}

EComResponse CFBDKASN1ComLayer::openConnection(char *){
  //We don't need layer specific parameters
  return e_InitOk;
}

void CFBDKASN1ComLayer::closeConnection(){
  //We don't need to do anything specific on closing
}

void CFBDKASN1ComLayer::resizeDeserBuffer(unsigned int pa_size){
  //FIXME use realloc here
  TForteByte *newBuf = new TForteByte[pa_size];
  for(unsigned int i = 0; i < mDeserBufPos; i++){
    newBuf[i] = mDeserBuf[i];
  }
  delete[] mDeserBuf;
  mDeserBuf = newBuf;
  mDeserBufSize = pa_size;
}

EComResponse CFBDKASN1ComLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  EComResponse eRetVal = e_ProcessDataNoSocket;

  if(m_poBottomLayer != 0){
    TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr > (pa_pvData);
    unsigned int unNeededBufferSize = 0;

    if(0 == apoSDs){
       return e_ProcessDataDataTypeError;
     }

    for(size_t i = 0; i < pa_unSize; ++i){
      unNeededBufferSize += getRequiredSerializationSize(apoSDs[i]);
    }
    TForteByte *paUsedBuffer = 0;
    TForteByte *paDynSerBuffer = 0;
    //FIXME grow buffer instead of allocating every time a new one
    if(unNeededBufferSize <= mStatSerBufSize){
      paUsedBuffer = mStatSerBuf;
    }
    else{
       paDynSerBuffer = new TForteByte[unNeededBufferSize];
       paUsedBuffer = paDynSerBuffer;
    }

    int ser_size = -1; //stays negative if no Buffer for serialization is provided or error occurs while serialization

    if (0 != paUsedBuffer) {
      ser_size = serializeFBDataPointArray(paUsedBuffer, unNeededBufferSize, apoSDs, pa_unSize);
    }

    if(ser_size > 0){
      eRetVal = m_poBottomLayer->sendData(paUsedBuffer, ser_size);
    }
    else{ // serialize failed
      eRetVal = e_ProcessDataDataTypeError;
      DEVLOG_ERROR("CAsn1Layer:: serializeData failed\n");
    }

    if(0 != paDynSerBuffer){
      delete[] paDynSerBuffer;
    }
  }

  return eRetVal;
}

EComResponse CFBDKASN1ComLayer::recvData(const void *paData, unsigned int paSize){
  TForteByte *receivedData = const_cast<TForteByte*>(static_cast<const TForteByte *>(paData));
  EComResponse eRetVal = e_Nothing;

  if(m_poFb != 0){
    CIEC_ANY *apoRDs = m_poFb->getRDs();
    unsigned int unNumRD = m_poFb->getNumRD();
    unsigned int usedBufferSize = 0;
    TForteByte *usedBuffer = 0;

    // TODO: only copy if necessary
    if(0 == mDeserBufPos){
      usedBuffer = receivedData;
      usedBufferSize = paSize;
    }
    else{
      if((paSize + mDeserBufPos) > mDeserBufSize){
        resizeDeserBuffer(paSize + mDeserBufPos);
      }
      memcpy((mDeserBuf + mDeserBufPos), receivedData, paSize);
      usedBuffer = mDeserBuf;
      mDeserBufPos = mDeserBufPos + paSize;
      usedBufferSize = mDeserBufPos;
    }
    int nBuf;
    while(usedBufferSize != 0){
      if(unNumRD == 0){
        nBuf = 1;
      }
      else{
        if(0 == apoRDs){
          DEVLOG_ERROR("Data type error\n");
          eRetVal = e_ProcessDataDataTypeError;
          break;
        }
        nBuf = deserializeDataPoint(usedBuffer, usedBufferSize, apoRDs[mDOPos]);
      }

      // deserialize failed, copy data into buffer for next package
      if(nBuf <= 0){
        if(-2 == nBuf){
          //We had a data type problem
          eRetVal = e_ProcessDataDataTypeError;
          break;
        }
        else{
          if(0 == mDeserBufPos){ //usedBuffer == recvData
            if((mDeserBufSize) < usedBufferSize){
              resizeDeserBuffer(usedBufferSize);
            }
            memcpy(mDeserBuf, usedBuffer, usedBufferSize);
            mDeserBufPos = usedBufferSize;
          }
          nBuf = usedBufferSize;
        }
      }
      else{
        mDOPos++;
        mDeserBufPos = 0; // TODO: MH: think on this
      }

      usedBufferSize -= nBuf;
      usedBuffer += nBuf;

      // required if e.g. the first element is an bool and the second element is "fragmented" - otherwise the resize in the buffer will not be resized
      receivedData += nBuf;

      if(mDOPos >= unNumRD){
        mDOPos = 0;
        eRetVal = e_ProcessDataOk;
        //FIXME if at this point data is left it is from a second run. Better first deliver this packet and afterwards handle second run
      }
    }
  }
  return eRetVal;
}

/*Initialization csm_aDataTags array
 *
 */
const TForteByte CFBDKASN1ComLayer::csm_aDataTags[][2] = {
//!< {Tag, Size of data + tag size}; Size == 255 means unknown
//TODO: consider size=0 for unknown
    { e_APPLICATION + e_PRIMITIVE + e_ANY_TAG, 255 }, { e_APPLICATION + e_PRIMITIVE + e_BOOL_TAG, 1 }, { e_APPLICATION + e_PRIMITIVE + e_SINT_TAG, 2 }, { e_APPLICATION + e_PRIMITIVE + e_INT_TAG, 3 }, { e_APPLICATION + e_PRIMITIVE + e_DINT_TAG, 5 }, { e_APPLICATION + e_PRIMITIVE + e_LINT_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_USINT_TAG, 2 }, { e_APPLICATION + e_PRIMITIVE + e_UINT_TAG, 3 }, { e_APPLICATION + e_PRIMITIVE + e_UDINT_TAG, 5 }, { e_APPLICATION + e_PRIMITIVE + e_ULINT_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_BYTE_TAG, 2 }, { e_APPLICATION + e_PRIMITIVE + e_WORD_TAG, 3 }, { e_APPLICATION + e_PRIMITIVE + e_DWORD_TAG, 5 }, { e_APPLICATION + e_PRIMITIVE + e_LWORD_TAG, 9 },  {
        e_APPLICATION + e_PRIMITIVE + e_DATE_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_TIME_OF_DAY_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_DATE_AND_TIME_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_TIME_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_REAL_TAG, 5 }, { e_APPLICATION + e_PRIMITIVE + e_LREAL_TAG, 9 }, { e_APPLICATION + e_PRIMITIVE + e_STRING_TAG, 255 }, { e_APPLICATION + e_PRIMITIVE + e_WSTRING_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_DerivedData_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_DirectlyDerivedData_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_EnumeratedData_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_SubrangeData_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_ARRAY_TAG, 255 }, { e_APPLICATION + e_CONSTRUCTED + e_STRUCT_TAG, 255 } };

int CFBDKASN1ComLayer::serializeDataPointArray(TForteByte *pa_pcBytes, unsigned int pa_nStreamSize, TConstIEC_ANYPtr *pa_apoData, unsigned int pa_nDataNum){
  int nRetVal = -1;
  if(0 == pa_nDataNum){
    serializeNull(pa_pcBytes);
    nRetVal = 1;
  }
  else{
    int nRemainingBytes = pa_nStreamSize;
    int nBuf;
    nRetVal = 0;
    for(unsigned int i = 0; i < pa_nDataNum; i++){
      if(0 != (pa_apoData[i])){
        nBuf = serializeDataPoint(pa_pcBytes, nRemainingBytes, *pa_apoData[i]);
        if(0 < nBuf){
          nRetVal += nBuf;
          nRemainingBytes -= nBuf;
          pa_pcBytes += nBuf;
        }
        else{
          nRetVal = -1;
          break;
        }
      }
      else{
        nRetVal = -1;
        break;
      }
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::serializeDataPoint(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY &pa_roCIECData){
  int nRetVal = -1;
  if(0 < pa_nStreamSize){
    serializeTag(pa_pcBytes, pa_roCIECData);
    nRetVal = serializeValue(pa_pcBytes + 1, pa_nStreamSize - 1, pa_roCIECData);
    if(0 <= nRetVal){
      ++nRetVal;        //add one for the tag
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::serializeFBDataPointArray(TForteByte* pa_pcBytes, unsigned int pa_nStreamSize, TConstIEC_ANYPtr pa_aoData, unsigned int pa_nDataNum){
  int nRetVal = -1;
    if(0 == pa_nDataNum){
      serializeNull(pa_pcBytes);
      nRetVal = 1;
    }
    else{
      if(0 == pa_aoData){
        return -1;
      }
      int nRemainingBytes = pa_nStreamSize;
      nRetVal = 0;
      for(unsigned int i = 0; i < pa_nDataNum; i++){
        int nBuf = serializeDataPoint(pa_pcBytes, nRemainingBytes, pa_aoData[i]);
        if(0 < nBuf){
          nRetVal += nBuf;
          nRemainingBytes -= nBuf;
          pa_pcBytes += nBuf;
        }
        else{
          nRetVal = -1;
          break;
        }
      }
    }
    return nRetVal;
}

void CFBDKASN1ComLayer::serializeTag(TForteByte* pa_pcBytes, const CIEC_ANY &pa_roCIECData){
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
  if (CIEC_ANY::e_External == pa_roCIECData.getDataTypeID()) {
    pa_pcBytes[0] = pa_roCIECData.getTag();
  } else {
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */
    if (CIEC_ANY::e_STRUCT != pa_roCIECData.getDataTypeID()) {
      pa_pcBytes[0] = csm_aDataTags[pa_roCIECData.getDataTypeID()][0];
    }
    else {
      //get ASN1-tag from implementing datatype for STRUCT-datatypes
      pa_pcBytes[0] = ((CIEC_STRUCT&) pa_roCIECData).getASN1StructType();
    }
      //TODO add bool stuff here
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
  }
#endif/* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

}

int CFBDKASN1ComLayer::serializeValue(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY &pa_roCIECData){
  int nRetVal = -1;

  CIEC_ANY::EDataTypeID eDataType = pa_roCIECData.getDataTypeID();

  if((CIEC_ANY::e_BOOL < eDataType && eDataType <= CIEC_ANY::e_DATE_AND_TIME)||(CIEC_ANY::e_REAL == eDataType)||(CIEC_ANY::e_LREAL == eDataType)){
    //Simple data types except bool can be handled the same way
    nRetVal = serializeValueSimpleDataType(pa_pcBytes, pa_nStreamSize, pa_roCIECData);
  }
  else{
    switch (eDataType){
      case CIEC_ANY::e_BOOL:
        if(!((CIEC_BOOL &) pa_roCIECData).operator bool()){
          //data of bool is encoded in the tag; if CIEC_BOOL == false => Tag must be e_APPLICATION + e_PRIMITIVE
          pa_pcBytes[-1] = static_cast<TForteByte>(pa_pcBytes[-1] - static_cast<TForteByte>(e_BOOL_TAG));
        }
        nRetVal = 0;
        break;
      case CIEC_ANY::e_TIME:
        nRetVal = serializeValueTime(pa_pcBytes, pa_nStreamSize, static_cast<const CIEC_TIME &>(pa_roCIECData));
        break;
#ifdef FORTE_USE_WSTRING_DATATYPE
      case CIEC_ANY::e_WSTRING:
        nRetVal = serializeValueWString(pa_pcBytes, pa_nStreamSize, static_cast<const CIEC_WSTRING &>(pa_roCIECData));
        break;
#endif
      case CIEC_ANY::e_STRING:
        nRetVal = serializeValueString(pa_pcBytes, pa_nStreamSize, static_cast<const CIEC_STRING &>(pa_roCIECData));
        break;
#ifdef FORTE_SUPPORT_ARRAYS
      case CIEC_ANY::e_ARRAY:
        nRetVal = serializeArray(pa_pcBytes, pa_nStreamSize, static_cast<const CIEC_ARRAY &>(pa_roCIECData));
        break;
#endif /* FORTE_SUPPORT_ARRAYS */
      case CIEC_ANY::e_STRUCT:
        nRetVal = serializeValueStruct(pa_pcBytes, pa_nStreamSize, static_cast<const CIEC_STRUCT &>(pa_roCIECData));
        break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
      case CIEC_ANY::e_External:
        nRetVal = pa_roCIECData.serializeCustomType(pa_pcBytes, pa_nStreamSize);
        break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

      default:
        break;
    }
  }

  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueSimpleDataType(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY & pa_roDataPoint){
  int nRetVal = csm_aDataTags[pa_roDataPoint.getDataTypeID()][1];
  --nRetVal; //Length of the tag

  if(nRetVal <= pa_nStreamSize){
    const TForteByte* acDataPtr = pa_roDataPoint.getConstDataPtr();

#ifdef FORTE_LITTLE_ENDIAN
# if defined(__ARMEL__) && ! defined(__VFP_FP__) // Little endian ARM with old mixed endian FPA float ABI needs to swap
        if(CIEC_ANY::e_LREAL == pa_roDataPoint.getDataTypeID()) {
          TForteUInt32 anSwapped[2];
          anSwapped[0] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[1];
          anSwapped[1] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[0];
          acDataPtr = reinterpret_cast<const TForteByte*>(&anSwapped[0]);
        }
# endif //defined(__ARMEL__) && ! defined(__VFP_FP__)
      for(int i = 0; i < nRetVal; i++){
        pa_pcBytes[(nRetVal - 1) - i] = acDataPtr[i];
      }
#endif //FORTE_LITTLE_ENDIAN

#ifdef FORTE_BIG_ENDIAN
      if(CIEC_ANY::e_REAL != pa_roDataPoint.getDataTypeID()){
        for (int i = 0; i < nRetVal; i++){
          pa_pcBytes[(nRetVal - 1) - i] = acDataPtr[(sizeof(CIEC_ANY::TLargestUIntValueType) - 1)-i];
        }
      }
      else{
        for (unsigned int i = 0; i < nRetVal; i++){
          pa_pcBytes[(nRetVal - 1) - i] = acDataPtr[(sizeof(TForteFloat) - 1) - i];
        }
      }
#endif //FORTE_BIG_ENDIAN
  }
  else{
    nRetVal = -1;
  }

  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueTime(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_TIME & pa_roTime){
  int nRetVal = -1;
  if(8 <= pa_nStreamSize){
    TForteInt64 timeInMicroSeconds = pa_roTime.getInMicroSeconds();
    pa_pcBytes[0] = (TForteByte) (timeInMicroSeconds >> 56) & 0xFF;
    pa_pcBytes[1] = (TForteByte) (timeInMicroSeconds >> 48) & 0xFF;
    pa_pcBytes[2] = (TForteByte) (timeInMicroSeconds >> 40) & 0xFF;
    pa_pcBytes[3] = (TForteByte) (timeInMicroSeconds >> 32) & 0xFF;
    pa_pcBytes[4] = (TForteByte) (timeInMicroSeconds >> 24) & 0xFF;
    pa_pcBytes[5] = (TForteByte) (timeInMicroSeconds >> 16) & 0xFF;
    pa_pcBytes[6] = (TForteByte) (timeInMicroSeconds >> 8) & 0xFF;
    pa_pcBytes[7] = (TForteByte) (timeInMicroSeconds) & 0xFF;
    nRetVal = 8;
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueString(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_STRING & pa_roString){
  int nRetVal = -1;
  TForteUInt16 unStringLen = pa_roString.length();
  if(unStringLen + 2 <= pa_nStreamSize){
    pa_pcBytes[0] = (TForteByte) ((unStringLen >> 8) & 0x00FF);
    pa_pcBytes[1] = (TForteByte) (unStringLen & 0x00FF);
    pa_pcBytes += 2;
    memcpy(pa_pcBytes, pa_roString.getValue(), unStringLen);
    nRetVal = unStringLen + 2;
  }
  return nRetVal;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
int CFBDKASN1ComLayer::serializeValueWString(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_WSTRING & pa_roWString){
  int nRetVal = -1;

  if (pa_nStreamSize < 2) {
    return -1;
  }
  nRetVal = pa_roWString.toUTF16(pa_pcBytes + 2, pa_nStreamSize - 2);
  if (nRetVal < 0 || nRetVal > 131070) {
    return -1;
  }
  pa_pcBytes[0] = (TForteByte) ((nRetVal >> 9) & 0x00FF);
  pa_pcBytes[1] = (TForteByte) ((nRetVal >> 1) & 0x00FF);
  nRetVal += 2;

  return nRetVal;
}
#endif

#ifdef FORTE_SUPPORT_ARRAYS

int CFBDKASN1ComLayer::serializeArray(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ARRAY &pa_roArray){
  int nRetVal = -1;
  TForteUInt16 nArraySize = pa_roArray.size();

  //Number of array elements
  pa_pcBytes[0] = (TForteByte) ((nArraySize >> 8) & 0x00FF);
  pa_pcBytes[1] = (TForteByte) (nArraySize & 0x00FF);

  pa_pcBytes += 2;
  //TODO should we check if the array has size zero?


  if( CIEC_ANY::e_BOOL == pa_roArray[0]->getDataTypeID()){
    //bool arrays are special
    nRetVal = 2; // array len
    pa_nStreamSize -= nRetVal;
    for(TForteUInt16 i = 0; i < nArraySize; i++){ //serialize elements
      int nSerSize = serializeDataPoint(pa_pcBytes, pa_nStreamSize, *pa_roArray[i]);
      if(0 < nSerSize){
        nRetVal += nSerSize;
        pa_pcBytes += nSerSize;
        pa_nStreamSize -= nSerSize;
      }
      else{
        nRetVal = -1;
        break;
      }
    }
  }
  else{
    serializeTag(pa_pcBytes, *pa_roArray[0]);

    nRetVal = 2 + 1; // array len + contained data tag
    pa_nStreamSize -= nRetVal;

    ++pa_pcBytes;
    for(TForteUInt16 i = 0; i < nArraySize; i++){ //serialize elements
      int nSerSize = serializeValue(pa_pcBytes, pa_nStreamSize, *pa_roArray[i]);
      if(-1 == nSerSize){
        nRetVal = -1;
        break;
      }
      nRetVal += nSerSize;
      pa_nStreamSize -= nSerSize;
      pa_pcBytes += nSerSize; //shift buffer array
    }

  }
  return nRetVal;
}
#endif

int CFBDKASN1ComLayer::serializeValueStruct(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_STRUCT & pa_roStruct) {
  int nStreamUsed = 0;
  int nTotalStreamUsed = 0;

  int nStructSize = pa_roStruct.getStructSize();

  for (int i = 0; i < nStructSize; i++){
    const CIEC_ANY& ro_val = (pa_roStruct.getMembers())[i];
    if (CIEC_ANY::e_BOOL == ro_val.getDataTypeID()) {
      //Handle BOOL-values differently, since value is encoded in tag
      nStreamUsed = serializeDataPoint(pa_pcBytes, pa_nStreamSize, ro_val);
    }
    else {
      nStreamUsed = serializeValue(pa_pcBytes, pa_nStreamSize, ro_val);
    }


    if(-1 == nStreamUsed) {
      nTotalStreamUsed = -1;
      break;
    }
    pa_nStreamSize -= nStreamUsed;
    pa_pcBytes += nStreamUsed;
    nTotalStreamUsed += nStreamUsed;
  }
  return nTotalStreamUsed;
}

bool CFBDKASN1ComLayer::deserializeDataPointArray(const TForteByte* pa_pcBytes, unsigned int pa_nStreamSize, TIEC_ANYPtr *pa_apoData, unsigned int pa_nDataNum){
  bool bRetval = true;
  if(0 == pa_nDataNum){
    bRetval = ((pa_nStreamSize == 1) && (isNull(pa_pcBytes)));
  }
  else{
    for(unsigned int i = 0; i < pa_nDataNum; ++i){
      if(0 == pa_apoData[i]){
        bRetval = false;
        break;
      }
      int nBuf = deserializeDataPoint(pa_pcBytes, pa_nStreamSize, *pa_apoData[i]);
      if(nBuf <= 0){
        // we could not deserialize the data of we have a to small packet
        // with the compliance profile for feasibility demonstration's protocol we can not determine packet order
        // and therefore not handle
        bRetval = false;
        break;
      }
      pa_nStreamSize -= nBuf;
      pa_pcBytes += nBuf;
    }
  }
  return bRetval;
}

int CFBDKASN1ComLayer::deserializeDataPoint(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roCIECData){
  int nRetVal = -1;
  if(0 < pa_nStreamSize && e_APPLICATION == (pa_pcBytes[0] & e_APPLICATION)) {
    if(deserializeTag(*pa_pcBytes, pa_roCIECData)) {
      nRetVal = deserializeValue(pa_pcBytes + 1, pa_nStreamSize - 1, pa_roCIECData) + 1;
    } else {
      DEVLOG_ERROR("Datatype error\n");
      nRetVal = -2;
    }
  }
  return nRetVal;
}

bool CFBDKASN1ComLayer::deserializeTag(const TForteByte pa_cByte, CIEC_ANY &pa_roCIECData){
  bool bRetVal;
  CIEC_ANY::EDataTypeID eDataType = pa_roCIECData.getDataTypeID();

  switch(eDataType){
    case CIEC_ANY::e_BOOL:
      bRetVal = (((e_APPLICATION + e_PRIMITIVE) == pa_cByte) || ((e_APPLICATION + e_PRIMITIVE + CIEC_ANY::e_BOOL) == pa_cByte));
      if(bRetVal){
        static_cast<CIEC_BOOL &>(pa_roCIECData) = (csm_aDataTags[CIEC_ANY::e_BOOL][0] == pa_cByte);
      }
      break;
    case CIEC_ANY::e_STRUCT:
      bRetVal = (static_cast<CIEC_STRUCT &>(pa_roCIECData).getASN1StructType() == pa_cByte);
      break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      bRetVal = pa_roCIECData.deserializeTag(pa_cByte);
      break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

    default:
      bRetVal = (csm_aDataTags[eDataType][0] == pa_cByte);
      break;
  }
  return bRetVal;
}

int CFBDKASN1ComLayer::deserializeValue(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roCIECData){
  int nRetVal = -1;
  CIEC_ANY::EDataTypeID eDataType = pa_roCIECData.getDataTypeID();

  if((CIEC_ANY::e_BOOL < eDataType && eDataType <= CIEC_ANY::e_DATE_AND_TIME)||(CIEC_ANY::e_REAL == eDataType)||(CIEC_ANY::e_LREAL == eDataType)){//e_ANY to e_DATE_AND_TIME can be handled the same way
    nRetVal = deserializeValueSimpleDataType(pa_pcBytes, pa_nStreamSize, pa_roCIECData);
  }
  else{
    switch (eDataType){
      case CIEC_ANY::e_BOOL:
        //bool data is decoded in the bool tag
        nRetVal = 0;
        break;
      case CIEC_ANY::e_TIME:
        nRetVal = deserializeValueTime(pa_pcBytes, pa_nStreamSize, static_cast<CIEC_TIME &>(pa_roCIECData));
        break;
#ifdef FORTE_USE_WSTRING_DATATYPE
      case CIEC_ANY::e_WSTRING:
        nRetVal = deserializeValueWString(pa_pcBytes, pa_nStreamSize, static_cast<CIEC_WSTRING &>(pa_roCIECData));
        break;
#endif
      case CIEC_ANY::e_STRING:
        nRetVal = deserializeValueString(pa_pcBytes, pa_nStreamSize, static_cast<CIEC_STRING &>(pa_roCIECData));
        break;
#ifdef FORTE_SUPPORT_ARRAYS
      case CIEC_ANY::e_ARRAY:
        nRetVal = deserializeArray(pa_pcBytes, pa_nStreamSize, static_cast<CIEC_ARRAY &>(pa_roCIECData));
        break;
#endif /* FORTE_SUPPORT_ARRAYS*/
      case CIEC_ANY::e_STRUCT:
        nRetVal = deserializeValueStruct(pa_pcBytes, pa_nStreamSize, static_cast<CIEC_STRUCT &>(pa_roCIECData));
        break;

#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      nRetVal = pa_roCIECData.deserializeCustomType(pa_pcBytes, pa_nStreamSize);
      break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */
      default:
        break;
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueSimpleDataType(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roIECData){
  int nRetVal = -1;
  int nValueSize = csm_aDataTags[pa_roIECData.getDataTypeID()][1] - 1;

  if(pa_nStreamSize >= nValueSize){
    TForteByte *acDataPtr = pa_roIECData.getDataPtr();

    //setting m_uAnyData to 0
    *((CIEC_ANY::TLargestUIntValueType *) acDataPtr) = 0;

    //we only need to check for SINT, INT, and DINT as LINT will fill all bytes
    if(pa_roIECData.getDataTypeID() <= CIEC_ANY::e_DINT && (pa_pcBytes[0] & 0x80)) {
      //we received a negative number set all bits to true
      *((CIEC_ANY::TLargestIntValueType *) acDataPtr) = -1;
    }

#ifdef FORTE_LITTLE_ENDIAN
      for(int i = 0; i < nValueSize; i++){
        acDataPtr[i] = pa_pcBytes[(nValueSize - 1) - i];
      }
# if defined(__ARMEL__) && ! defined(__VFP_FP__) // Little endian ARM with old mixed endian FPA float ABI needs to swap
        if(CIEC_ANY::e_LREAL == pa_roIECData.getDataTypeID()) {
          TForteUInt32 nTmp = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[1];
          ((TForteUInt32 *) acDataPtr)[1] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[0];
          reinterpret_cast<TForteUInt32 *>(acDataPtr)[0] = nTmp;
        }
# endif //defined(__ARMEL__) && ! defined(__VFP_FP__)
#endif //FORTE_LITTLE_ENDIAN
#ifdef FORTE_BIG_ENDIAN
      if(CIEC_ANY:: e_REAL != pa_roIECData.getDataTypeID()){
        for (unsigned int i=0; i < nValueSize;i++){
          acDataPtr[(sizeof(CIEC_ANY::TLargestUIntValueType) - 1) - i] = pa_pcBytes[(nValueSize - 1) - i];
        }
      }
      else{ //FLOAT must be processed this way, because it is always saved in the first 4 bytes
        for (unsigned int i = 0; i < nValueSize; i++){
          acDataPtr[(sizeof(TForteFloat) - 1) - i] = pa_pcBytes[(nValueSize - 1) - i];
        }
      }
#endif //FORTE_BIG_ENDIAN
    nRetVal = nValueSize;
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueTime(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_TIME &pa_roIECData){
  int nRetVal = -1;
  if(pa_nStreamSize >= 8){
    TForteInt64 timeInMicroSeconds = ((((TForteInt64) pa_pcBytes[0]) << 56) & 0xFF00000000000000LL) + ((((TForteInt64) pa_pcBytes[1]) << 48) & 0x00FF000000000000LL) + ((((TForteInt64) pa_pcBytes[2]) << 40) & 0x0000FF0000000000LL) + ((((TForteInt64) pa_pcBytes[3]) << 32) & 0x000000FF00000000LL) + ((((TForteInt64) pa_pcBytes[4]) << 24) & 0x00000000FF000000) + ((((TForteInt64) pa_pcBytes[5]) << 16) & 0x0000000000FF0000) + ((((TForteInt64) pa_pcBytes[6]) << 8) & 0x000000000000FF00) + (((TForteInt64) pa_pcBytes[7]) & 0x00000000000000FF);
    pa_roIECData.setFromMicroSeconds(timeInMicroSeconds);
    nRetVal = 8;
  }
  return nRetVal;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
int CFBDKASN1ComLayer::deserializeValueWString(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_WSTRING &pa_roIECData){
  int nRetVal = -1;
  if(pa_nStreamSize >= 2){
    TForteUInt16 unStringSize = static_cast<TForteUInt16>(((static_cast<TForteUInt16>(pa_pcBytes[0]) << 8) & 0xFF00) + (static_cast<TForteUInt16>(pa_pcBytes[1]) & 0x00FF));
    if(pa_nStreamSize >= (unStringSize * 2 + 2) && pa_roIECData.fromUTF16(pa_pcBytes + 2, unStringSize*2)) {
      nRetVal = unStringSize * 2 + 2;
    }
  }
  return nRetVal;
}
#endif

int CFBDKASN1ComLayer::deserializeValueString(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_STRING &pa_roIECData){
  int nRetVal = -1;
  if(pa_nStreamSize >= 2){
    TForteUInt16 unStringSize = static_cast<TForteUInt16>(((((TForteUInt16) pa_pcBytes[0]) << 8) & 0xFF00) + ((TForteUInt16) pa_pcBytes[1] & 0x00FF));
    if(pa_nStreamSize >= (unStringSize + 2)){
      pa_roIECData.assign((const char*)&(pa_pcBytes[2]), unStringSize);
      nRetVal = 2 + unStringSize;
    }
  }
  return nRetVal;
}

#ifdef FORTE_SUPPORT_ARRAYS

int CFBDKASN1ComLayer::deserializeArray(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ARRAY &pa_roArray){
  int nRetVal = -1;

  if(pa_nStreamSize >= 2){
    TForteUInt16 nSize = static_cast<TForteUInt16>(((((TForteUInt16) pa_pcBytes[0]) << 8) & 0xFF00) + ((TForteUInt16) pa_pcBytes[1] & 0x00FF));

    //number of elements in ARRAY must be read from the incoming message
    nRetVal = 2;
    if(nSize > 0){
      int nValueLen;
      pa_pcBytes += 2;
      pa_nStreamSize -= 2;

      //TODO do we need to check if the array's size is bigger than 0
      if(CIEC_ANY::e_BOOL == pa_roArray[0]->getDataTypeID()){
        //bool arrays are special
        nValueLen = deserializeValueBoolArray(pa_pcBytes, pa_nStreamSize, pa_roArray, nSize);
        if(0 <= nValueLen){
          nRetVal += nValueLen;
        }
        else{
          nRetVal = nValueLen;
        }
      }
      else{
        if(!deserializeTag(*pa_pcBytes, *pa_roArray[0])){
          return -1;
        }
        pa_pcBytes += 1;
        pa_nStreamSize -= 1;
        ++nRetVal;
        CIEC_ANY *poBufVal = 0;
        TForteUInt16 unArraySize = pa_roArray.size();

        for(TForteUInt16 i = 0; i < nSize; ++i){
          if(i < unArraySize){
            nValueLen = deserializeValue(pa_pcBytes, pa_nStreamSize, *pa_roArray[i]);
          }
          else{
            if(poBufVal == 0) {
              poBufVal = pa_roArray[0]->clone(0);
            }
            nValueLen = deserializeValue(pa_pcBytes, pa_nStreamSize, *poBufVal);
          }
          //size of the elements is given by the array datatype
          if(nValueLen <= 0) {
            if(poBufVal != 0) {
              delete poBufVal;
            }
            return nValueLen;
          }
          pa_nStreamSize -= nValueLen;
          pa_pcBytes += nValueLen;
          nRetVal += nValueLen;
        }

        if (poBufVal != 0)
          delete poBufVal;
      }
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueBoolArray(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ARRAY &pa_roArray, TForteUInt16 pa_unDecodedArraySize){
  int nRetVal = 0;
  CIEC_BOOL oBoolVal;  //buffer value for handling to large input data

  TForteUInt16 unArraySize = pa_roArray.size();
  int nValueLen;

  for(TForteUInt16 i = 0; i < pa_unDecodedArraySize; ++i){
    if(i < unArraySize){
      nValueLen = deserializeDataPoint(pa_pcBytes, pa_nStreamSize, *pa_roArray[i]);
    }
    else{
      nValueLen = deserializeDataPoint(pa_pcBytes, pa_nStreamSize, oBoolVal);
    }
    if(nValueLen <= 0){
      nRetVal = nValueLen;
      break;
    }
    pa_pcBytes += nValueLen;
    pa_nStreamSize -= nValueLen;
    nRetVal += nValueLen;
  }
  return nRetVal;
}

#endif

int CFBDKASN1ComLayer::deserializeValueStruct(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_STRUCT &pa_roIECData){
  int nRetVal = 0;
  int nValueLen;

  for (unsigned int i=0; i < pa_roIECData.getStructSize(); i++) {
    CIEC_ANY& ro_val = (pa_roIECData.getMembers())[i];
    if (CIEC_ANY::e_BOOL == ro_val.getDataTypeID()) {
      //Hanlde BOOL-datatype differently since value is encoded in tag
      nValueLen = deserializeDataPoint(pa_pcBytes, pa_nStreamSize, ro_val);
    }
    else {
      nValueLen = deserializeValue(pa_pcBytes, pa_nStreamSize, ro_val);
    }
    if(nValueLen <= 0){
      nRetVal = nValueLen;
      break;
    }
    pa_nStreamSize -= nValueLen;
    pa_pcBytes += nValueLen;
    nRetVal += nValueLen;
  }
  return nRetVal;
}


unsigned int CFBDKASN1ComLayer::getRequiredSerializationSize(const CIEC_ANY &pa_roCIECData){
  unsigned int unRetVal = 0;

  switch (pa_roCIECData.getDataTypeID()){
    case CIEC_ANY::e_STRING:
      unRetVal += ((CIEC_STRING&)pa_roCIECData).length() + 3;
      break;
#ifdef FORTE_USE_WSTRING_DATATYPE
    case CIEC_ANY::e_WSTRING:
      unRetVal += ((CIEC_WSTRING&)pa_roCIECData).toUTF16(0, 0) + 3;
      break;
#endif
#ifdef FORTE_SUPPORT_ARRAYS
    case CIEC_ANY::e_ARRAY:
      unRetVal += 3;
      if(((CIEC_ARRAY&) pa_roCIECData).getElementDataTypeID() == CIEC_ANY::e_BOOL) {
        unRetVal += ((CIEC_ARRAY&)pa_roCIECData).size();
      } else {
        for(TForteUInt16 j = 0; j < ((CIEC_ARRAY&)pa_roCIECData).size(); ++j){
          unRetVal += getRequiredSerializationSize(*(((CIEC_ARRAY&)pa_roCIECData)[j]));
        }
        // First element with tag, subsequent ones without - adjust
        unRetVal -= ((CIEC_ARRAY&)pa_roCIECData).size() - 1;
      }
      break;
#endif
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      unRetVal += pa_roCIECData.getRequiredSerializationSize();

     break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

    default:
      unRetVal += csm_aDataTags[pa_roCIECData.getDataTypeID()][1];
      break;
  }
  return unRetVal;
}
