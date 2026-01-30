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

#include "forte/iec61131_functions/func_TRUNC_UINT.h"
#include "forte/iec61131_functions/func_TRUNC.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_real.h"

namespace forte {
  CIEC_UINT func_TRUNC_UINT(const CIEC_REAL &paIN) {
    return func_TRUNC<CIEC_UINT, CIEC_REAL>(paIN);
  }

  CIEC_UINT func_TRUNC_UINT(const CIEC_LREAL &paIN) {
    return func_TRUNC<CIEC_UINT, CIEC_LREAL>(paIN);
  }

} // namespace forte
