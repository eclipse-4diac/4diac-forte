/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_N_TABLE
 *** Description: Generation of a finite train of sperate events
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.0: 2025-08-25/Martin Erich Jobst -  - remove erroneous F_SUB
 *************************************************************************/

#include "forte/iec61499/events/E_N_TABLE_fbt.h"

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cEventInputNames = std::array{"START"_STRID, "STOP"_STRID};
    const auto cEventOutputNames = std::array{"EO0"_STRID, "EO1"_STRID, "EO2"_STRID, "EO3"_STRID};
    const auto cDataInputNames = std::array{"DT"_STRID, "N"_STRID};

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
        {{}, "START"_STRID, "E_TABLE"_STRID, "START"_STRID},
        {{}, "STOP"_STRID, "E_TABLE"_STRID, "STOP"_STRID},
        {"E_TABLE"_STRID, "EO"_STRID, "E_DEMUX"_STRID, "EI"_STRID},
        {"E_DEMUX"_STRID, "EO0"_STRID, {}, "EO0"_STRID},
        {"E_DEMUX"_STRID, "EO1"_STRID, {}, "EO1"_STRID},
        {"E_DEMUX"_STRID, "EO2"_STRID, {}, "EO2"_STRID},
        {"E_DEMUX"_STRID, "EO3"_STRID, {}, "EO3"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "DT"_STRID, "E_TABLE"_STRID, "DT"_STRID},
        {{}, "N"_STRID, "E_TABLE"_STRID, "N"_STRID},
        {"E_TABLE"_STRID, "CV"_STRID, "E_DEMUX"_STRID, "K"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_N_TABLE, "iec61499::events::E_N_TABLE"_STRID)

  FORTE_E_N_TABLE::FORTE_E_N_TABLE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_TABLE("E_TABLE"_STRID, *this),
      fb_E_DEMUX("E_DEMUX"_STRID, *this),
      conn_EO0(*this, 0),
      conn_EO1(*this, 1),
      conn_EO2(*this, 2),
      conn_EO3(*this, 3),
      conn_DT(nullptr),
      conn_N(nullptr),
      conn_if2in_DT(*this, 0, CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
      conn_if2in_N(*this, 1, 0_UINT) {};

  void FORTE_E_N_TABLE::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_DT.getValue() = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
    conn_if2in_N.getValue() = 0_UINT;
  }

  void FORTE_E_N_TABLE::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, conn_if2in_DT.getValue(), conn_DT);
        readData(1, conn_if2in_N.getValue(), conn_N);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_N_TABLE::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_N_TABLE::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT.getValue();
      case 1: return &conn_if2in_N.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_N_TABLE::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_N_TABLE::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO0;
      case 1: return &conn_EO1;
      case 2: return &conn_EO2;
      case 3: return &conn_EO3;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_N_TABLE::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
      case 1: return &conn_N;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_N_TABLE::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_N_TABLE::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT;
      case 1: return &conn_if2in_N;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
