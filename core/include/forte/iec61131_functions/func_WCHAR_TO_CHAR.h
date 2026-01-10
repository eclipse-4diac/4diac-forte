/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include <type_traits>
#include "common.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_wstring.h"

namespace forte {
  inline CIEC_CHAR func_WCHAR_TO_CHAR(const CIEC_WCHAR &paVal) {
    return CIEC_ANY::cast<CIEC_CHAR>(paVal);
  }
} // namespace forte
