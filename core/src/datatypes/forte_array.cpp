/*******************************************************************************
 * Copyright (c) 2007, 2023 ACIN, nxtControl GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Stansilav Meduna, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians, Martin Jobst - added ctors for copying one type
 *                  to another and iterators
 *   Martin Jobst - add support for repeat syntax
 *                - add equals function
 *                - collapse identical consecutive elements
 *                - add support for data types with different size
 *                - refactored for elements with dynamic size
 *                - refactored for lower and upper bounds
 *                - refactored array type structure
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_ulint.h"

using namespace forte::literals;

namespace forte {
  StringId CIEC_ARRAY::getTypeNameID() const {
    return "ARRAY"_STRID;
  }

  void CIEC_ARRAY::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_ARRAY) {
      operator=(static_cast<const CIEC_ARRAY &>(paValue));
    }
  }

  void CIEC_ARRAY::reset() {
    for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
      operator[](i).reset();
    }
  }

  void CIEC_ARRAY::toString(std::string &paTargetBuf) const {
    if (size() > cmCollapseMaxSize) {
      toCollapsedString(paTargetBuf);
    } else {
      paTargetBuf += '[';
      if (size()) { // check if initialized
        for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
          operator[](i).toString(paTargetBuf);
          if (i != end) {
            paTargetBuf += ',';
          }
        }
      }
      paTargetBuf += ']';
    }
  }

  void CIEC_ARRAY::toCollapsedString(std::string &paTargetBuf) const {
    paTargetBuf += '[';
    size_t count = 0;
    const CIEC_ANY *lastElement = nullptr;
    for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
      if (lastElement != nullptr && !lastElement->equals(operator[](i))) {
        toCollapsedElementString(*lastElement, count, paTargetBuf);
        if (i != end) {
          paTargetBuf += ',';
        }
        count = 0;
      }
      lastElement = &operator[](i);
      count++;
    }
    if (lastElement) {
      toCollapsedElementString(*lastElement, count, paTargetBuf);
    }
    paTargetBuf += ']';
  }

  void CIEC_ARRAY::toCollapsedElementString(const CIEC_ANY &paElement, size_t paCount, std::string &paTargetBuf) const {
    if (paCount > 1) {
      CIEC_ULINT(paCount).toString(paTargetBuf);
      paTargetBuf += '(';
    }
    paElement.toString(paTargetBuf);
    if (paCount > 1) {
      paTargetBuf += ')';
    }
  }
} // namespace forte
