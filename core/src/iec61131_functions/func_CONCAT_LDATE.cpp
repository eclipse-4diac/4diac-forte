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

#include "forte/datatypes/forte_lint.h"
#include "forte/iec61131_functions/func_CONCAT_LDATE.h"
#include "forte/iec61131_functions/func_CONCAT_DT.h"
#include "forte/iec61131_functions/func_LDT_TO_LDATE.h"
#include "forte/datatypes/forte_ldate.h"
#include "forte/datatypes/forte_any_int.h"

namespace forte {
  CIEC_LDATE func_CONCAT_LDATE(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY) {
    return func_LDT_TO_LDATE(func_CONCAT_DT(YEAR, MONTH, DAY, CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0)));
  }

} // namespace forte
