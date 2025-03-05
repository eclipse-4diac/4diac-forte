/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - add equals function
 *                 - refactor struct memory layout
 *******************************************************************************/
#include "forte_struct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_struct_gen.cpp"
#endif
#include <cstring>

CIEC_ANY *CIEC_STRUCT::getMemberNamed(CStringDictionary::TStringId paMemberNameId) {
  size_t index = getMemberIndex(paMemberNameId);
  if(index != csmNIndex){
    return getMember(static_cast<size_t>(index));
  }
  return nullptr;
}

CIEC_ANY *CIEC_STRUCT::getMemberNamed(const char *paMemberName) {
  CStringDictionary::TStringId elementNameId = CStringDictionary::getId(paMemberName);
  if (CStringDictionary::scmInvalidStringId != elementNameId) {
    return getMemberNamed(elementNameId);
  }
  return nullptr;
}

size_t CIEC_STRUCT::getMemberIndex(CStringDictionary::TStringId paMemberNameId){
  const CStringDictionary::TStringId *punMemberNameIds = elementNames();
  for(size_t i = 0; i < getStructSize(); ++i){
    if(punMemberNameIds[i] == paMemberNameId){
      return i;
    }
  }
  return csmNIndex;
}

void CIEC_STRUCT::setValue(const CIEC_ANY &paValue) {
  if (paValue.getDataTypeID() == e_STRUCT) {
    auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
    if (getStructTypeNameID() == otherStruct.getStructTypeNameID()) {
      size_t structSize = getStructSize();
      for (size_t i = 0; i < structSize; ++i) {
        getMember(i)->setValue(*otherStruct.getMember(i));
      }
    }
  }
}

int CIEC_STRUCT::fromString(const char *paValue) {
  int nRetVal = -1;
  const char *pcRunner = paValue;
  if ('(' == *(pcRunner++)) {
    while (('\0' != *pcRunner) && (')' != *pcRunner)) {
      findNextNonBlankSpace(&pcRunner);

      int valueLength = initializeFromString(pcRunner);
      if (0 > valueLength) {
        break;
      }
      pcRunner += valueLength;

      findNextNonBlankSpace(&pcRunner);
      if (',' == *pcRunner) {
        pcRunner++;
      } else {
        //we have an error or the end parentheses
        break;
      }
    }
    if (*(pcRunner++) == ')') { //arrays have to end on a closing parentheses
      nRetVal = static_cast<int>(pcRunner - paValue);
    }
  }
  return nRetVal;
}

int CIEC_STRUCT::initializeFromString(const char *paValue) {
  int nRetVal = -1;
  const char *pcRunner = paValue;
  //first extract the element name
  CStringDictionary::TStringId elementNameId = parseNextElementId(pcRunner);
  if (CStringDictionary::scmInvalidStringId != elementNameId) {
    findNextNonBlankSpace(&pcRunner);
    if (':' == *(pcRunner++) && '=' == *(pcRunner++)) { // parse ":="
      findNextNonBlankSpace(&pcRunner);
      CIEC_ANY *member = getMemberNamed(elementNameId);
      if(nullptr != member) {
        const int valueLength = member->fromString(pcRunner);
        if (0 <= valueLength) {
          pcRunner += valueLength;
          nRetVal = static_cast<int>(pcRunner - paValue);
        }
      }
    }
  }
  return nRetVal;
}

CStringDictionary::TStringId CIEC_STRUCT::parseNextElementId(const char *&paRunner) {
  CStringDictionary::TStringId result = CStringDictionary::scmInvalidStringId;
  const char *identifierStart = paRunner;
  const char *identifierEnd = std::strpbrk(paRunner, " :)");
  if (identifierEnd) {
    paRunner = identifierEnd;
    result = CStringDictionary::getId(identifierStart,
                                                    static_cast<size_t>(identifierEnd - identifierStart));
  }
  return result;
}

int CIEC_STRUCT::toString(char *paValue, size_t paBufferSize) const {
  char *start = paValue;
  if (!paBufferSize) {
    return -1;
  }

  *paValue = '(';
  paValue++;
  paBufferSize--;

  size_t structSize = getStructSize();
  const CStringDictionary::TStringId *memberNameIds = elementNames();
  for (size_t i = 0; i < structSize; ++i) {
    const char *memberName = CStringDictionary::get(memberNameIds[i]);
    size_t memberNameSize = std::strlen(memberName);
    if (memberNameSize + 2 > paBufferSize) {
      return -1;
    }
    std::memcpy(paValue, memberName, memberNameSize);
    paValue += memberNameSize;
    *(paValue++) = ':';
    *(paValue++) = '=';
    paBufferSize -= memberNameSize + 2;

    int usedBytesByElement = getMember(i)->toString(paValue, paBufferSize);
    if (0 > usedBytesByElement) {
      return -1;
    }
    paValue += usedBytesByElement;
    paBufferSize -= static_cast<size_t>(usedBytesByElement);
    if (!paBufferSize) {
      return -1;
    }

    if (i != structSize - 1) {
      *(paValue++) = ',';
      paBufferSize--;
    }
  }
  if (paBufferSize < 2) {
    return -1;
  }
  *(paValue++) = ')';
  *paValue = '\0';
  return static_cast<int>(paValue - start);
}

size_t CIEC_STRUCT::getToStringBufferSize() const {
  size_t retVal = 3; // 2 bytes for the open and closing brackets one for the '\0'
  size_t structSize = getStructSize();
  retVal += structSize ? structSize - 1 : 0; //for the commas between the elements
  retVal += structSize * 2; //for the := of each element
  for (size_t i = 0; i < structSize; i++) {
    retVal += strlen(CStringDictionary::get(elementNames()[i])); //element name
    retVal += getMember(i)->getToStringBufferSize() - 1; //length of the element itself. -1 for the included \0 in each element
  }
  return retVal;
}

void CIEC_STRUCT::findNextNonBlankSpace(const char **paRunner) {
  while (' ' == **paRunner) {
    (*paRunner)++;
  }
}

bool CIEC_STRUCT::equals(const CIEC_ANY &paOther) const {
  if (paOther.getDataTypeID() == CIEC_ANY::e_STRUCT) {
    auto &otherStruct = static_cast<const CIEC_STRUCT &>(paOther);
    if (getStructTypeNameID() == otherStruct.getStructTypeNameID()) {
      for (size_t i = 0; i < getStructSize(); ++i) {
        if (!getMember(i)->equals(*otherStruct.getMember(i))) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

