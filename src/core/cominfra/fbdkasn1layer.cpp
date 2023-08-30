/*******************************************************************************
 * Copyright (c) 2010-2013 TU Wien ACIN, Profactor GmbH, nxtControl GmbH
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#include "fbdkasn1layer.h"
#include "basecommfb.h"
#include "../../arch/timerha.h"
#include "../../arch/devlog.h"
#include <fortenew.h>

using namespace forte::com_infra;

const std::set<CIEC_ANY::EDataTypeID> CFBDKASN1ComLayer::scmSimpleEncodableDataTypes{
    CIEC_ANY::e_SINT,
    CIEC_ANY::e_INT,
    CIEC_ANY::e_DINT,
    CIEC_ANY::e_LINT,
    CIEC_ANY::e_USINT,
    CIEC_ANY::e_UINT,
    CIEC_ANY::e_UDINT,
    CIEC_ANY::e_ULINT,
    CIEC_ANY::e_BYTE,
    CIEC_ANY::e_WORD,
    CIEC_ANY::e_DWORD,
    CIEC_ANY::e_LWORD,
    CIEC_ANY::e_DATE,
    CIEC_ANY::e_TIME_OF_DAY,
    CIEC_ANY::e_DATE_AND_TIME,
    CIEC_ANY::e_CHAR,
    CIEC_ANY::e_WCHAR,
    CIEC_ANY::e_LDATE,
    CIEC_ANY::e_LTIME_OF_DAY,
    CIEC_ANY::e_LDATE_AND_TIME,
    CIEC_ANY::e_REAL,
    CIEC_ANY::e_LREAL};

CFBDKASN1ComLayer::CFBDKASN1ComLayer(CComLayer* paUpperLayer, CBaseCommFB * paComFB) :
  CComLayer(paUpperLayer, paComFB), mStatSerBuf(nullptr), mStatSerBufSize(0), mDeserBuf(nullptr), mDeserBufSize(0), mDeserBufPos(0), mDIPos(0), mDOPos(0){

  if(nullptr != paComFB){
    TPortId sdNum = paComFB->getNumSD();
    CIEC_ANY **apoSDs = paComFB->getSDs();
    for(TPortId i = 0; i < sdNum; ++i){
      if(apoSDs[i] != nullptr){
        TForteByte typeSize = csmDataTags[apoSDs[i]->unwrap().getDataTypeID()][1];
        if(typeSize != 255){
          mStatSerBufSize += typeSize + 1;
        }
      }
    }

    TPortId rdNum = paComFB->getNumRD();
    CIEC_ANY **apoRDs = paComFB->getRDs();
    for(TPortId i = 0; i < rdNum; ++i){
      if(nullptr != apoRDs[i]){
        TForteByte typeSize = csmDataTags[apoRDs[i]->unwrap().getDataTypeID()][1];
        if(typeSize != 255){
          mDeserBufSize += typeSize + 1;
        }
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

EComResponse CFBDKASN1ComLayer::sendData(void *paData, unsigned int paSize){
  EComResponse eRetVal = e_ProcessDataNoSocket;

  if(mBottomLayer != nullptr){
    TConstIEC_ANYPtr *apoSDs = static_cast<TConstIEC_ANYPtr *> (paData);
    size_t unNeededBufferSize = 0;

    if(nullptr == apoSDs){
       return e_ProcessDataDataTypeError;
     }

    for(size_t i = 0; i < paSize; ++i){
      unNeededBufferSize += getRequiredSerializationSize(*apoSDs[i]);
    }
    TForteByte *paUsedBuffer = nullptr;
    TForteByte *paDynSerBuffer = nullptr;
    //FIXME grow buffer instead of allocating every time a new one
    if(unNeededBufferSize <= mStatSerBufSize){
      paUsedBuffer = mStatSerBuf;
    }
    else{
       paDynSerBuffer = new TForteByte[unNeededBufferSize];
       paUsedBuffer = paDynSerBuffer;
    }

    int ser_size = -1; //stays negative if no Buffer for serialization is provided or error occurs while serialization

    if (nullptr != paUsedBuffer) {
      ser_size = serializeDataPointArray(paUsedBuffer, unNeededBufferSize, apoSDs, paSize);
    }

    if(ser_size > 0){
      eRetVal = mBottomLayer->sendData(paUsedBuffer, ser_size);
    }
    else{ // serialize failed
      eRetVal = e_ProcessDataDataTypeError;
      DEVLOG_ERROR("CAsn1Layer:: serializeData failed\n");
    }

    if(nullptr != paDynSerBuffer){
      delete[] paDynSerBuffer;
    }
  }

  return eRetVal;
}

EComResponse CFBDKASN1ComLayer::recvData(const void *paData, unsigned int paSize){
  TForteByte *receivedData = const_cast<TForteByte*>(static_cast<const TForteByte *>(paData));
  EComResponse eRetVal = e_Nothing;

  if(mFb != nullptr){
    CIEC_ANY **apoRDs = mFb->getRDs();
    TPortId unNumRD = mFb->getNumRD();
    unsigned int usedBufferSize = 0;
    TForteByte *usedBuffer = nullptr;

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
        if(nullptr == apoRDs){
          DEVLOG_ERROR("Data type error\n");
          eRetVal = e_ProcessDataDataTypeError;
          break;
        }
        nBuf = deserializeDataPoint(usedBuffer, usedBufferSize, *apoRDs[mDOPos]);
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

/*Initialization csmDataTags array
 *
 */
const TForteByte CFBDKASN1ComLayer::csmDataTags[][2] = {
    //!< {Tag, Size of data + tag size}; Size == 255 means unknown
    // TODO: consider size=0 for unknown
    {e_APPLICATION + e_PRIMITIVE + e_ANY_TAG, 255},
    {e_APPLICATION + e_PRIMITIVE + e_BOOL_TAG, 1},
    {e_APPLICATION + e_PRIMITIVE + e_SINT_TAG, 2},
    {e_APPLICATION + e_PRIMITIVE + e_INT_TAG, 3},
    {e_APPLICATION + e_PRIMITIVE + e_DINT_TAG, 5},
    {e_APPLICATION + e_PRIMITIVE + e_LINT_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_USINT_TAG, 2},
    {e_APPLICATION + e_PRIMITIVE + e_UINT_TAG, 3},
    {e_APPLICATION + e_PRIMITIVE + e_UDINT_TAG, 5},
    {e_APPLICATION + e_PRIMITIVE + e_ULINT_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_BYTE_TAG, 2},
    {e_APPLICATION + e_PRIMITIVE + e_WORD_TAG, 3},
    {e_APPLICATION + e_PRIMITIVE + e_DWORD_TAG, 5},
    {e_APPLICATION + e_PRIMITIVE + e_LWORD_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_DATE_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_TIME_OF_DAY_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_DATE_AND_TIME_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_TIME_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_CHAR_TAG, 2},
    {e_APPLICATION + e_PRIMITIVE + e_WCHAR_TAG, 3},
    {e_APPLICATION + e_PRIMITIVE + e_LDATE_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_LTIME_OF_DAY_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_LDATE_AND_TIME_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_LTIME_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_REAL_TAG, 5},
    {e_APPLICATION + e_PRIMITIVE + e_LREAL_TAG, 9},
    {e_APPLICATION + e_PRIMITIVE + e_STRING_TAG, 255},
    {e_APPLICATION + e_PRIMITIVE + e_WSTRING_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_DerivedData_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_DirectlyDerivedData_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_EnumeratedData_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_SubrangeData_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_ARRAY_TAG, 255},
    {e_APPLICATION + e_CONSTRUCTED + e_STRUCT_TAG, 255}};

int CFBDKASN1ComLayer::serializeDataPointArray(TForteByte *paBytes, const size_t paStreamSize, TConstIEC_ANYPtr *paData, size_t paDataNum){
  int nRetVal = -1;
  if(0 == paDataNum){
    serializeNull(paBytes);
    nRetVal = 1;
  }
  else {
    if (paStreamSize > std::numeric_limits<int>::max()) {
      DEVLOG_ERROR("FBDK ASN1 Layer: paStreamSize too big!\n");
      return -1;
    }
    int nRemainingBytes = static_cast<int>(paStreamSize);
    int nBuf;
    nRetVal = 0;
    for(size_t i = 0; i < paDataNum; i++){
      if(nullptr != (paData[i])){
        nBuf = serializeDataPoint(paBytes, nRemainingBytes, *paData[i]);
        if(0 < nBuf){
          nRetVal += nBuf;
          nRemainingBytes -= nBuf;
          paBytes += nBuf;
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

int CFBDKASN1ComLayer::serializeDataPoint(TForteByte* paBytes, int paStreamSize, const CIEC_ANY &paCIECData){
  int nRetVal = -1;
  if(0 < paStreamSize){
    serializeTag(paBytes, paCIECData);
    nRetVal = serializeValue(paBytes + 1, paStreamSize - 1, paCIECData);
    if(0 <= nRetVal){
      ++nRetVal;        //add one for the tag
    }
  }
  return nRetVal;
}

void CFBDKASN1ComLayer::serializeTag(TForteByte* paBytes, const CIEC_ANY &paCIECData){
  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();

  switch(eDataType) {
    case CIEC_ANY::e_ANY:
      serializeTag(paBytes, paCIECData.unwrap());
      break;
    case CIEC_ANY::e_STRUCT:
      //get ASN1-tag from implementing datatype for STRUCT-datatypes
      paBytes[0] = ((CIEC_STRUCT&) paCIECData).getASN1StructType();
      break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      paBytes[0] = paCIECData.getTag();
      break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */
    default:
      paBytes[0] = csmDataTags[eDataType][0];
      break;
  }
}

int CFBDKASN1ComLayer::serializeValue(TForteByte* paBytes, int paStreamSize, const CIEC_ANY &paCIECData){
  int nRetVal = -1;

  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();

  if((CIEC_ANY::e_BOOL < eDataType && eDataType <= CIEC_ANY::e_DATE_AND_TIME)||(CIEC_ANY::e_REAL == eDataType)||(CIEC_ANY::e_LREAL == eDataType)){
    //Simple data types except bool can be handled the same way
    nRetVal = serializeValueSimpleDataType(paBytes, paStreamSize, paCIECData);
  }
  else{
    switch (eDataType){
      case CIEC_ANY::e_ANY:
        nRetVal = serializeValue(paBytes, paStreamSize, paCIECData.unwrap());
        break;
      case CIEC_ANY::e_BOOL:
        if(!((CIEC_BOOL &) paCIECData).operator bool()){
          //data of bool is encoded in the tag; if CIEC_BOOL == false => Tag must be e_APPLICATION + e_PRIMITIVE
          paBytes[-1] = static_cast<TForteByte>(paBytes[-1] - static_cast<TForteByte>(e_BOOL_TAG));
        }
        nRetVal = 0;
        break;
      case CIEC_ANY::e_TIME:
        nRetVal = serializeValueTime(paBytes, paStreamSize, static_cast<const CIEC_TIME &>(paCIECData));
        break;
#ifdef FORTE_USE_WSTRING_DATATYPE
      case CIEC_ANY::e_WSTRING:
        nRetVal = serializeValueWString(paBytes, paStreamSize, static_cast<const CIEC_WSTRING &>(paCIECData));
        break;
#endif
      case CIEC_ANY::e_STRING:
        nRetVal = serializeValueString(paBytes, paStreamSize, static_cast<const CIEC_STRING &>(paCIECData));
        break;
      case CIEC_ANY::e_ARRAY:
        nRetVal = serializeArray(paBytes, paStreamSize, static_cast<const CIEC_ARRAY &>(paCIECData));
        break;
      case CIEC_ANY::e_STRUCT:
        nRetVal = serializeValueStruct(paBytes, paStreamSize, static_cast<const CIEC_STRUCT &>(paCIECData));
        break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
      case CIEC_ANY::e_External:
        nRetVal = paCIECData.serializeCustomType(paBytes, paStreamSize);
        break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

      default:
        break;
    }
  }

  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueSimpleDataType(TForteByte* paBytes, int paStreamSize, const CIEC_ANY & paDataPoint){
  int nRetVal = csmDataTags[paDataPoint.getDataTypeID()][1];
  --nRetVal; //Length of the tag

  if(nRetVal <= paStreamSize){
    const TForteByte* acDataPtr = paDataPoint.getConstDataPtr();

#ifdef FORTE_LITTLE_ENDIAN
# if defined(__ARMEL__) && ! defined(__VFP_FP__) // Little endian ARM with old mixed endian FPA float ABI needs to swap
        if(CIEC_ANY::e_LREAL == paDataPoint.getDataTypeID()) {
          TForteUInt32 anSwapped[2];
          anSwapped[0] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[1];
          anSwapped[1] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[0];
          acDataPtr = reinterpret_cast<const TForteByte*>(&anSwapped[0]);
        }
# endif //defined(__ARMEL__) && ! defined(__VFP_FP__)
      for(int i = 0; i < nRetVal; i++){
        paBytes[(nRetVal - 1) - i] = acDataPtr[i];
      }
#endif //FORTE_LITTLE_ENDIAN

#ifdef FORTE_BIG_ENDIAN
      if(CIEC_ANY::e_REAL != paDataPoint.getDataTypeID()){
        for (int i = 0; i < nRetVal; i++){
          paBytes[(nRetVal - 1) - i] = acDataPtr[(sizeof(CIEC_ANY::TLargestUIntValueType) - 1)-i];
        }
      }
      else{
        for (unsigned int i = 0; i < nRetVal; i++){
          paBytes[(nRetVal - 1) - i] = acDataPtr[(sizeof(TForteFloat) - 1) - i];
        }
      }
#endif //FORTE_BIG_ENDIAN
  }
  else{
    nRetVal = -1;
  }

  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueTime(TForteByte* paBytes, int paStreamSize, const CIEC_TIME & paTime){
  int nRetVal = -1;
  if(8 <= paStreamSize){
    TForteInt64 timeInMicroSeconds = paTime.getInMicroSeconds();
    paBytes[0] = (TForteByte) (timeInMicroSeconds >> 56) & 0xFF;
    paBytes[1] = (TForteByte) (timeInMicroSeconds >> 48) & 0xFF;
    paBytes[2] = (TForteByte) (timeInMicroSeconds >> 40) & 0xFF;
    paBytes[3] = (TForteByte) (timeInMicroSeconds >> 32) & 0xFF;
    paBytes[4] = (TForteByte) (timeInMicroSeconds >> 24) & 0xFF;
    paBytes[5] = (TForteByte) (timeInMicroSeconds >> 16) & 0xFF;
    paBytes[6] = (TForteByte) (timeInMicroSeconds >> 8) & 0xFF;
    paBytes[7] = (TForteByte) (timeInMicroSeconds) & 0xFF;
    nRetVal = 8;
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueString(TForteByte* paBytes, int paStreamSize, const CIEC_STRING & paString){
  int nRetVal = -1;
  TForteUInt16 unStringLen = paString.length();
  if(unStringLen + 2 <= paStreamSize){
    paBytes[0] = (TForteByte) ((unStringLen >> 8) & 0x00FF);
    paBytes[1] = (TForteByte) (unStringLen & 0x00FF);
    paBytes += 2;
    memcpy(paBytes, paString.getStorage().c_str(), unStringLen);
    nRetVal = unStringLen + 2;
  }
  return nRetVal;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
int CFBDKASN1ComLayer::serializeValueWString(TForteByte* paBytes, int paStreamSize, const CIEC_WSTRING & paWString){
  int nRetVal = -1;

  if (paStreamSize < 2) {
    return -1;
  }
  nRetVal = paWString.toUTF16(paBytes + 2, paStreamSize - 2);
  if (nRetVal < 0 || nRetVal > 131070) {
    return -1;
  }
  paBytes[0] = (TForteByte) ((nRetVal >> 9) & 0x00FF);
  paBytes[1] = (TForteByte) ((nRetVal >> 1) & 0x00FF);
  nRetVal += 2;

  return nRetVal;
}
#endif

int CFBDKASN1ComLayer::serializeArray(TForteByte *paBytes, int paStreamSize, const CIEC_ARRAY &paArray) {
  int nRetVal = -1;
  size_t nArraySize = paArray.size();

  //Number of array elements
  paBytes[0] = (TForteByte) ((nArraySize >> 8) & 0x00FF);
  paBytes[1] = (TForteByte) (nArraySize & 0x00FF);

  paBytes += 2;
  //TODO should we check if the array has size zero?


  if( CIEC_ANY::e_BOOL == paArray[0].getDataTypeID()){
    //bool arrays are special
    nRetVal = 2; // array len
    paStreamSize -= nRetVal;
    for(TForteUInt16 i = 0; i < nArraySize; i++){ //serialize elements
      int nSerSize = serializeDataPoint(paBytes, paStreamSize, paArray[i]);
      if(0 < nSerSize){
        nRetVal += nSerSize;
        paBytes += nSerSize;
        paStreamSize -= nSerSize;
      }
      else{
        nRetVal = -1;
        break;
      }
    }
  }
  else{
    serializeTag(paBytes, paArray[0]);

    nRetVal = 2 + 1; // array len + contained data tag
    paStreamSize -= nRetVal;

    ++paBytes;
    for(TForteUInt16 i = 0; i < nArraySize; i++){ //serialize elements
      int nSerSize = serializeValue(paBytes, paStreamSize, paArray[i]);
      if(-1 == nSerSize){
        nRetVal = -1;
        break;
      }
      nRetVal += nSerSize;
      paStreamSize -= nSerSize;
      paBytes += nSerSize; //shift buffer array
    }

  }
  return nRetVal;
}

int CFBDKASN1ComLayer::serializeValueStruct(TForteByte* paBytes, int paStreamSize, const CIEC_STRUCT & paStruct) {
  int nStreamUsed = 0;
  int nTotalStreamUsed = 0;

  size_t structSize = paStruct.getStructSize();
  for (size_t i = 0; i < structSize; i++){
    const CIEC_ANY& ro_val = *paStruct.getMember(i);
    if (CIEC_ANY::e_BOOL == ro_val.getDataTypeID()) {
      //Handle BOOL-values differently, since value is encoded in tag
      nStreamUsed = serializeDataPoint(paBytes, paStreamSize, ro_val);
    }
    else {
      nStreamUsed = serializeValue(paBytes, paStreamSize, ro_val);
    }


    if(-1 == nStreamUsed) {
      nTotalStreamUsed = -1;
      break;
    }
    paStreamSize -= nStreamUsed;
    paBytes += nStreamUsed;
    nTotalStreamUsed += nStreamUsed;
  }
  return nTotalStreamUsed;
}

bool CFBDKASN1ComLayer::deserializeDataPointArray(const TForteByte* paBytes, unsigned int paStreamSize, TIEC_ANYPtr *paData, size_t paDataNum){
  bool bRetval = true;
  if(0 == paDataNum){
    bRetval = ((paStreamSize == 1) && (isNull(paBytes)));
  }
  else{
    for(size_t i = 0; i < paDataNum; ++i){
      if(nullptr == paData[i]){
        bRetval = false;
        break;
      }
      int nBuf = deserializeDataPoint(paBytes, paStreamSize, *paData[i]);
      if(nBuf <= 0){
        // we could not deserialize the data of we have a to small packet
        // with the compliance profile for feasibility demonstration's protocol we can not determine packet order
        // and therefore not handle
        bRetval = false;
        break;
      }
      paStreamSize -= nBuf;
      paBytes += nBuf;
    }
  }
  return bRetval;
}

int CFBDKASN1ComLayer::deserializeDataPoint(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paCIECData){
  int nRetVal = -1;
  if(0 < paStreamSize && e_APPLICATION == (paBytes[0] & e_APPLICATION)) {
    if(deserializeTag(*paBytes, paCIECData)) {
      nRetVal = deserializeValue(paBytes + 1, paStreamSize - 1, paCIECData) + 1;
    } else {
      DEVLOG_ERROR("Datatype error\n");
      nRetVal = -2;
    }
  }
  return nRetVal;
}

bool CFBDKASN1ComLayer::deserializeTag(const TForteByte paByte, CIEC_ANY &paCIECData){
  bool bRetVal;
  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();

  switch(eDataType){
    case CIEC_ANY::e_ANY:
      bRetVal = deserializeTag(paByte, paCIECData.unwrap());
      break;
    case CIEC_ANY::e_BOOL:
      bRetVal = (((e_APPLICATION + e_PRIMITIVE) == paByte) || ((e_APPLICATION + e_PRIMITIVE + CIEC_ANY::e_BOOL) == paByte));
      if(bRetVal){
        static_cast<CIEC_BOOL &>(paCIECData) = CIEC_BOOL((csmDataTags[CIEC_ANY::e_BOOL][0] == paByte));
      }
      break;
    case CIEC_ANY::e_STRUCT:
      bRetVal = (static_cast<CIEC_STRUCT &>(paCIECData).getASN1StructType() == paByte);
      break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      bRetVal = paCIECData.deserializeTag(paByte);
      break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

    default:
      bRetVal = (csmDataTags[eDataType][0] == paByte);
      break;
  }
  return bRetVal;
}

int CFBDKASN1ComLayer::deserializeValue(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paCIECData){
  int nRetVal = -1;
  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();

  if (scmSimpleEncodableDataTypes.find(eDataType) != scmSimpleEncodableDataTypes.end())
  { // e_SINT to e_DATE_AND_TIME can be handled the same way
    nRetVal = deserializeValueSimpleDataType(paBytes, paStreamSize, paCIECData);
  }
  else{
    switch (eDataType){
      case CIEC_ANY::e_ANY:
        nRetVal = deserializeValue(paBytes, paStreamSize, paCIECData.unwrap());
        break;
      case CIEC_ANY::e_BOOL:
        //bool data is decoded in the bool tag
        nRetVal = 0;
        break;
      case CIEC_ANY::e_TIME:
      case CIEC_ANY::e_LTIME:
        nRetVal = deserializeValueTime(paBytes, paStreamSize, static_cast<CIEC_TIME &>(paCIECData));
        break;
#ifdef FORTE_USE_WSTRING_DATATYPE
      case CIEC_ANY::e_WSTRING:
        nRetVal = deserializeValueWString(paBytes, paStreamSize, static_cast<CIEC_WSTRING &>(paCIECData));
        break;
#endif
      case CIEC_ANY::e_STRING:
        nRetVal = deserializeValueString(paBytes, paStreamSize, static_cast<CIEC_STRING &>(paCIECData));
        break;
      case CIEC_ANY::e_ARRAY:
        nRetVal = deserializeArray(paBytes, paStreamSize, static_cast<CIEC_ARRAY &>(paCIECData));
        break;
      case CIEC_ANY::e_STRUCT:
        nRetVal = deserializeValueStruct(paBytes, paStreamSize, static_cast<CIEC_STRUCT &>(paCIECData));
        break;

#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      nRetVal = paCIECData.deserializeCustomType(paBytes, paStreamSize);
      break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */
      default:
        break;
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueSimpleDataType(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paIECData){
  int nRetVal = -1;
  int nValueSize = csmDataTags[paIECData.getDataTypeID()][1] - 1;

  if(paStreamSize >= nValueSize){
    TForteByte *acDataPtr = paIECData.getDataPtr();

    //setting mAnyData to 0
    *((CIEC_ANY::TLargestUIntValueType *) acDataPtr) = 0;

    //we only need to check for SINT, INT, and DINT as LINT will fill all bytes
    if(paIECData.getDataTypeID() <= CIEC_ANY::e_DINT && (paBytes[0] & 0x80)) {
      //we received a negative number set all bits to true
      *((CIEC_ANY::TLargestIntValueType *) acDataPtr) = -1;
    }

#ifdef FORTE_LITTLE_ENDIAN
      for(int i = 0; i < nValueSize; i++){
        acDataPtr[i] = paBytes[(nValueSize - 1) - i];
      }
# if defined(__ARMEL__) && ! defined(__VFP_FP__) // Little endian ARM with old mixed endian FPA float ABI needs to swap
        if(CIEC_ANY::e_LREAL == paIECData.getDataTypeID()) {
          TForteUInt32 nTmp = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[1];
          ((TForteUInt32 *) acDataPtr)[1] = reinterpret_cast<const TForteUInt32 *>(acDataPtr)[0];
          reinterpret_cast<TForteUInt32 *>(acDataPtr)[0] = nTmp;
        }
# endif //defined(__ARMEL__) && ! defined(__VFP_FP__)
#endif //FORTE_LITTLE_ENDIAN
#ifdef FORTE_BIG_ENDIAN
      if(CIEC_ANY:: e_REAL != paIECData.getDataTypeID()){
        for (unsigned int i=0; i < nValueSize;i++){
          acDataPtr[(sizeof(CIEC_ANY::TLargestUIntValueType) - 1) - i] = paBytes[(nValueSize - 1) - i];
        }
      }
      else{ //FLOAT must be processed this way, because it is always saved in the first 4 bytes
        for (unsigned int i = 0; i < nValueSize; i++){
          acDataPtr[(sizeof(TForteFloat) - 1) - i] = paBytes[(nValueSize - 1) - i];
        }
      }
#endif //FORTE_BIG_ENDIAN
    nRetVal = nValueSize;
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueTime(const TForteByte* paBytes, int paStreamSize, CIEC_TIME &paIECData){
  int nRetVal = -1;
  if(paStreamSize >= 8){
    TForteInt64 timeInMicroSeconds = ((((TForteInt64) paBytes[0]) << 56) & 0xFF00000000000000LL) + ((((TForteInt64) paBytes[1]) << 48) & 0x00FF000000000000LL) + ((((TForteInt64) paBytes[2]) << 40) & 0x0000FF0000000000LL) + ((((TForteInt64) paBytes[3]) << 32) & 0x000000FF00000000LL) + ((((TForteInt64) paBytes[4]) << 24) & 0x00000000FF000000) + ((((TForteInt64) paBytes[5]) << 16) & 0x0000000000FF0000) + ((((TForteInt64) paBytes[6]) << 8) & 0x000000000000FF00) + (((TForteInt64) paBytes[7]) & 0x00000000000000FF);
    paIECData.setFromMicroSeconds(timeInMicroSeconds);
    nRetVal = 8;
  }
  return nRetVal;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
int CFBDKASN1ComLayer::deserializeValueWString(const TForteByte* paBytes, int paStreamSize, CIEC_WSTRING &paIECData){
  int nRetVal = -1;
  if(paStreamSize >= 2){
    TForteUInt16 unStringSize = static_cast<TForteUInt16>(((static_cast<TForteUInt16>(paBytes[0]) << 8) & 0xFF00) + (static_cast<TForteUInt16>(paBytes[1]) & 0x00FF));
    if(paStreamSize >= (unStringSize * 2 + 2) && paIECData.fromUTF16(paBytes + 2, unStringSize*2)) {
      nRetVal = unStringSize * 2 + 2;
    }
  }
  return nRetVal;
}
#endif

int CFBDKASN1ComLayer::deserializeValueString(const TForteByte* paBytes, int paStreamSize, CIEC_STRING &paIECData){
  int nRetVal = -1;
  if(paStreamSize >= 2){
    TForteUInt16 unStringSize = static_cast<TForteUInt16>(((((TForteUInt16) paBytes[0]) << 8) & 0xFF00) + ((TForteUInt16) paBytes[1] & 0x00FF));
    if(paStreamSize >= (unStringSize + 2)){
      paIECData.assign((const char*)&(paBytes[2]), unStringSize);
      nRetVal = 2 + unStringSize;
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeArray(const TForteByte* paBytes, int paStreamSize, CIEC_ARRAY &paArray) {
  int nRetVal = -1;

  if(paStreamSize >= 2){
    TForteUInt16 nSize = static_cast<TForteUInt16>(((((TForteUInt16) paBytes[0]) << 8) & 0xFF00) + ((TForteUInt16) paBytes[1] & 0x00FF));

    //number of elements in ARRAY must be read from the incoming message
    nRetVal = 2;
    if(nSize > 0){
      int nValueLen;
      paBytes += 2;
      paStreamSize -= 2;

      //TODO do we need to check if the array's size is bigger than 0
      if(CIEC_ANY::e_BOOL == paArray[0].getDataTypeID()){
        //bool arrays are special
        nValueLen = deserializeValueBoolArray(paBytes, paStreamSize, paArray, nSize);
        if(0 <= nValueLen){
          nRetVal += nValueLen;
        }
        else{
          nRetVal = nValueLen;
        }
      }
      else{
        if(!deserializeTag(*paBytes, paArray[0])){
          return -1;
        }
        paBytes += 1;
        paStreamSize -= 1;
        ++nRetVal;
        CIEC_ANY *poBufVal = nullptr;
        size_t unArraySize = paArray.size();

        for(TForteUInt16 i = 0; i < nSize; ++i){
          if(i < unArraySize){
            nValueLen = deserializeValue(paBytes, paStreamSize, paArray[i]);
          }
          else{
            if(poBufVal == nullptr) {
              poBufVal = paArray[0].clone(nullptr);
            }
            nValueLen = deserializeValue(paBytes, paStreamSize, *poBufVal);
          }
          //size of the elements is given by the array datatype
          if(nValueLen <= 0) {
            if(poBufVal != nullptr) {
              delete poBufVal;
            }
            return nValueLen;
          }
          paStreamSize -= nValueLen;
          paBytes += nValueLen;
          nRetVal += nValueLen;
        }

        if (poBufVal != nullptr)
          delete poBufVal;
      }
    }
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueBoolArray(const TForteByte* paBytes, int paStreamSize, CIEC_ARRAY &paArray, TForteUInt16 paDecodedArraySize) {
  int nRetVal = 0;
  CIEC_BOOL oBoolVal;  //buffer value for handling to large input data

  size_t unArraySize = paArray.size();
  int nValueLen;

  for(TForteUInt16 i = 0; i < paDecodedArraySize; ++i){
    if(i < unArraySize){
      nValueLen = deserializeDataPoint(paBytes, paStreamSize, paArray[i]);
    }
    else{
      nValueLen = deserializeDataPoint(paBytes, paStreamSize, oBoolVal);
    }
    if(nValueLen <= 0){
      nRetVal = nValueLen;
      break;
    }
    paBytes += nValueLen;
    paStreamSize -= nValueLen;
    nRetVal += nValueLen;
  }
  return nRetVal;
}

int CFBDKASN1ComLayer::deserializeValueStruct(const TForteByte* paBytes, int paStreamSize, CIEC_STRUCT &paIECData){
  int nRetVal = 0;
  int nValueLen;

  size_t structSize = paIECData.getStructSize();
  for (size_t i = 0; i < structSize; i++){
    CIEC_ANY& ro_val = *paIECData.getMember(i);
    if (CIEC_ANY::e_BOOL == ro_val.getDataTypeID()) {
      //Hanlde BOOL-datatype differently since value is encoded in tag
      nValueLen = deserializeDataPoint(paBytes, paStreamSize, ro_val);
    }
    else {
      nValueLen = deserializeValue(paBytes, paStreamSize, ro_val);
    }
    if(nValueLen <= 0){
      nRetVal = nValueLen;
      break;
    }
    paStreamSize -= nValueLen;
    paBytes += nValueLen;
    nRetVal += nValueLen;
  }
  return nRetVal;
}


size_t CFBDKASN1ComLayer::getRequiredSerializationSize(const CIEC_ANY &paCIECData){
  size_t unRetVal = 0;

  switch (paCIECData.getDataTypeID()){
    case CIEC_ANY::e_ANY:
      unRetVal = getRequiredSerializationSize(paCIECData.unwrap());
      break;
    case CIEC_ANY::e_STRING:
      unRetVal += ((CIEC_STRING&)paCIECData).length() + 3;
      break;
#ifdef FORTE_USE_WSTRING_DATATYPE
    case CIEC_ANY::e_WSTRING:
      unRetVal += ((CIEC_WSTRING&)paCIECData).toUTF16(nullptr, 0) + 3;
      break;
#endif
    case CIEC_ANY::e_ARRAY:
      unRetVal += 3;
      if (((CIEC_ARRAY &)paCIECData).getElementDataTypeID() == CIEC_ANY::e_BOOL) {
        unRetVal += ((CIEC_ARRAY &)paCIECData).size();
      } else {
        for (TForteUInt16 j = 0; j < ((CIEC_ARRAY &)paCIECData).size(); ++j) {
          unRetVal += getRequiredSerializationSize((((CIEC_ARRAY &)paCIECData)[j]));
        }
        // First element with tag, subsequent ones without - adjust
        unRetVal -= ((CIEC_ARRAY &)paCIECData).size() - 1;
      }
      break;
#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    case CIEC_ANY::e_External:
      unRetVal += paCIECData.getRequiredSerializationSize();

     break;
#endif /* FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES */

    default:
      unRetVal += csmDataTags[paCIECData.getDataTypeID()][1];
      break;
  }
  return unRetVal;
}
