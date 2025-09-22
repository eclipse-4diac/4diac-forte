/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/eclipse4diac/rtevents/RT_E_TRAIN_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::rtevents {
  namespace {
    const auto cDataInputNames = std::array{"DT"_STRID, "N"_STRID, "Deadline"_STRID, "WCET"_STRID};
    const auto cDataOutputNames = std::array{"CV"_STRID};
    const auto cEventInputNames = std::array{"START"_STRID, "STOP"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
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
        {"RT_E_CYCLE"_STRID, "EO"_STRID, "E_CTU"_STRID, "CU"_STRID},
        {{}, "START"_STRID, "E_CTU"_STRID, "R"_STRID},
        {{}, "STOP"_STRID, "RT_E_CYCLE"_STRID, "STOP"_STRID},
        {"E_CTU"_STRID, "RO"_STRID, "RT_E_CYCLE"_STRID, "START"_STRID},
        {"E_CTU"_STRID, "CUO"_STRID, "E_SWITCH"_STRID, "EI"_STRID},
        {"E_SWITCH"_STRID, "EO0"_STRID, {}, "EO"_STRID},
        {"E_SWITCH"_STRID, "EO1"_STRID, "RT_E_CYCLE"_STRID, "STOP"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {{}, "DT"_STRID, "RT_E_CYCLE"_STRID, "DT"_STRID},
        {{}, "Deadline"_STRID, "RT_E_CYCLE"_STRID, "Deadline"_STRID},
        {{}, "WCET"_STRID, "RT_E_CYCLE"_STRID, "WCET"_STRID},
        {{}, "N"_STRID, "E_CTU"_STRID, "PV"_STRID},
        {"E_CTU"_STRID, "CV"_STRID, {}, "CV"_STRID},
        {"E_CTU"_STRID, "Q"_STRID, "E_SWITCH"_STRID, "G"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_RT_E_TRAIN, "eclipse4diac::rtevents::RT_E_TRAIN"_STRID)

  FORTE_RT_E_TRAIN::FORTE_RT_E_TRAIN(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_RT_E_CYCLE("RT_E_CYCLE"_STRID, *this),
      fb_E_CTU("E_CTU"_STRID, *this),
      fb_E_SWITCH("E_SWITCH"_STRID, *this),
      conn_EO(*this, 0),
      conn_DT(nullptr),
      conn_N(nullptr),
      conn_Deadline(nullptr),
      conn_WCET(nullptr),
      conn_CV(*this, 0, 0_UINT),
      conn_if2in_DT(*this, 0, 0_TIME),
      conn_if2in_N(*this, 1, 0_UINT),
      conn_if2in_Deadline(*this, 2, 0_TIME),
      conn_if2in_WCET(*this, 3, 0_TIME) {};

  void FORTE_RT_E_TRAIN::setInitialValues() {
    conn_if2in_DT.getValue() = 0_TIME;
    conn_if2in_N.getValue() = 0_UINT;
    conn_if2in_Deadline.getValue() = 0_TIME;
    conn_if2in_WCET.getValue() = 0_TIME;
    fb_E_CTU->conn_CV.getValue() = 0_UINT;
  }

  void FORTE_RT_E_TRAIN::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, conn_if2in_DT.getValue(), conn_DT);
        readData(1, conn_if2in_N.getValue(), conn_N);
        readData(2, conn_if2in_Deadline.getValue(), conn_Deadline);
        readData(3, conn_if2in_WCET.getValue(), conn_WCET);
        break;
      }
      case scmEventSTOPID: {
        break;
      }
      default: break;
    }
  }

  void FORTE_RT_E_TRAIN::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_CTU->conn_CV.getValue(), conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_RT_E_TRAIN::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT.getValue();
      case 1: return &conn_if2in_N.getValue();
      case 2: return &conn_if2in_Deadline.getValue();
      case 3: return &conn_if2in_WCET.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RT_E_TRAIN::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_CTU->conn_CV.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_RT_E_TRAIN::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_RT_E_TRAIN::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
      case 1: return &conn_N;
      case 2: return &conn_Deadline;
      case 3: return &conn_WCET;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RT_E_TRAIN::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RT_E_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_DT;
      case 1: return &conn_if2in_N;
      case 2: return &conn_if2in_Deadline;
      case 3: return &conn_if2in_WCET;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::rtevents
