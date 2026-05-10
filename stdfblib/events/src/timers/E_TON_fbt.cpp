/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
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
 *** Name: E_TON
 *** Description: standard timer function block (on-delay timing)
 *** Version:
 ***     3.1: 2026-05-08/Franz Höpfinger - HR Agrartechnik GmbH - E_RF_TRIG instead of E_SWITCH
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2024-03-04/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/iec61499/events/timers/E_TON_fbt.h"

#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events::timers {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID, "PT"_STRID};
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

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "REQ"_STRID, "E_RF_TRIG"_STRID, "EI"_STRID},
        {"E_RF_TRIG"_STRID, "ER"_STRID, "E_DELAY"_STRID, "START"_STRID},
        {"E_RF_TRIG"_STRID, "EF"_STRID, "E_DELAY"_STRID, "STOP"_STRID},
        {"E_DELAY"_STRID, "EO"_STRID, "E_SR"_STRID, "S"_STRID},
        {"E_RF_TRIG"_STRID, "EF"_STRID, "E_SR"_STRID, "R"_STRID},
        {"E_SR"_STRID, "EO"_STRID, {}, "CNF"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "IN"_STRID, "E_RF_TRIG"_STRID, "QI"_STRID},
        {{}, "PT"_STRID, "E_DELAY"_STRID, "DT"_STRID},
        {"E_SR"_STRID, "Q"_STRID, {}, "Q"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TON, "iec61499::events::timers::E_TON"_STRID, TypeHash)

  FORTE_E_TON::FORTE_E_TON(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_RF_TRIG("E_RF_TRIG"_STRID, *this),
      fb_E_DELAY("E_DELAY"_STRID, *this),
      fb_E_SR("E_SR"_STRID, *this),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_PT(nullptr),
      conn_Q(*this, 0, 0_BOOL),
      conn_if2in_IN(*this, 0, 0_BOOL),
      conn_if2in_PT(*this, 1, 0_TIME) {};

  void FORTE_E_TON::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_IN.getValue() = 0_BOOL;
    conn_if2in_PT.getValue() = 0_TIME;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_TON::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, conn_if2in_IN.getValue(), conn_IN);
        readData(1, conn_if2in_PT.getValue(), conn_PT);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TON::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, fb_E_SR->conn_Q.getValue(), conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_TON::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN.getValue();
      case 1: return &conn_if2in_PT.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TON::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_SR->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TON::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TON::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_PT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TON::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TON::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN;
      case 1: return &conn_if2in_PT;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events::timers
