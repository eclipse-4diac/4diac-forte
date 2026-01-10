/*******************************************************************************
 * Copyright (c) 2010-2019 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *               2022 Primetals Technologies Austria GmbH
 *
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
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_ltime_of_day.h"
#include "func_DT_TO_TOD.h"
#include "forte/datatypes/forte_date_and_time.h"

namespace forte {
  inline CIEC_LTIME_OF_DAY func_DT_TO_LTOD(const CIEC_DATE_AND_TIME &paValue) {
    return CIEC_LTIME_OF_DAY(func_DT_TO_TOD(paValue));
  }
} // namespace forte
