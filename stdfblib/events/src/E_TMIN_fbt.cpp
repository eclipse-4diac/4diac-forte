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
 *** Name: E_TMIN
 *** Description: Forwards Events with a minimum inter-arrival time between EI events
 *** Version:
 ***     1.0: 2026-05-14/Franz Höpfinger - HR Agrartechnik GmbH - Initial API
 *************************************************************************/

#include "forte/iec61499/events/E_TMIN_fbt.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID, "EI"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "EO"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"Tmin"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {"E_DELAY"_STRID, "EO"_STRID, "E_REND"_STRID, "EI2"_STRID},
        {"E_REND"_STRID, "EO"_STRID, "E_DELAY"_STRID, "START"_STRID},
        {{}, "INIT"_STRID, {}, "INITO"_STRID},
        {{}, "INIT"_STRID, "E_REND"_STRID, "EI2"_STRID},
        {{}, "EI"_STRID, "E_REND"_STRID, "EI1"_STRID},
        {"E_REND"_STRID, "EO"_STRID, {}, "EO"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "Tmin"_STRID, "E_DELAY"_STRID, "DT"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TMIN, "iec61499::events::E_TMIN"_STRID, TypeHash)

  FORTE_E_TMIN::FORTE_E_TMIN(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_REND("E_REND"_STRID, *this),
      fb_E_DELAY("E_DELAY"_STRID, *this),
      conn_INITO(*this, 0),
      conn_EO(*this, 1),
      conn_Tmin(nullptr),
      conn_if2in_Tmin(*this, 0, 0_TIME) {};

  void FORTE_E_TMIN::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_Tmin.getValue() = 0_TIME;
  }

  void FORTE_E_TMIN::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, conn_if2in_Tmin.getValue(), conn_Tmin);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TMIN::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_TMIN::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_Tmin.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TMIN::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_TMIN::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TMIN::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Tmin;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TMIN::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_TMIN::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_Tmin;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
