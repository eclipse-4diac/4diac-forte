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
 *** Name: E_TABLE
 *** Description: Composite Function Block Type
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "forte/iec61499/events/E_TABLE_fbt.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"DT"_STRID, "N"_STRID};
    const auto cDataOutputNames = std::array{"CV"_STRID};
    const auto cEventInputNames = std::array{"START"_STRID, "STOP"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
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
        {"E_TABLE_CTRL"_STRID, "CLKO"_STRID, "E_DELAY"_STRID, "START"_STRID},
        {"E_DELAY"_STRID, "EO"_STRID, {}, "EO"_STRID},
        {"E_DELAY"_STRID, "EO"_STRID, "E_TABLE_CTRL"_STRID, "CLK"_STRID},
        {{}, "START"_STRID, "E_TABLE_CTRL"_STRID, "START"_STRID},
        {{}, "STOP"_STRID, "E_DELAY"_STRID, "STOP"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {"E_TABLE_CTRL"_STRID, "DTO"_STRID, "E_DELAY"_STRID, "DT"_STRID},
        {"E_TABLE_CTRL"_STRID, "CV"_STRID, {}, "CV"_STRID},
        {{}, "DT"_STRID, "E_TABLE_CTRL"_STRID, "DT"_STRID},
        {{}, "N"_STRID, "E_TABLE_CTRL"_STRID, "N"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TABLE, "iec61499::events::E_TABLE"_STRID)

  FORTE_E_TABLE::FORTE_E_TABLE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_TABLE_CTRL("E_TABLE_CTRL"_STRID, *this),
      fb_E_DELAY("E_DELAY"_STRID, *this),
      conn_EO(*this, 0),
      conn_DT(nullptr),
      conn_N(nullptr),
      conn_CV(*this, 0, 0_UINT),
      conn_if2in_DT(*this, 0, CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
      conn_if2in_N(*this, 0, 0_UINT) {};

  void FORTE_E_TABLE::setInitialValues() {
    conn_if2in_DT.getValue() = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
    conn_if2in_N.getValue() = 0_UINT;
    fb_E_TABLE_CTRL->conn_CV.getValue() = 0_UINT;
  }

  void FORTE_E_TABLE::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, conn_if2in_DT.getValue(), conn_DT);
        readData(1, conn_if2in_N.getValue(), conn_N);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TABLE::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_TABLE_CTRL->conn_CV.getValue(), conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_TABLE::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT.getValue();
      case 1: return &conn_if2in_N.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TABLE::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_TABLE_CTRL->conn_CV.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TABLE::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TABLE::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
      case 1: return &conn_N;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TABLE::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TABLE::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT;
      case 1: return &conn_if2in_N;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
