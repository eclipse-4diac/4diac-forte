/*******************************************************************************
 * Copyright (c) 2010, 2025 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                          HIT robot group, Monika Wenger,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *     Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik-Merkumians - fixes DT_TO_TOD
 *   Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *   Zhao Xin -fixes string conversion
 *   Monika Wenger - func_ANY_AS_STRING
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

namespace forte {
  inline void stringConverter(CIEC_WSTRING &paString, const CIEC_ANY &paVal) {
    std::string buffer;
    paVal.toString(buffer);
    paString = CIEC_WSTRING(buffer.c_str());
  }

  inline void stringConverter(CIEC_STRING &paString, const CIEC_ANY &paVal) {
    std::string buffer;
    paVal.toString(buffer);
    paString = CIEC_STRING(std::move(buffer));
  }
} // namespace forte
