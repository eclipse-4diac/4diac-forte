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

#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_char.h"

namespace forte {
  inline CIEC_STRING func_CHAR_TO_STRING(const CIEC_CHAR &paVal) {
    CIEC_STRING stringCast(paVal);
    return stringCast;
  }
} // namespace forte
