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
 *** Name: E_RF_TRIG
 *** Description: Boolean falling and rising edge detection
 *** Version:
 ***     1.0: 2026-05-08/Franz Höpfinger - HR Agrartechnik GmbH - initial API and implementation and/or initial
 *documentation
 *************************************************************************/

#include "forte/iec61499/events/E_RF_TRIG_fbt.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventOutputNames = std::array{"ER"_STRID, "EF"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "EI"_STRID, "E_D_FF"_STRID, "CLK"_STRID},
        {"E_D_FF"_STRID, "EO"_STRID, "E_SWITCH"_STRID, "EI"_STRID},
        {"E_SWITCH"_STRID, "EO1"_STRID, {}, "ER"_STRID},
        {"E_SWITCH"_STRID, "EO0"_STRID, {}, "EF"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "QI"_STRID, "E_D_FF"_STRID, "D"_STRID},
        {"E_D_FF"_STRID, "Q"_STRID, "E_SWITCH"_STRID, "G"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_RF_TRIG, "iec61499::events::E_RF_TRIG"_STRID, TypeHash)

  FORTE_E_RF_TRIG::FORTE_E_RF_TRIG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_D_FF("E_D_FF"_STRID, *this),
      fb_E_SWITCH("E_SWITCH"_STRID, *this),
      conn_ER(*this, 0),
      conn_EF(*this, 1),
      conn_QI(nullptr),
      conn_if2in_QI(*this, 0, 0_BOOL) {};

  void FORTE_E_RF_TRIG::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_QI.getValue() = 0_BOOL;
  }

  void FORTE_E_RF_TRIG::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEIID: {
        readData(0, conn_if2in_QI.getValue(), conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_RF_TRIG::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_RF_TRIG::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_RF_TRIG::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_RF_TRIG::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_ER;
      case 1: return &conn_EF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_RF_TRIG::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_RF_TRIG::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_RF_TRIG::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
