/*******************************************************************************
  * Copyright (c) 2007 - 2015 ACIN, nxtControl GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Ingo Hegny, Stansilav Meduna
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <fortenew.h>
#include "forte_array.h"
#include <stdlib.h>


#ifdef FORTE_SUPPORT_ARRAYS
DEFINE_FIRMWARE_DATATYPE(ARRAY, g_nStringIdARRAY)

CIEC_ARRAY::CIEC_ARRAY() {
}

CIEC_ARRAY::CIEC_ARRAY(TForteUInt16 pa_nLength, CStringDictionary::TStringId pa_unArrayType) {
  setup(pa_nLength, pa_unArrayType);
}

CIEC_ARRAY::CIEC_ARRAY(const CIEC_ARRAY& pa_roValue) :
    CIEC_ANY_DERIVED(){

  TForteInt16 nSize = pa_roValue.size();

  if(0 != nSize){
    setGenData((TForteByte *) forte_malloc(8 + (1 + pa_roValue.size()) * sizeof(CIEC_ANY)));
    if(0 != getGenData()){
      setSize(pa_roValue.size());
      TForteByte *acDataBuf = (TForteByte *) (getArray() - 1); // The first element is a reference element
      const CIEC_ANY *poBuf = pa_roValue.getArray() - 1;

      for(int i = 0; i <= nSize; ++i, ++poBuf){
        //as we acDataBuf is already the target place we don't need to store the resulting pointer
        poBuf->clone(acDataBuf); //clone is faster than the CTypeLib call
        acDataBuf += sizeof(CIEC_ANY);
      }
    }
  }

}

CIEC_ARRAY::~CIEC_ARRAY(){
  clear();
}

void CIEC_ARRAY::setup(TForteUInt16 pa_nLength, CStringDictionary::TStringId pa_unArrayType){
  if(0 != pa_nLength){
    if(0 != getGenData()){
      clear();
    }

    // The first element is a reference element, that is used
    // - to initialize the elements not set by fromString or deserialize
    // - to get the element id, even if the array has a zero size (not enabled yet, open to discussion)

    setGenData((TForteByte *) forte_malloc(8 + (1 + pa_nLength) * sizeof(CIEC_ANY)));
    setSize(pa_nLength);
    TForteByte *acDataBuf = (TForteByte *) (getArray() - 1); // The first element is a reference element
    CIEC_ANY *poBuf = CTypeLib::createDataTypeInstance(pa_unArrayType, acDataBuf);

    for(unsigned int i = 0; i < pa_nLength; ++i){
      acDataBuf += sizeof(CIEC_ANY);
      //as we acDataBuf is already the target place we don't need to store the resulting pointer
      poBuf->clone(acDataBuf); //clone is faster than the CTypeLib call
    }
  }
}

void CIEC_ARRAY::setValue(const CIEC_ANY& pa_roValue){
  if(pa_roValue.getDataTypeID() == e_ARRAY){
    //TODO maybe check if array data is of same type or castable
    const CIEC_ANY *poSrcArray = static_cast<const CIEC_ARRAY &>(pa_roValue).getArray();

    TForteUInt16 unSize = (size() < static_cast<const CIEC_ARRAY&>(pa_roValue).size()) ? size() : static_cast<const CIEC_ARRAY&>(pa_roValue).size();

    for(TForteUInt16 i = 0; i < unSize; ++i, ++poSrcArray){
      (*this)[i]->setValue(*poSrcArray);
    }
  }
}

void CIEC_ARRAY::clear(){
  if(size()){
    CIEC_ANY *poArray = getArray();
    for(unsigned int i = 0; i < size(); ++i, ++poArray){
      poArray->~CIEC_ANY();
    }

    forte_free(getGenData());
    setGenData(0);
  }
}

//TODO: for full compliance support of multiple element-statements must be implemented
int CIEC_ARRAY::fromString(const char *pa_pacValue){
  int nRetVal = -1;
  const char *pcRunner = pa_pacValue;
  if('[' == pa_pacValue[0]){

    CIEC_ANY *poBufVal = 0;
    TForteUInt16 i = 0;
    TForteUInt16 unArraySize = size();
    int nValueLen;

    while((*pcRunner != '\0') && (*pcRunner != ']')){
      pcRunner++;
      if(i < unArraySize){
        nValueLen = (*this)[i]->fromString(pcRunner);
      }
      else{
        if(0 == poBufVal)
          poBufVal = (getArray() - 1)->clone(0);
        nValueLen = poBufVal->fromString(pcRunner);
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
      nRetVal = static_cast<int>(pcRunner - pa_pacValue + 1); //+1 from the closing bracket
      // For the rest of the array size copy the default element
      for(; i < unArraySize; ++i){
        (*this)[i]->setValue(*(&(getArray()[-1])));
      }
    }
    delete poBufVal;
  }
  return nRetVal;
}

int CIEC_ARRAY::toString(char* pa_acValue, unsigned int pa_nBufferSize) const{
  int nBytesUsed = -1;

  if(pa_nBufferSize){
    *pa_acValue = '[';
    pa_acValue++;
    pa_nBufferSize--;
    nBytesUsed = 1;
    int nUsedBytesByElement;
    TForteUInt16 unSize = size();
    const CIEC_ANY *poArray = getArray();
    for(unsigned int i = 0; i < unSize; ++i, ++poArray){
      nUsedBytesByElement = poArray->toString(pa_acValue, pa_nBufferSize);
      if(-1 == nUsedBytesByElement){
        return -1;
      }
      pa_nBufferSize -= nUsedBytesByElement;
      pa_acValue += nUsedBytesByElement;
      if(!pa_nBufferSize){
        return -1;
      }

      nBytesUsed += nUsedBytesByElement;

      if(i != static_cast<unsigned int>(unSize - 1)){
        *pa_acValue = ',';
        pa_acValue++;

        ++nBytesUsed;
        pa_nBufferSize--;
      }
    }
    if(pa_nBufferSize < 2){
      return -1;
    }
    *pa_acValue = ']';
    pa_acValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

#endif /* FORTE_SUPPORT_ARRAYS */
