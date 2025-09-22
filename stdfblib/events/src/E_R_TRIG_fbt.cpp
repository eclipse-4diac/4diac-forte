/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_R_TRIG_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID};
    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
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
        {"E_SWITCH"_STRID, "EO1"_STRID, {}, "EO"_STRID},
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

  DEFINE_FIRMWARE_FB(FORTE_E_R_TRIG, "iec61499::events::E_R_TRIG"_STRID)

  FORTE_E_R_TRIG::FORTE_E_R_TRIG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_D_FF("E_D_FF"_STRID, *this),
      fb_E_SWITCH("E_SWITCH"_STRID, *this),
      conn_EO(*this, 0),
      conn_QI(nullptr),
      conn_if2in_QI(*this, 0, 0_BOOL) {};

  void FORTE_E_R_TRIG::setInitialValues() {
    conn_if2in_QI.getValue() = 0_BOOL;
  }

  void FORTE_E_R_TRIG::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventEIID: {
        readData(0, conn_if2in_QI.getValue(), conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_R_TRIG::writeOutputData(TEventID) {
  }

  CIEC_ANY *FORTE_E_R_TRIG::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_R_TRIG::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_R_TRIG::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_R_TRIG::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_R_TRIG::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_R_TRIG::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
