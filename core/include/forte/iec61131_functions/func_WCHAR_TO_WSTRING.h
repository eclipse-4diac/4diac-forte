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

#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_wchar.h"

namespace forte {
  inline CIEC_WSTRING func_WCHAR_TO_WSTRING(const CIEC_WCHAR &paVal) {
    CIEC_WSTRING wstringCast(paVal);
    return wstringCast;
  }
} // namespace forte
