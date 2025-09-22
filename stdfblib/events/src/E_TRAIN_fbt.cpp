/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, HR Agrartechnik GmbH
 *
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
 *** Name: E_TRAIN
 *** Description: Generate of a finite train of events seperated by the time DT
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2025-02-15/Franz Höpfinger -  HR Agrartechnik GmbH   - Bugfix
 *************************************************************************/

#include "forte/iec61499/events/E_TRAIN_fbt.h"

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
        {"CTR"_STRID, "CUO"_STRID, "GATE"_STRID, "EI"_STRID},
        {"CTR"_STRID, "RO"_STRID, "GATE"_STRID, "EI"_STRID},
        {{}, "START"_STRID, "CTR"_STRID, "R"_STRID},
        {"GATE"_STRID, "EO0"_STRID, "DLY"_STRID, "START"_STRID},
        {{}, "STOP"_STRID, "DLY"_STRID, "STOP"_STRID},
        {"DLY"_STRID, "EO"_STRID, {}, "EO"_STRID},
        {"DLY"_STRID, "EO"_STRID, "CTR"_STRID, "CU"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "N"_STRID, "CTR"_STRID, "PV"_STRID},
        {"CTR"_STRID, "CV"_STRID, {}, "CV"_STRID},
        {{}, "DT"_STRID, "DLY"_STRID, "DT"_STRID},
        {"CTR"_STRID, "Q"_STRID, "GATE"_STRID, "G"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, "iec61499::events::E_TRAIN"_STRID)

  FORTE_E_TRAIN::FORTE_E_TRAIN(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_CTR("CTR"_STRID, *this),
      fb_GATE("GATE"_STRID, *this),
      fb_DLY("DLY"_STRID, *this),
      conn_EO(*this, 0),
      conn_DT(nullptr),
      conn_N(nullptr),
      conn_CV(*this, 0, 0_UINT),
      conn_if2in_DT(*this, 0, 0_TIME),
      conn_if2in_N(*this, 1, 0_UINT) {};

  void FORTE_E_TRAIN::setInitialValues() {
    conn_if2in_DT.getValue() = 0_TIME;
    conn_if2in_N.getValue() = 0_UINT;
    fb_CTR->conn_CV.getValue() = 0_UINT;
  }

  void FORTE_E_TRAIN::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, conn_if2in_DT.getValue(), conn_DT);
        readData(1, conn_if2in_N.getValue(), conn_N);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TRAIN::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_CTR->conn_CV.getValue(), conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_TRAIN::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT.getValue();
      case 1: return &conn_if2in_N.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TRAIN::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_CTR->conn_CV.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TRAIN::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TRAIN::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
      case 1: return &conn_N;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TRAIN::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT;
      case 1: return &conn_if2in_N;
    }
    return nullptr;
  }

} // namespace forte::iec61499::events
