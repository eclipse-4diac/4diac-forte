/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: E_D_FF_TMIN
 *** Description: Data latch (d) flip flop, with a Minimum inter-disposal Time between EO
 *** Version:
 ***     1.0: 2026-05-14/Franz Höpfinger - HR Agrartechnik GmbH - Inital API
 *************************************************************************/

#include "forte/iec61499/events/E_D_FF_TMIN_fbt.h"

#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID, "CLK"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "EO"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"D"_STRID, "Tmin"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "CLK"_STRID, "E_D_FF"_STRID, "CLK"_STRID},
        {{}, "INIT"_STRID, "E_TMIN"_STRID, "INIT"_STRID},
        {"E_TMIN"_STRID, "INITO"_STRID, {}, "INITO"_STRID},
        {"E_D_FF"_STRID, "EO"_STRID, "E_TMIN"_STRID, "EI"_STRID},
        {"E_TMIN"_STRID, "EO"_STRID, {}, "EO"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {"E_D_FF"_STRID, "Q"_STRID, {}, "Q"_STRID},
        {{}, "D"_STRID, "E_D_FF"_STRID, "D"_STRID},
        {{}, "Tmin"_STRID, "E_TMIN"_STRID, "Tmin"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_D_FF_TMIN, "iec61499::events::E_D_FF_TMIN"_STRID, TypeHash)

  FORTE_E_D_FF_TMIN::FORTE_E_D_FF_TMIN(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_D_FF("E_D_FF"_STRID, *this),
      fb_E_TMIN("E_TMIN"_STRID, *this),
      conn_INITO(*this, 0),
      conn_EO(*this, 1),
      conn_D(nullptr),
      conn_Tmin(nullptr),
      conn_Q(*this, 0, 0_BOOL),
      conn_if2in_D(*this, 0, 0_BOOL),
      conn_if2in_Tmin(*this, 1, 0_TIME) {};

  void FORTE_E_D_FF_TMIN::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_D.getValue() = 0_BOOL;
    conn_if2in_Tmin.getValue() = 0_TIME;
    fb_E_D_FF->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_D_FF_TMIN::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(1, conn_if2in_Tmin.getValue(), conn_Tmin);
        break;
      }
      case scmEventCLKID: {
        readData(0, conn_if2in_D.getValue(), conn_D);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_D_FF_TMIN::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(2, fb_E_D_FF->conn_Q.getValue(), conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_D_FF_TMIN::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_D.getValue();
      case 1: return &conn_if2in_Tmin.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_D_FF_TMIN::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_D_FF->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_D_FF_TMIN::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_D_FF_TMIN::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_D;
      case 1: return &conn_Tmin;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_D_FF_TMIN::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_D_FF_TMIN::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_D;
      case 1: return &conn_if2in_Tmin;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
