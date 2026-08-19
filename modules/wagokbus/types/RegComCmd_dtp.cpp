/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.qualifier!
 ***
 *** Name: RegComCmd
 *** Description: Write register communication
 *** Version:
 ***     3.0: 2026-07-21/Monika Wenger -  -
 *************************************************************************/

#include "RegComCmd_dtp.h"


using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::io::wago {

  DEFINE_FIRMWARE_DATATYPE(RegComCmd, "eclipse4diac::io::wago::RegComCmd"_STRID);

  const StringId CIEC_RegComCmd::scmElementNames[] = {"RegNr"_STRID, "REG_D0"_STRID, "REG_D1"_STRID};

  CIEC_RegComCmd::CIEC_RegComCmd() :
      CIEC_STRUCT(),
      var_RegNr(0_USINT),
      var_REG_D0(0_BYTE),
      var_REG_D1(0_BYTE) {
  }

  CIEC_RegComCmd::CIEC_RegComCmd(const CIEC_USINT &paRegNr, const CIEC_BYTE &paREG_D0, const CIEC_BYTE &paREG_D1) :
      CIEC_STRUCT(),
      var_RegNr(paRegNr),
      var_REG_D0(paREG_D0),
      var_REG_D1(paREG_D1) {
  }

  StringId CIEC_RegComCmd::getStructTypeNameID() const {
    return "eclipse4diac::io::wago::RegComCmd"_STRID;
  }

  void CIEC_RegComCmd::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("eclipse4diac::io::wago::RegComCmd"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_RegComCmd &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_RegComCmd::getMember(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_RegNr;
      case 1: return &var_REG_D0;
      case 2: return &var_REG_D1;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_RegComCmd::getMember(const size_t paIndex) const {
    switch(paIndex) {
      case 0: return &var_RegNr;
      case 1: return &var_REG_D0;
      case 2: return &var_REG_D1;
    }
    return nullptr;
  }

}
