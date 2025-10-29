/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TONOF
 *** Description: standard timer function block (on/off-delay timing)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 *************************************************************************/

#include "forte/iec61499/events/timers/E_TONOF_fbt.h"

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events::timers {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID, "PT_ON"_STRID, "PT_OFF"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID, "R"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};

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
        {{}, "REQ"_STRID, "E_SWITCH"_STRID, "EI"_STRID},
        {"E_SWITCH"_STRID, "EO1"_STRID, "E_DELAY_ON"_STRID, "START"_STRID},
        {"E_SWITCH"_STRID, "EO1"_STRID, "E_DELAY_OFF"_STRID, "STOP"_STRID},
        {"E_SWITCH"_STRID, "EO0"_STRID, "E_DELAY_ON"_STRID, "STOP"_STRID},
        {"E_SWITCH"_STRID, "EO0"_STRID, "E_DELAY_OFF"_STRID, "START"_STRID},
        {"E_DELAY_ON"_STRID, "EO"_STRID, "E_RS"_STRID, "S"_STRID},
        {"E_RS"_STRID, "EO"_STRID, {}, "CNF"_STRID},
        {"E_DELAY_OFF"_STRID, "EO"_STRID, "E_RS"_STRID, "R"_STRID},
        {{}, "R"_STRID, "E_RS"_STRID, "R"_STRID},
        {{}, "R"_STRID, "E_DELAY_OFF"_STRID, "STOP"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "IN"_STRID, "E_SWITCH"_STRID, "G"_STRID},
        {{}, "PT_ON"_STRID, "E_DELAY_ON"_STRID, "DT"_STRID},
        {"E_RS"_STRID, "Q"_STRID, {}, "Q"_STRID},
        {{}, "PT_OFF"_STRID, "E_DELAY_OFF"_STRID, "DT"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TONOF, "iec61499::events::timers::E_TONOF"_STRID)

  FORTE_E_TONOF::FORTE_E_TONOF(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_SWITCH("E_SWITCH"_STRID, *this),
      fb_E_DELAY_ON("E_DELAY_ON"_STRID, *this),
      fb_E_RS("E_RS"_STRID, *this),
      fb_E_DELAY_OFF("E_DELAY_OFF"_STRID, *this),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_PT_ON(nullptr),
      conn_PT_OFF(nullptr),
      conn_Q(*this, 0, 0_BOOL),
      conn_if2in_IN(*this, 0, 0_BOOL),
      conn_if2in_PT_ON(*this, 1, 0_TIME),
      conn_if2in_PT_OFF(*this, 2, 0_TIME) {};

  void FORTE_E_TONOF::setInitialValues() {
    conn_if2in_IN.getValue() = 0_BOOL;
    conn_if2in_PT_ON.getValue() = 0_TIME;
    conn_if2in_PT_OFF.getValue() = 0_TIME;
    fb_E_RS->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_TONOF::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, conn_if2in_IN.getValue(), conn_IN);
        readData(1, conn_if2in_PT_ON.getValue(), conn_PT_ON);
        readData(2, conn_if2in_PT_OFF.getValue(), conn_PT_OFF);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TONOF::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_RS->conn_Q.getValue(), conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_TONOF::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN.getValue();
      case 1: return &conn_if2in_PT_ON.getValue();
      case 2: return &conn_if2in_PT_OFF.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TONOF::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_RS->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TONOF::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TONOF::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_PT_ON;
      case 2: return &conn_PT_OFF;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TONOF::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TONOF::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN;
      case 1: return &conn_if2in_PT_ON;
      case 2: return &conn_if2in_PT_OFF;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events::timers
