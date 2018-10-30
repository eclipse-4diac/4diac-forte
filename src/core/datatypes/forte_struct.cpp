/*******************************************************************************
  * Copyright (c) 2010 - 2015 ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Ingo Hegny, Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <fortenew.h>
#include "forte_struct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_struct_gen.cpp"
#endif
#include <stdlib.h>
#ifdef FORTE_SUPPORT_ARRAYS
# include "forte_array.h"
#endif

CIEC_STRUCT::CIEC_STRUCT(CStringDictionary::TStringId paTypeName, TForteUInt16 paLength, const CStringDictionary::TStringId paElementTypes[],
    const CStringDictionary::TStringId paElementNames[], TForteUInt8 paTypeID, TStructInitialValues *paInitialValues) {
  setup(paTypeName, paLength, paElementTypes, paElementNames, paTypeID, paInitialValues);
}

CIEC_STRUCT::CIEC_STRUCT(const CIEC_STRUCT& paValue) :
    CIEC_ANY_DERIVED() {
  if(0 != paValue.getStructSize()) {
    setGenData(
      reinterpret_cast<TForteByte*>(new CSTRUCT_SPECS(paValue.getStructTypeNameID(), paValue.getStructSize(), paValue.elementNames(),
        (paValue.getASN1StructType()))));

    TForteByte *localMembers = reinterpret_cast<TForteByte *>(getMembers());
    const CIEC_ANY* poSrcBuf = paValue.getMembers();

    for(size_t i = 0; i < getStructSize(); ++i, ++poSrcBuf) {
      poSrcBuf->clone(localMembers); //clone is faster than the CTypeLib call
      localMembers += sizeof(CIEC_ANY);
    }
  }
}

CIEC_STRUCT::~CIEC_STRUCT(){
  clear();
}

void CIEC_STRUCT::setup(CStringDictionary::TStringId paTypeName, TForteUInt16 paLength, const CStringDictionary::TStringId paElementTypes[],
    const CStringDictionary::TStringId paElementNames[], TForteUInt8 paTypeID, TStructInitialValues *paInitialValues) {
  if(0 != paLength){
    if(0 != getGenData()){
      clear();
    }
    setGenData(reinterpret_cast<TForteByte*>(new CSTRUCT_SPECS(paTypeName, paLength, paElementNames, paTypeID)));
    TForteByte *acDataBuf = reinterpret_cast<TForteByte *>(getMembers());

    for(size_t i = 0; i < paLength; ++i) {
      CIEC_ANY *retVal = CTypeLib::createDataTypeInstance(*paElementTypes, acDataBuf);
      ++paElementTypes;
#ifdef FORTE_SUPPORT_ARRAYS
      if(g_nStringIdARRAY == paElementTypes[-1]){
        if(0 != retVal){
          //For an array we have to do more
          (static_cast<CIEC_ARRAY *>(retVal))->setup(static_cast<TForteUInt16>(*paElementTypes), paElementTypes[1]);
        }
        paElementTypes += 2;
      }
#endif
      acDataBuf += sizeof(CIEC_ANY);
      if(paInitialValues && paInitialValues[i]) {
        retVal->fromString(paInitialValues[i]);
      }
    }
  }
}

void CIEC_STRUCT::setValue(const CIEC_ANY& paValue){
  if(paValue.getDataTypeID() == e_STRUCT && (getStructTypeNameID() == static_cast<const CIEC_STRUCT&>(paValue).getStructTypeNameID())){
    CIEC_ANY *poDstBuf = getMembers();
    const CIEC_ANY* poSrcBuf = static_cast<const CIEC_STRUCT&>(paValue).getMembers();
    for(size_t i = 0; i < getStructSize(); ++i, ++poSrcBuf, ++poDstBuf) {
      poDstBuf->setValue(*poSrcBuf);
    }
  }
}

void CIEC_STRUCT::clear(){
  if(getASN1StructType()){
    CIEC_ANY *poBuf = getMembers();

    for(TForteUInt8 i = 0; i < getStructSize(); ++i, ++poBuf){
      poBuf->~CIEC_ANY();
    }

    delete getGenData();
    setGenData(0);
  }
}

int CIEC_STRUCT::fromString(const char *paValue){
  int nRetVal = -1;

  if('(' == paValue[0]){
    const char *pcRunner = paValue;
    bool errorOcurred = false;
    pcRunner++;

    while(('\0' != *pcRunner) && (')' != *pcRunner) && !errorOcurred) {

      //first extract the element name
      int nLength = 0;
      CStringDictionary::TStringId unElementNameId = parseNextElementId(pcRunner, nLength);

      if(CStringDictionary::scm_nInvalidStringId != unElementNameId) {
        pcRunner += nLength;

        CIEC_ANY *poMember = getMemberNamed(unElementNameId);
        if(0 != poMember) {
          nLength = poMember->fromString(pcRunner);

          if(0 < nLength) {
            pcRunner += nLength;

            findNextNonBlankSpace(&pcRunner);

            if(',' != *pcRunner) {
              break;
            }
          } else {
            //we have an error or the end bracket
            errorOcurred = true;
          }
        } else {
          errorOcurred = true;
        }
      }else{
        errorOcurred = true;
      }
      pcRunner++;
    }

    if(')' == *pcRunner && !errorOcurred) {
      //structs have to have an opening and a closing round bracket
      nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing round bracket
    }
  }
  return nRetVal;
}

CStringDictionary::TStringId CIEC_STRUCT::parseNextElementId(const char *paRunner, int &paCounter){
  CStringDictionary::TStringId unElementNameId = CStringDictionary::scm_nInvalidStringId;
  TIdentifier acIdentifier;

  //remove any leading whitespaces before the identifier
  while(' ' == *paRunner){
    paRunner++;
    paCounter++;
  }

  for(size_t i = 0; (('\0' != *paRunner) && (')' != *paRunner)); ++i, ++paCounter, ++paRunner) {
    if(i >= cg_nIdentifierLength){
      //currently we only allow identifiers smaller that size
      return CStringDictionary::scm_nInvalidStringId;
    }

    if((':' == *paRunner) || (' ' == *paRunner)){
      acIdentifier[i] = '\0'; //close identifier string
      break;
    }
    acIdentifier[i] = *paRunner;
  }
  //remove any whitespaces between identifier and assignment operator
  while(' ' == *paRunner){
    paRunner++;
    paCounter++;
  }

  if(':' == *paRunner){
    //only if we have a : we have a correct identifier
    paRunner++;
    paCounter++;
    if('=' == *paRunner){
      unElementNameId = CStringDictionary::getInstance().getId(acIdentifier);
      paRunner++;
      paCounter++;
      //remove any whitespaces between assignment operator and value
      while(' ' == *paRunner){
        paRunner++;
        paCounter++;
      }
      if(('\0' == *paRunner) || (')' == *paRunner)){
        return CStringDictionary::scm_nInvalidStringId;
      }
    }
  }

  return unElementNameId;
}

int CIEC_STRUCT::toString(char* paValue, unsigned int paBufferSize) const{
  int nBytesUsed = -1;

  if(paBufferSize) {
    *paValue = '(';
    paValue++;
    paBufferSize--;
    nBytesUsed = 1;
    TForteUInt16 unSize = getStructSize();
    const CStringDictionary::TStringId *punMemberNameIds = elementNames();
    const CIEC_ANY *poMembers = getMembers();

    for(size_t i = 0; i < unSize; ++i, ++poMembers, ++punMemberNameIds) {
      const char *acMemberName = CStringDictionary::getInstance().get(*punMemberNameIds);
      int nUsedBytesByElement;
      if(strlen(acMemberName) + 2 > paBufferSize){
        return -1;
      }

      strncpy(paValue, acMemberName, strlen(acMemberName));
      paValue += strlen(acMemberName);
      *paValue = ':';
      paValue++;
      *paValue = '=';
      paValue++;
      paBufferSize -= static_cast<unsigned int>(strlen(acMemberName) + 2);
      nBytesUsed += static_cast<int>(strlen(acMemberName) + 2);

      nUsedBytesByElement = poMembers->toString(paValue, paBufferSize);
      if(-1 == nUsedBytesByElement){
        return -1;
      }
      paBufferSize -= nUsedBytesByElement;
      paValue += nUsedBytesByElement;
      if(!paBufferSize){
        return -1;
      }
      nBytesUsed += nUsedBytesByElement;

      if(i != static_cast<size_t>(unSize - 1)) {
        *paValue = ',';
        paValue++;

        ++nBytesUsed;
        paBufferSize--;
      }
    }
    if(paBufferSize < 2){
      return -1;
    }
    *paValue = ')';
    paValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

CIEC_ANY *CIEC_STRUCT::getMemberNamed(CStringDictionary::TStringId paMemberNameId){
  CIEC_ANY *poRetVal = 0;

  CIEC_ANY *poMembers = getMembers();
  const CStringDictionary::TStringId *punMemberNameIds = elementNames();

  for(size_t i = 0; i < getStructSize(); ++i, ++poMembers, ++punMemberNameIds) {
    if(*punMemberNameIds == paMemberNameId){
      poRetVal = poMembers;
      break;
    }
  }
  return poRetVal;
}

unsigned int CIEC_STRUCT::getToStringBufferSize() const {
  unsigned int retVal = 3; // 2 bytes for the open and closing brackets one for the '\0'
  TForteUInt16 nSize = getStructSize();
  retVal += (nSize > 1) ? (nSize - 1) : 0; //for the commas between the elements
  retVal += (nSize * 2); //for the := of each element
  const CIEC_ANY *poMembers = getMembers();
  if(0 != poMembers) {
    for(size_t i = 0; i < nSize; i++) {
      retVal += strlen(CStringDictionary::getInstance().get(elementNames()[i])); //element name
      retVal += (poMembers[i].getToStringBufferSize() - 1); //length of the element itself. -1 for the included \0 in each element
    }
  }
  return retVal;
}

void CIEC_STRUCT::findNextNonBlankSpace(const char** paRunner) {
  while(' ' == **paRunner) {
    (*paRunner)++;
  }
}
