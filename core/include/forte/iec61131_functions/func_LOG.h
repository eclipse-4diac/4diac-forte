/*******************************************************************************
 * Copyright (c) 2010,2025 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH
 *                         Primetals Technologies Austria GmbH
 *                         Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger, Ingo Hegny, Patrick Smejkal, Gerhard Ebenhofer,
 *    Matthias Plasch, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - added different type templates for IEC 61131-3 functions and adds several type guards
 *      - Reworked binary operator templates to create correctly calculated and typed results
 *      - reworked and fixes ADD and SUB for time types
 *      - Added variadic comparison, MIN, MAX, ADD, MUL functions
 *      - Added unary plus function
 *    Martin Erich Jobst
 *      - added lower and upper bound functions
 *******************************************************************************/

#pragma once

#include <cmath>

#ifdef VXWORKS
#define log10f(x) static_cast<TForteFloat>(log10(x))
#endif

#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_real.h"

namespace forte {
  inline CIEC_REAL func_LOG(const CIEC_REAL &paIN) {
    return CIEC_REAL(log10f(static_cast<CIEC_REAL::TValueType>(paIN)));
  }
  inline CIEC_LREAL func_LOG(const CIEC_LREAL &paIN) {
    return CIEC_LREAL(log10(static_cast<CIEC_LREAL::TValueType>(paIN)));
  }

} // namespace forte
