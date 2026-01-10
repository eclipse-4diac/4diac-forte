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

#include "forte/iec61131_functions/func_SUB_DATE_DATE.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_date.h"

namespace forte {
  CIEC_TIME func_SUB_DATE_DATE(const CIEC_DATE &paIN1, const CIEC_DATE &paIN2) {
    return CIEC_TIME(static_cast<CIEC_DATE::TValueType>(paIN1) - static_cast<CIEC_DATE::TValueType>(paIN2));
  }

} // namespace forte
