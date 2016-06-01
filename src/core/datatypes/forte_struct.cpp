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
#include <fortealloc.h>
#include "forte_struct.h"
#include <stdlib.h>
//#include <stdio.h>

CIEC_STRUCT::CIEC_STRUCT(CStringDictionary::TStringId pa_unTypeName, TForteUInt16 pa_nLength, const CStringDictionary::TStringId pa_unElementTypes[], const CStringDictionary::TStringId pa_unElementNames[], TForteUInt8 TypeID){
  setup(pa_unTypeName, pa_nLength, pa_unElementTypes, pa_unElementNames, TypeID);
//  printf("STRUCT() Type: %s\n",CStringDictionary::getInstance().get(getStructTypeNameID()));
}

CIEC_STRUCT::CIEC_STRUCT(const CIEC_STRUCT& pa_roValue) :
    CIEC_ANY_DERIVED(){
  //TODO: check if same datatype;
  if(0 != pa_roValue.getStructSize()){
    setGenData((TForteByte *) forte_malloc(
        scm_unMembersOffset +
        pa_roValue.getStructSize() * sizeof(CIEC_ANY))); //Data
    setStructSize(pa_roValue.getStructSize());
    setASN1StructType(pa_roValue.getASN1StructType());
    setElementNames((pa_roValue.elementNames()));
    setStructTypeNameID(pa_roValue.getStructTypeNameID());
  }

  TForteByte *acDataBuf = (TForteByte *) getMembers();
  if(0 != acDataBuf){
    const CIEC_ANY* poSrcBuf = pa_roValue.getMembers();

    for(unsigned int i = 0; i < getStructSize(); ++i, ++poSrcBuf){
      poSrcBuf->clone(acDataBuf); //clone is faster than the CTypeLib call
      acDataBuf += sizeof(CIEC_ANY);
    }
  }
  else{
//    printf("target of copy constructor is null-pointer!\n");
  }
}

CIEC_STRUCT::~CIEC_STRUCT(){
  clear();
}

void CIEC_STRUCT::setup(CStringDictionary::TStringId pa_unTypeName, TForteUInt16 pa_nLength, const CStringDictionary::TStringId pa_aunElementTypes[], const CStringDictionary::TStringId pa_aunElementNames[], TForteUInt8 pa_TypeID){
  if(0 != pa_nLength){
    if(0 != getGenData()){
      clear();
    }
    setGenData((TForteByte *) forte_malloc(
        scm_unMembersOffset +
        pa_nLength * sizeof(CIEC_ANY))); //Data (system-dependent)
    setASN1StructType(pa_TypeID);
    setStructSize(pa_nLength);
    setElementNames(pa_aunElementNames);
    setStructTypeNameID(pa_unTypeName);
    TForteByte *acDataBuf = (TForteByte *) getMembers();

    for(unsigned int i = 0; i < pa_nLength; ++i){
      CTypeLib::createDataTypeInstance(pa_aunElementTypes[i], acDataBuf);
      acDataBuf += sizeof(CIEC_ANY);
    }
  }
}

void CIEC_STRUCT::setValue(const CIEC_ANY& pa_roValue){
  if(pa_roValue.getDataTypeID() == e_STRUCT){
    if(getStructTypeNameID() == static_cast<const CIEC_STRUCT&>(pa_roValue).getStructTypeNameID()){
      CIEC_ANY *poDstBuf = getMembers();
      const CIEC_ANY* poSrcBuf = static_cast<const CIEC_STRUCT&>(pa_roValue).getMembers();
      for(unsigned int i = 0; i < getStructSize(); ++i, ++poSrcBuf, ++poDstBuf){
        poDstBuf->setValue( *poSrcBuf);
      }
    }
  }
}

void CIEC_STRUCT::clear(){
  if(getASN1StructType()){
    CIEC_ANY *poBuf = getMembers();

    for(TForteUInt8 i = 0; i < getStructSize(); ++i, ++poBuf){
      poBuf->~CIEC_ANY();
    }

    forte_free(getGenData());
    setGenData(0);
  }
}

int CIEC_STRUCT::fromString(const char *pa_pacValue){
  int nRetVal = -1;

  if('(' == pa_pacValue[0]){
    const char *pcRunner = pa_pacValue;
    unsigned int nElementCount = 0;    // count how much elements we could retrieve

    while((*pcRunner != '\0') && (*pcRunner != ')') && (nElementCount < getStructSize())){
      pcRunner++;

      //first extract the element name
      int nLength = 0;
      CStringDictionary::TStringId unElementNameId = parseNextElementId(pcRunner, nLength);

      if(CStringDictionary::scm_nInvalidStringId == unElementNameId){
        return -1;
      }

      pcRunner += nLength;

      CIEC_ANY *poMember = getMemberNamed(unElementNameId);
      if(0 == poMember){
        return -1;
      }

      nLength = poMember->fromString(pcRunner);

      if(0 < nLength){
        nElementCount++;
        pcRunner += nLength;

        //remove trailing whitespaces
        while(' ' == *pcRunner){
          pcRunner++;
        }

        if(',' != *pcRunner){
          break;
        }
      }
      else{
        //we have an error or the end bracket
        break;
      }
    }

    if((*pcRunner == ')')&& (nElementCount == getStructSize())) {
      //structs have to and on a closing bracket
      nRetVal = static_cast<int>(pcRunner - pa_pacValue + 1); //+1 from the closing bracket
    }
  }
  return nRetVal;
}

CStringDictionary::TStringId CIEC_STRUCT::parseNextElementId(const char *pa_pcRunner, int &pa_nCounter){
  CStringDictionary::TStringId unElementNameId = CStringDictionary::scm_nInvalidStringId;
  TIdentifier acIdentifier;

  //remove any leading whitespaces before the identifier
  while(' ' == *pa_pcRunner){
    pa_pcRunner++;
    pa_nCounter++;
  }

  for(unsigned int i = 0; ((*pa_pcRunner != '\0') && (*pa_pcRunner != ')')); ++i, ++pa_nCounter, ++pa_pcRunner){
    if(i >= cg_nIdentifierLength){
      //currently we only allow identifiers smaller that size
      return CStringDictionary::scm_nInvalidStringId;
    }

    if((':' == *pa_pcRunner) || (' ' == *pa_pcRunner)){
      acIdentifier[i] = '\0'; //close identifier string
      break;
    }
    acIdentifier[i] = *pa_pcRunner;
  }
  //remove any whitespaces between identifier and assignment operator
  while(' ' == *pa_pcRunner){
    pa_pcRunner++;
    pa_nCounter++;
  }

  if(':' == *pa_pcRunner){
    //only if we have a : we have a correct identifier
    pa_pcRunner++;
    pa_nCounter++;
    if('=' == *pa_pcRunner){
      unElementNameId = CStringDictionary::getInstance().getId(acIdentifier);
      pa_pcRunner++;
      pa_nCounter++;
      //remove any whitespaces between assignment operator and value
      while(' ' == *pa_pcRunner){
        pa_pcRunner++;
        pa_nCounter++;
      }
      if(('\0' == *pa_pcRunner) || (')' == *pa_pcRunner)){
        return CStringDictionary::scm_nInvalidStringId;
      }
    }
  }

  return unElementNameId;
}

int CIEC_STRUCT::toString(char* pa_acValue, unsigned int pa_nBufferSize) const{
  int nBytesUsed = -1;

  if(pa_nBufferSize){
    *pa_acValue = '(';
    pa_acValue++;
    pa_nBufferSize--;
    nBytesUsed = 1;
    TForteUInt16 unSize = getStructSize();
    const CStringDictionary::TStringId *punMemberNameIds = elementNames();
    const CIEC_ANY *poMembers = getMembers();

    for(unsigned int i = 0; i < unSize; ++i, ++poMembers, ++punMemberNameIds){
      const char *acMemberName = CStringDictionary::getInstance().get(*punMemberNameIds);
      int nUsedBytesByElement;
      if(strlen(acMemberName) + 2 > pa_nBufferSize){
        return -1;
      }

      strcpy(pa_acValue, acMemberName);
      pa_acValue += strlen(acMemberName);
      *pa_acValue = ':';
      pa_acValue++;
      *pa_acValue = '=';
      pa_acValue++;
      pa_nBufferSize -= (strlen(acMemberName) + 2);
      nBytesUsed += (strlen(acMemberName) + 2);

      nUsedBytesByElement = poMembers->toString(pa_acValue, pa_nBufferSize);
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
    *pa_acValue = ')';
    pa_acValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

CIEC_ANY *CIEC_STRUCT::getMemberNamed(CStringDictionary::TStringId pa_unMemberNameId){
  CIEC_ANY *poRetVal = 0;

  CIEC_ANY *poMembers = getMembers();
  const CStringDictionary::TStringId *punMemberNameIds = elementNames();

  for(unsigned int i = 0; i < getStructSize(); ++i, ++poMembers, ++punMemberNameIds){
    if(*punMemberNameIds == pa_unMemberNameId){
      poRetVal = poMembers;
      break;
    }

  }

  return poRetVal;
}

