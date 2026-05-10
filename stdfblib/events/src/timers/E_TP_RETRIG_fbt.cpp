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
 *** Name: E_TP_RETRIG
 *** Description: standard timer function block (pulse) - retriggerable version
 *** Version:
 ***     1.1: 2026-04-19/Franz Höpfinger - HR Agrartechnik GmbH - retriggerable version
 *************************************************************************/

#include "forte/iec61499/events/timers/E_TP_RETRIG_fbt.h"

#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events::timers {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID, "R"_STRID};
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
        {"E_SR"_STRID, "EO"_STRID, {}, "CNF"_STRID},
        {{}, "R"_STRID, "E_DELAY"_STRID, "STOP"_STRID},
        {{}, "R"_STRID, "E_SR"_STRID, "R"_STRID},
        {"E_DELAY"_STRID, "EO"_STRID, "E_SR"_STRID, "R"_STRID},
        {{}, "REQ"_STRID, "E_R_TRIG"_STRID, "EI"_STRID},
        {"E_R_TRIG"_STRID, "EO"_STRID, "E_SR"_STRID, "S"_STRID},
        {"E_R_TRIG"_STRID, "EO"_STRID, "E_DELAY"_STRID, "START"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "PT"_STRID, "E_DELAY"_STRID, "DT"_STRID},
        {"E_SR"_STRID, "Q"_STRID, {}, "Q"_STRID},
        {{}, "IN"_STRID, "E_R_TRIG"_STRID, "QI"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TP_RETRIG, "iec61499::events::timers::E_TP_RETRIG"_STRID, TypeHash)

  FORTE_E_TP_RETRIG::FORTE_E_TP_RETRIG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_DELAY("E_DELAY"_STRID, *this),
      fb_E_SR("E_SR"_STRID, *this),
      fb_E_R_TRIG("E_R_TRIG"_STRID, *this),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_PT(nullptr),
      conn_Q(*this, 0, 0_BOOL),
      conn_if2in_IN(*this, 0, 0_BOOL),
      conn_if2in_PT(*this, 1, 0_TIME) {};

  void FORTE_E_TP_RETRIG::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_IN.getValue() = 0_BOOL;
    conn_if2in_PT.getValue() = 0_TIME;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_TP_RETRIG::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, conn_if2in_IN.getValue(), conn_IN);
        readData(1, conn_if2in_PT.getValue(), conn_PT);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TP_RETRIG::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, fb_E_SR->conn_Q.getValue(), conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_TP_RETRIG::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN.getValue();
      case 1: return &conn_if2in_PT.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TP_RETRIG::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_SR->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TP_RETRIG::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TP_RETRIG::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_PT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TP_RETRIG::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TP_RETRIG::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_IN;
      case 1: return &conn_if2in_PT;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events::timers
