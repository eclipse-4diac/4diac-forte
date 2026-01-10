/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 ACIN, fortiss GmbH, 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds NOW and NOW_MONOTONIC functions,
 *        changes for removed implicit constructor for primitive types
 *******************************************************************************/

#include "forte/iec61131_functions/func_ADD_LDT_LTIME.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_ldate_and_time.h"

namespace forte {
  CIEC_LDATE_AND_TIME func_ADD_LDT_LTIME(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2) {
    return CIEC_LDATE_AND_TIME(static_cast<TForteUInt64>(paIN1) + static_cast<TForteUInt64>(paIN2));
  }

} // namespace forte
