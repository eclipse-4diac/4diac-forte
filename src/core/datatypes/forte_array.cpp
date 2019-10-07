/*******************************************************************************
 * Copyright (c) 2007 - 2015 ACIN, nxtControl GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_array.h"
#include <stdlib.h>


#ifdef FORTE_SUPPORT_ARRAYS
DEFINE_FIRMWARE_DATATYPE(ARRAY, g_nStringIdARRAY)

CIEC_ARRAY::CIEC_ARRAY() {
}

CIEC_ARRAY::CIEC_ARRAY(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType) {
  setup(paLength, paArrayType);
}

CIEC_ARRAY::CIEC_ARRAY(const CIEC_ARRAY& paValue) :
    CIEC_ANY_DERIVED(){

  TForteInt16 nSize = paValue.size();

  if(0 != nSize){
    setGenData(reinterpret_cast<TForteByte*>(new CArraySpecs(nSize)));
    paValue.getReferenceElement()->clone(reinterpret_cast<TForteByte *>(getReferenceElement()));

    CIEC_ANY *destArray = getArray();
    const CIEC_ANY *srcArray = paValue.getArray();

    for(int i = 0; i < nSize; ++i) {
      //as we destArray is already the target place we don't need to store the resulting pointer
      srcArray[i].clone(reinterpret_cast<TForteByte *>(&(destArray[i]))); //clone is faster than the CTypeLib call
    }
  }

}

CIEC_ARRAY::~CIEC_ARRAY(){
  clear();
}

void CIEC_ARRAY::setup(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType){
  if(0 != paLength){
    clear();

    setGenData(reinterpret_cast<TForteByte*>(new CArraySpecs(paLength)));

    CIEC_ANY *destArray = getArray();

    // The reference element is used
    // - to initialize the elements not set by fromString or deserialize
    // - to get the element id, even if the array has a zero size (not enabled yet, open to discussion)
    CIEC_ANY *refElement = getReferenceElement();

    if(CTypeLib::createDataTypeInstance(paArrayType, reinterpret_cast<TForteByte *>(refElement))) {
      for(unsigned int i = 0; i < paLength; ++i) {
        //as we acDataBuf is already the target place we don't need to store the resulting pointer
        refElement->clone(reinterpret_cast<TForteByte *>(&(destArray[i]))); //clone is faster than the CTypeLib call
      }
    } else { //datatype not found, clear everything
      clear();
    }
  }
}

void CIEC_ARRAY::setValue(const CIEC_ANY& paValue){
  if(paValue.getDataTypeID() == e_ARRAY){
    //TODO maybe check if array data is of same type or castable
    const CIEC_ANY *poSrcArray = static_cast<const CIEC_ARRAY &>(paValue).getArray();

    TForteUInt16 unSize = (size() < static_cast<const CIEC_ARRAY&>(paValue).size()) ? size() : static_cast<const CIEC_ARRAY&>(paValue).size();

    for(TForteUInt16 i = 0; i < unSize; ++i, ++poSrcArray){
      (*this)[i]->setValue(*poSrcArray);
    }
  }
}

void CIEC_ARRAY::clear(){
  if(getGenData()) {
    delete getSpecs();
    setGenData(0);
  }
}

//TODO: for full compliance support of multiple element-statements must be implemented
int CIEC_ARRAY::fromString(const char *paValue){
  int nRetVal = -1;
  const char *pcRunner = paValue;
  if('[' == paValue[0]){

    CIEC_ANY *poBufVal = 0;
    TForteUInt16 i = 0;
    TForteUInt16 unArraySize = size();
    int nValueLen;

    while(('\0' != *pcRunner) && (']' != *pcRunner)) {
      pcRunner++;
      findNextNonBlankSpace(&pcRunner);

      initializeFromString(unArraySize, &nValueLen, i, pcRunner, &poBufVal);

      while(' ' == pcRunner[nValueLen]){
        nValueLen++;
      }
      if((0 < nValueLen) && ((',' == pcRunner[nValueLen]) || (']' == pcRunner[nValueLen]))){
        pcRunner += nValueLen;
      }
      else{
        //we have an error or the end bracket
        break;
      }
      i++;
    }
    if(*pcRunner == ']'){
      //arrays have to and on a closing bracket
      nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing bracket
      // For the rest of the array size copy the default element
      for(; i < unArraySize; ++i){
        (*this)[i]->setValue(*(getReferenceElement()));
      }
    }
    delete poBufVal;
  }
  return nRetVal;
}

void CIEC_ARRAY::initializeFromString(TForteUInt16 paArraySize, int* paValueLen, TForteUInt16 paPosition, const char* paSrcString, CIEC_ANY ** paBufVal) {
  if(paPosition < paArraySize) {
    *paValueLen = (*this)[paPosition]->fromString(paSrcString);
  } else {
    if(0 == *paBufVal) {
      *paBufVal = (getReferenceElement())->clone(0);
    }
    *paValueLen = (*paBufVal)->fromString(paSrcString);
  }
}

int CIEC_ARRAY::toString(char* paValue, size_t paBufferSize) const {
  int nBytesUsed = -1;

  if(paBufferSize) {
    *paValue = '[';
    paValue++;
    paBufferSize--;
    nBytesUsed = 1;
    TForteUInt16 unSize = size();
    const CIEC_ANY *poArray = getArray();
    for(unsigned int i = 0; i < unSize; ++i, ++poArray){
      int nUsedBytesByElement = poArray->toString(paValue, paBufferSize);
      if(-1 == nUsedBytesByElement){
        return -1;
      }
      paBufferSize -= nUsedBytesByElement;
      paValue += nUsedBytesByElement;
      if(!paBufferSize) {
        return -1;
      }

      nBytesUsed += nUsedBytesByElement;

      if(i != static_cast<unsigned int>(unSize - 1)){
        *paValue = ',';
        paValue++;

        ++nBytesUsed;
        paBufferSize--;
      }
    }
    if(paBufferSize < 2) {
      return -1;
    }
    *paValue = ']';
    paValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

size_t CIEC_ARRAY::getToStringBufferSize() const {
  size_t retVal = 3; // 2 bytes for the open and closing brackets and one for the '\0'
  TForteUInt16 nSize = size();
  retVal += (nSize > 1) ? (nSize - 1) : 0; //for the commas between the elements

  const CIEC_ANY* members = getArray();
  if(0 != members) {
    switch(getElementDataTypeID()){ //in these cases, the length of the elements are not always the same
      case CIEC_ANY::e_WSTRING:
      case CIEC_ANY::e_STRING: //quotes or double quotes are already counted in ANY_STRING
      case CIEC_ANY::e_ARRAY:
      case CIEC_ANY::e_STRUCT:
        for(size_t i = 0; i < nSize; i++) {
          retVal += (members[i].getToStringBufferSize() - 1); //-1 since the \0 of each element is counted as just one at the beginning
        }
        break;
      default:
        retVal += nSize * (getReferenceElement()->getToStringBufferSize() - 1); //-1 since the \0 of each element is counted as just one at the beginning
        break;
    }
  }

  return retVal;
}

void CIEC_ARRAY::findNextNonBlankSpace(const char** paRunner) {
  while(' ' == **paRunner) {
    (*paRunner)++;
  }
}

#endif /* FORTE_SUPPORT_ARRAYS */
