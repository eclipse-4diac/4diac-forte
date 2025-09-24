/*******************************************************************************
 * Copyright (c) 2007, 2025 ACIN, nxtControl GmbH, fortiss GmbH,
 *                          Martin Erich Jobst
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
 *    Martin Jobst
 *      - refactored array type structure
 *      - added lower and upper bound with dimension
 *******************************************************************************/

#include "forte/datatypes/forte_array_dynamic.h"
#include "forte/datatypes/forte_ulint.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ARRAY_DYNAMIC, "ARRAY"_STRID)

  //! Function to configure the array if it is created via the typelib
  void CIEC_ARRAY_DYNAMIC::setup(TForteUInt16 paLength, StringId paArrayType) {
    clear();
    if (paLength) {
      mLowerBound = 0;
      mUpperBound = paLength - 1;

      mElementDataTypeEntry = getDataTypeEntry(paArrayType);
      if (mElementDataTypeEntry) {
        mElementSize = mElementDataTypeEntry->getSize();
        mData = operator new(paLength * mElementSize);
        auto *dest = static_cast<TForteByte *>(mData);
        for (; mSize < paLength; ++mSize) { // increment size one-by-one to track allocated elements for destruction
          mElementDataTypeEntry->createDataTypeInstance(dest);
          dest += mElementSize;
        }
      }
    }
  }

  //! Function to configure the array if it is created via the typelib
  void CIEC_ARRAY_DYNAMIC::setup(intmax_t paLowerBound, intmax_t paUpperBound, StringId paArrayType) {
    clear();
    mLowerBound = paLowerBound;
    mUpperBound = paUpperBound;

    mElementDataTypeEntry = getDataTypeEntry(paArrayType);
    if (mElementDataTypeEntry) {
      auto size = static_cast<size_t>(paUpperBound - paLowerBound + 1);
      mElementSize = mElementDataTypeEntry->getSize();
      mData = operator new(size * mElementSize);
      auto *dest = static_cast<TForteByte *>(mData);
      for (; mSize < size; ++mSize) { // increment size one-by-one to track allocated elements for destruction
        mElementDataTypeEntry->createDataTypeInstance(dest);
        dest += mElementSize;
      }
    }
  }

  int CIEC_ARRAY_DYNAMIC::fromString(const char *paValue) {
    int nRetVal = -1;
    const char *pcRunner = paValue;
    if ('[' == paValue[0]) {
      pcRunner++;

      CIEC_ANY *poBufVal = nullptr;
      iterator iter = begin();

      while (('\0' != *pcRunner) && (']' != *pcRunner)) {
        findNextNonBlankSpace(&pcRunner);

        int valueLength = initializeFromString(iter, pcRunner, &poBufVal);
        if (0 > valueLength) {
          break;
        }
        pcRunner += valueLength;

        findNextNonBlankSpace(&pcRunner);
        if (',' == *pcRunner) {
          pcRunner++;
        } else {
          // we have an error or the end bracket
          break;
        }
      }
      if (*pcRunner == ']') {
        // arrays have to and on a closing bracket
        nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing bracket
        // For the rest of the array size copy the default element
        std::destroy(iter, end());
        size_t size = mSize;
        mSize = static_cast<size_t>(iter - begin());
        auto *dest = reinterpret_cast<TForteByte *>(&*iter);
        for (; mSize < size; ++mSize) { // increment size one-by-one to track allocated elements for destruction
          mElementDataTypeEntry->createDataTypeInstance(dest);
          dest += mElementSize;
        }
      }
      delete poBufVal;
    }
    return nRetVal;
  }

  int CIEC_ARRAY_DYNAMIC::initializeFromString(iterator &paPosition, const char *paSrcString, CIEC_ANY **paBufVal) {
    // check repeat syntax (e.g., "255(1, 2, 3)")
    CIEC_ULINT repeat;
    const char *pcRunner = paSrcString;
    iterator initialPosition = paPosition;
    int repeatLength = repeat.fromString(pcRunner);
    if (0 < repeatLength) {
      pcRunner += repeatLength;
      findNextNonBlankSpace(&pcRunner);
      if ('(' == *pcRunner) {
        pcRunner++;
        while (('\0' != *pcRunner) && (')' != *pcRunner)) {
          findNextNonBlankSpace(&pcRunner);

          int valueLength = initializeSimpleFromString(paPosition, pcRunner, paBufVal);
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
        if (*pcRunner == ')') { // repeat syntax elements have to and on a closing parentheses
          intmax_t repeatSequenceLength = paPosition - initialPosition;
          for (size_t rep = 1; rep < repeat.getUnsignedValue() && paPosition != end(); ++rep) { // once added already
            for (intmax_t seqIndex = 0; seqIndex < repeatSequenceLength && paPosition != end(); ++seqIndex) {
              (paPosition++)->setValue(*(initialPosition + seqIndex));
            }
          }
          return static_cast<int>(pcRunner - paSrcString + 1); //+1 from the closing parentheses
        }
        return -1;
      }
    }
    return initializeSimpleFromString(paPosition, paSrcString, paBufVal);
  }

  int CIEC_ARRAY_DYNAMIC::initializeSimpleFromString(iterator &paPosition,
                                                     const char *paSrcString,
                                                     CIEC_ANY **paBufVal) {
    if (paPosition != end()) {
      return (paPosition++)->fromString(paSrcString);
    } else {
      if (nullptr == *paBufVal) {
        *paBufVal = mElementDataTypeEntry->createDataTypeInstance(nullptr);
      }
      return (*paBufVal)->fromString(paSrcString);
    }
  }

  intmax_t CIEC_ARRAY_DYNAMIC::getLowerBound(intmax_t paDimension) const {
    if (paDimension == 1) {
      return mLowerBound;
    }
    if (paDimension < 1) {
      DEVLOG_ERROR("The dimension must not be less than 1\n");
      return 0;
    }
    if (!mData) {
      DEVLOG_ERROR("The array is not initialized\n");
      return 0;
    }
    const CIEC_ANY &element = *reinterpret_cast<CIEC_ANY *>(mData);
    if (element.getDataTypeID() != e_ARRAY) {
      DEVLOG_ERROR("The dimension is larger than the dimensions of the array\n");
      return 0;
    }
    return static_cast<const CIEC_ARRAY &>(element).getLowerBound(paDimension - 1);
  }

  intmax_t CIEC_ARRAY_DYNAMIC::getUpperBound(intmax_t paDimension) const {
    if (paDimension == 1) {
      return mUpperBound;
    }
    if (paDimension < 1) {
      DEVLOG_ERROR("The dimension must not be less than 1\n");
      return 0;
    }
    if (!mData) {
      DEVLOG_ERROR("The array is not initialized\n");
      return 0;
    }
    const CIEC_ANY &element = *reinterpret_cast<CIEC_ANY *>(mData);
    if (element.getDataTypeID() != e_ARRAY) {
      DEVLOG_ERROR("The dimension is larger than the dimensions of the array\n");
      return 0;
    }
    return static_cast<const CIEC_ARRAY &>(element).getUpperBound(paDimension - 1);
  }

  void CIEC_ARRAY_DYNAMIC::setBounds(const CIEC_ARRAY &paArray) {
    setBounds(paArray.getLowerBound(), paArray.getUpperBound());
    if (getElementDataTypeID() == e_ARRAY && paArray.getElementDataTypeID() == e_ARRAY) {
      for (intmax_t i = mLowerBound; i <= mUpperBound; ++i) {
        static_cast<CIEC_ARRAY &>((*this)[i]).setBounds(static_cast<const CIEC_ARRAY &>(paArray[i]));
      }
    }
  }
} // namespace forte
