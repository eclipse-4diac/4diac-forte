/*******************************************************************************
 * Copyright (c) 2010, 2025 ACIN, fortiss GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ingo Hegny, Alois Zoitl
 *                - initial implementation and rework communication infrastructure
 *   Martin Jobst - add equals function
 *                - refactor struct memory layout
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte_struct.h"
#include <cstring>
#include "../util/string_utils.h"

using namespace std::string_literals;

CIEC_ANY *CIEC_STRUCT::getMemberNamed(CStringDictionary::TStringId paMemberNameId) {
  size_t index = getMemberIndex(paMemberNameId);
  if (index != csmNIndex) {
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

size_t CIEC_STRUCT::getMemberIndex(CStringDictionary::TStringId paMemberNameId) {
  const CStringDictionary::TStringId *punMemberNameIds = elementNames();
  for (size_t i = 0, structSize = getStructSize(); i < structSize; ++i) {
    if (punMemberNameIds[i] == paMemberNameId) {
      return i;
    }
  }
  return csmNIndex;
}

void CIEC_STRUCT::setValue(const CIEC_ANY &paValue) {
  if (paValue.getDataTypeID() == e_STRUCT) {
    auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
    if (getStructTypeNameID() == otherStruct.getStructTypeNameID()) {
      for (size_t i = 0, structSize = getStructSize(); i < structSize; ++i) {
        getMember(i)->setValue(*otherStruct.getMember(i));
      }
    }
  }
}

void CIEC_STRUCT::reset() {
  for (size_t i = 0, structSize = getStructSize(); i < structSize; ++i) {
    getMember(i)->reset();
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
        // we have an error or the end parentheses
        break;
      }
    }
    if (*(pcRunner++) == ')') { // arrays have to end on a closing parentheses
      nRetVal = static_cast<int>(pcRunner - paValue);
    }
  }
  return nRetVal;
}

int CIEC_STRUCT::initializeFromString(const char *paValue) {
  int nRetVal = -1;
  const char *pcRunner = paValue;
  // first extract the element name
  CStringDictionary::TStringId elementNameId = parseNextElementId(pcRunner);
  if (CStringDictionary::scmInvalidStringId != elementNameId) {
    findNextNonBlankSpace(&pcRunner);
    if (':' == *(pcRunner++) && '=' == *(pcRunner++)) { // parse ":="
      findNextNonBlankSpace(&pcRunner);
      CIEC_ANY *member = getMemberNamed(elementNameId);
      if (nullptr != member) {
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
    result = CStringDictionary::getId(identifierStart, static_cast<size_t>(identifierEnd - identifierStart));
  }
  return result;
}

void CIEC_STRUCT::toString(std::string &paTargetBuf) const {
  paTargetBuf += '(';
  size_t structSize = getStructSize();
  const CStringDictionary::TStringId *memberNameIds = elementNames();

  for (size_t i = 0; i < structSize; ++i) {
    forte::core::util::writeToStringNameValuePair(paTargetBuf, memberNameIds[i], getMember(i));
    if (i != structSize - 1) {
      paTargetBuf += ',';
    }
  }
  paTargetBuf += ')';
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
      for (size_t i = 0, structSize = getStructSize(); i < structSize; ++i) {
        if (!getMember(i)->equals(*otherStruct.getMember(i))) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}
