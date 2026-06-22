/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.0.202606092002!
 ***
 *** Name: E_PULSE
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     3.1: 2026-06-22/Franz Höpfinger - HR Agrartechnik GmbH - validate all timers
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-04-23/Franz Höpfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 ***     1.0.1: 2024-03-05/Franz Höpfinger - HR Agrartechnik GmbH - renamed to E_PULSE
 ***     1.0: 2023-08-21/Franz Höpfinger - HR Agrartechnik GmbH - initial implementation as E_IMPULSE
 *************************************************************************/

#include "forte/iec61499/events/timers/E_PULSE_fbt.h"

#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events::timers {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID, "R"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"PT"_STRID};
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
        {{}, "REQ"_STRID, "E_DELAY"_STRID, "START"_STRID},
        {{}, "REQ"_STRID, "E_SR"_STRID, "S"_STRID},
        {"E_DELAY"_STRID, "EO"_STRID, "E_SR"_STRID, "R"_STRID},
        {"E_SR"_STRID, "EO"_STRID, {}, "CNF"_STRID},
        {{}, "R"_STRID, "E_DELAY"_STRID, "STOP"_STRID},
        {{}, "R"_STRID, "E_SR"_STRID, "R"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "PT"_STRID, "E_DELAY"_STRID, "DT"_STRID},
        {"E_SR"_STRID, "Q"_STRID, {}, "Q"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_PULSE, "iec61499::events::timers::E_PULSE"_STRID, TypeHash)

  FORTE_E_PULSE::FORTE_E_PULSE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_DELAY("E_DELAY"_STRID, *this),
      fb_E_SR("E_SR"_STRID, *this),
      conn_CNF(*this, 0),
      conn_PT(nullptr),
      conn_Q(*this, 0, 0_BOOL),
      conn_if2in_PT(*this, 0, 0_TIME) {};

  void FORTE_E_PULSE::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_PT.getValue() = 0_TIME;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_PULSE::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, conn_if2in_PT.getValue(), conn_PT);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_PULSE::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, fb_E_SR->conn_Q.getValue(), conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_PULSE::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_PT.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_PULSE::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_SR->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_PULSE::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_PULSE::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_PT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_PULSE::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_PULSE::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_PT;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events::timers
