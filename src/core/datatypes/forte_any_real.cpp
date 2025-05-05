/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *     Martin Jobst
 *      - add support for infinity and nan values
 *******************************************************************************/
#include "forte_any_real.h"
#include <cctype>

using namespace std::literals::string_literals;

void CIEC_ANY_REAL::normalizeToStringRepresentation(std::string &paTargetBuf, size_t paStartPos) const {
  if (std::isalpha(paTargetBuf[paStartPos]) ||
      (paTargetBuf.size() > paStartPos + 1 && std::isalpha(paTargetBuf[paStartPos + 1]))) {
    // if first or second char is a alphabetic char we have nan or infinity and we should not perform corrections
    return;
  }

  auto dotPos = paTargetBuf.find('.', paStartPos);
  auto expPos = paTargetBuf.find('e', paStartPos);

  if (dotPos == std::string::npos && expPos == std::string::npos) {
    paTargetBuf += ".0"s;
  }
}
