/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                      2018-2019 TU Vienna/ACIN
 *               2022, 2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD, removes invalid casts,
 *        update implementation to use new cast function, changes for removed
 *        implicit constructor for primitve types
 *      - adds more *_BCD_TO_** functions
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_udint.h"
#include "func_TO_BCD.h"

namespace forte {
  inline CIEC_LWORD func_UDINT_TO_BCD_LWORD(const CIEC_UDINT &paVal) {
    return func_TO_BCD<CIEC_LWORD>(paVal);
  }
} // namespace forte
