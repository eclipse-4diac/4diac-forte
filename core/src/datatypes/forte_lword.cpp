/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkiumians, Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LWORD, "LWORD"_STRID)

  void CIEC_LWORD::setValue(const CIEC_ANY &paValue) {
    setValueSimple(paValue);
  }

  void CIEC_LWORD::setValue(const CIEC_REAL &paValue) {
    setValueSimple(paValue);
  }

  void CIEC_LWORD::setValue(const CIEC_LREAL &paValue) {
    setValueSimple(paValue);
  }

  const StringId CDataTypeTrait<CIEC_LWORD>::scmDataTypeName = "LWORD"_STRID;
} // namespace forte
