/*************************************************************************
 *** Copyright (c) 2026 HR Agartechnik GmbH
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604152003!
 ***
 *** Name: E_D_FF_ANY
 *** Description: Data latch (d) flip flop
 *** Version:
 ***     1.0: 2026-04-17/Franz Höpfinger - HR Agrartechnik GmbH - E_D_FF copied and replace BOOL by ANY
 *************************************************************************/

#include "forte/iec61499/events/E_D_FF_ANY_fbt.h"

#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"CLK"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
    const auto cDataInputNames = std::array{"D"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_D_FF_ANY, "iec61499::events::E_D_FF_ANY"_STRID, TypeHash)

  FORTE_E_D_FF_ANY::FORTE_E_D_FF_ANY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_D(CIEC_ANY_VARIANT()),
      var_Q(CIEC_ANY_VARIANT()),
      conn_EO(*this, 0),
      conn_D(nullptr),
      conn_Q(*this, 0, var_Q) {
  }

  void FORTE_E_D_FF_ANY::setInitialValues() {
    CBasicFB::setInitialValues();
    var_D = CIEC_ANY_VARIANT();
    var_Q = CIEC_ANY_VARIANT();
  }

  void FORTE_E_D_FF_ANY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if ((scmEventCLKID == paEIID) && (!var_Q.equals(var_D))) {
      var_Q.setValue(var_D.unwrap());
      sendOutputEvent(scmEventEOID, paECET);
    }
  }

  void FORTE_E_D_FF_ANY::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCLKID: {
        readData(0, var_D, conn_D);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_D_FF_ANY::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(1, var_Q, conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_D_FF_ANY::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_D;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_D_FF_ANY::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_D_FF_ANY::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_D_FF_ANY::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_D;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_D_FF_ANY::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_D_FF_ANY::getVarInternal(size_t) {
    return nullptr;
  }

} // namespace forte::iec61499::events
