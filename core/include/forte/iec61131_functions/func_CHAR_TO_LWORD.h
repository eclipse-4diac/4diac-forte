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

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_char.h"

namespace forte {
  inline CIEC_LWORD func_CHAR_TO_LWORD(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_LWORD>(paVal);
  }
} // namespace forte
