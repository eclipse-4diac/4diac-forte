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

#include "forte/iec61131_functions/func_FROM_LITTLE_ENDIAN.h"
#include "forte/datatypes/forte_array_dynamic.h"

namespace forte {
  CIEC_ARRAY_DYNAMIC func_FROM_LITTLE_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue) {
    if constexpr (std::endian::native == std::endian::big) {
      CIEC_ARRAY_DYNAMIC reversed(paValue);
      swapEndianessInplace(reversed);
      return reversed;
    } else {
      return paValue;
    }
  }
} // namespace forte
