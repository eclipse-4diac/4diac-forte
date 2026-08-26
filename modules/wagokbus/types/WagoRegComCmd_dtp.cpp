/*************************************************************************
 * Copyright (c) 2026 Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.qualifier!
 ***
 *** Name: WagoRegComCmd
 *** Description: Write register communication
 *** Version:
 ***     3.0: 2026-07-21/Monika Wenger -  -
 *************************************************************************/

#include "WagoRegComCmd_dtp.h"


using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::io::wago {

  DEFINE_FIRMWARE_DATATYPE(WagoRegComCmd, "eclipse4diac::io::wago::WagoRegComCmd"_STRID);

  const StringId CIEC_WagoRegComCmd::scmElementNames[] = {"RegNr"_STRID, "REG_D0"_STRID, "REG_D1"_STRID};

  CIEC_WagoRegComCmd::CIEC_WagoRegComCmd() :
      CIEC_STRUCT(),
      var_RegNr(0_USINT),
      var_REG_D0(0_BYTE),
      var_REG_D1(0_BYTE) {
  }

  CIEC_WagoRegComCmd::CIEC_WagoRegComCmd(const CIEC_USINT &paRegNr, const CIEC_BYTE &paREG_D0, const CIEC_BYTE &paREG_D1) :
      CIEC_STRUCT(),
      var_RegNr(paRegNr),
      var_REG_D0(paREG_D0),
      var_REG_D1(paREG_D1) {
  }

  StringId CIEC_WagoRegComCmd::getStructTypeNameID() const {
    return "eclipse4diac::io::wago::WagoRegComCmd"_STRID;
  }

  void CIEC_WagoRegComCmd::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("eclipse4diac::io::wago::WagoRegComCmd"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_WagoRegComCmd &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_WagoRegComCmd::getMember(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_RegNr;
      case 1: return &var_REG_D0;
      case 2: return &var_REG_D1;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_WagoRegComCmd::getMember(const size_t paIndex) const {
    switch(paIndex) {
      case 0: return &var_RegNr;
      case 1: return &var_REG_D0;
      case 2: return &var_REG_D1;
    }
    return nullptr;
  }

}
