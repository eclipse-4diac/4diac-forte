/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtControl GmbH, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 *               2022 - 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Stansilav Meduna, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians, Martin Jobst - added ctors for copying one type
 *      to another and iterators
 *    Martin Jobst - add support for repeat syntax
 *                 - add equals function
 *                 - collapse identical consecutive elements
 *                 - add support for data types with different size
 *                 - refactored for elements with dynamic size
 *                 - refactored for lower and upper bounds
 *                 - refactored array type structure
 *******************************************************************************/

#include "forte_array.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_array_gen.cpp"
#endif

#include "forte_ulint.h"

CStringDictionary::TStringId CIEC_ARRAY::getTypeNameID() const {
  return g_nStringIdARRAY;
}

void CIEC_ARRAY::setValue(const CIEC_ANY &paValue) {
  if (paValue.getDataTypeID() == CIEC_ANY::e_ARRAY) {
    operator=(static_cast<const CIEC_ARRAY &>(paValue));
  }
}

bool CIEC_ARRAY::equals(const CIEC_ANY &paOther) const {
  if (paOther.getDataTypeID() == CIEC_ANY::e_ARRAY) {
    auto &otherArray = static_cast<const CIEC_ARRAY &>(paOther);
    if (getLowerBound() != otherArray.getLowerBound() || getUpperBound() != otherArray.getUpperBound()) {
      return false;
    }

    for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
      if (!operator[](i).equals(otherArray[i])) {
        return false;
      }
    }
    return true;
  }
  return false;
}

int CIEC_ARRAY::toString(char *paValue, size_t paBufferSize) const {
  int nBytesUsed = -1;

  if (size() > cmCollapseMaxSize) {
    return toCollapsedString(paValue, paBufferSize);
  }

  if (paBufferSize) {
    *paValue = '[';
    paValue++;
    paBufferSize--;
    nBytesUsed = 1;
    if(size()) { // check if initialized
      for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
        int nUsedBytesByElement = operator[](i).toString(paValue, paBufferSize);
        if (-1 == nUsedBytesByElement) {
          return -1;
        }
        paBufferSize -= static_cast<size_t>(nUsedBytesByElement);
        paValue += nUsedBytesByElement;
        if (!paBufferSize) {
          return -1;
        }

        nBytesUsed += nUsedBytesByElement;

        if (i != end) {
          *paValue = ',';
          paValue++;

          ++nBytesUsed;
          paBufferSize--;
        }
      }
    }
    if (paBufferSize < 2) {
      return -1;
    }
    *paValue = ']';
    paValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

int CIEC_ARRAY::toCollapsedString(char *paValue, size_t paBufferSize) const {
  int nBytesUsed = -1;

  if (paBufferSize > 3) {
    *paValue = '[';
    paValue++;
    paBufferSize--;
    nBytesUsed = 1;
    size_t count = 0;
    const CIEC_ANY *lastElement = nullptr;
    for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
      if (lastElement != nullptr && !lastElement->equals(operator[](i))) {
        int usedBytesByElement = toCollapsedElementString(*lastElement, count, nBytesUsed > 1, paValue,
                                                          paBufferSize);
        if (usedBytesByElement < 0) {
          return -1;
        }
        paValue += usedBytesByElement;
        paBufferSize -= static_cast<size_t>(usedBytesByElement);
        nBytesUsed += usedBytesByElement;
        count = 0;
      }
      lastElement = &operator[](i);
      count++;
    }
    if (lastElement) {
      int usedBytesByElement = toCollapsedElementString(*lastElement, count, nBytesUsed > 1, paValue,
                                                        paBufferSize);
      if (usedBytesByElement < 0) {
        return -1;
      }
      paValue += usedBytesByElement;
      paBufferSize -= static_cast<size_t>(usedBytesByElement);
      nBytesUsed += usedBytesByElement;
    }

    if (paBufferSize < 2) {
      return -1;
    }
    *paValue = ']';
    paValue[1] = '\0';
    nBytesUsed++;
  }

  return nBytesUsed;
}

int CIEC_ARRAY::toCollapsedElementString(const CIEC_ANY &paElement, size_t paCount, bool paComma, char *paValue,
                                         size_t paBufferSize) const {
  int nBytesUsed = 0;

  if (paComma) {
    if (paBufferSize < 1) {
      return -1;
    }

    *(paValue++) = ',';
    paBufferSize--;
    nBytesUsed++;
  }

  if (paCount > 1) {
    int usedBytesByRepeat = CIEC_ULINT(paCount).toString(paValue, paBufferSize);
    if (usedBytesByRepeat < 0) {
      return -1;
    }
    paValue += usedBytesByRepeat;
    paBufferSize -= static_cast<size_t>(usedBytesByRepeat);
    nBytesUsed += usedBytesByRepeat;

    if (paBufferSize < 1) {
      return -1;
    }
    *(paValue++) = '(';
    paBufferSize--;
    nBytesUsed++;
  }

  int usedBytesByElement = paElement.toString(paValue, paBufferSize);
  if (usedBytesByElement < 0) {
    return -1;
  }
  paValue += usedBytesByElement;
  paBufferSize -= static_cast<size_t>(usedBytesByElement);
  nBytesUsed += usedBytesByElement;

  if (paCount > 1) {
    if (paBufferSize < 1) {
      return -1;
    }
    *(paValue++) = ')';
    // paBufferSize--; value unused
    nBytesUsed++;
  }
  return nBytesUsed;
}

size_t CIEC_ARRAY::getToStringBufferSize() const {
  size_t retVal = 3; // 2 bytes for the open and closing brackets and one for the '\0'
  retVal += (size() > 1) ? (size() - 1) : 0; // for the commas between the elements

  if (size()) {
    switch (getElementDataTypeID()) { // in these cases, the length of the elements are not always the same
      case CIEC_ANY::e_WSTRING:
      case CIEC_ANY::e_STRING: // quotes or double quotes are already counted in ANY_STRING
      case CIEC_ANY::e_ARRAY:
      case CIEC_ANY::e_STRUCT:
        for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
          // -1 since the \0 of each element is counted as just one at the beginning
          retVal += operator[](i).getToStringBufferSize() - 1;
        }
        break;
      default:
        //-1 since the \0 of each element is counted as just one at the beginning
        retVal += size() * (operator[](getLowerBound()).getToStringBufferSize() - 1);
        break;
    }
  }

  return retVal;
}

