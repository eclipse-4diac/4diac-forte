/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_BLINK
 *** Description: Simulates a blinking signal (turning on and off for specific durations)
 *** Version:
 ***     1.0: 2025-02-15/Franz Höpfinger - HR Agrartechnik GmbH   - initial API and implementation and/or initial
 *documentation
 *************************************************************************/

#include "forte/eclipse4diac/utils/signals/E_BLINK_fbt.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::signals {
  namespace {
    const auto cDataInputNames = std::array{"TIMELOW"_STRID, "TIMEHIGH"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"START"_STRID, "STOP"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
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
        {"E_CYCLE"_STRID, "EO"_STRID, "E_TP"_STRID, "REQ"_STRID},
        {{}, "START"_STRID, "ADD_2"_STRID, "REQ"_STRID},
        {"ADD_2"_STRID, "CNF"_STRID, "E_CYCLE"_STRID, "START"_STRID},
        {{}, "STOP"_STRID, "E_CYCLE"_STRID, "STOP"_STRID},
        {{}, "STOP"_STRID, "E_TP"_STRID, "R"_STRID},
        {"E_TP"_STRID, "CNF"_STRID, {}, "CNF"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {"ADD_2"_STRID, "OUT"_STRID, "E_CYCLE"_STRID, "DT"_STRID},
        {{}, "TIMELOW"_STRID, "ADD_2"_STRID, "IN1"_STRID},
        {{}, "TIMEHIGH"_STRID, "ADD_2"_STRID, "IN2"_STRID},
        {{}, "TIMEHIGH"_STRID, "E_TP"_STRID, "PT"_STRID},
        {"E_TP"_STRID, "Q"_STRID, {}, "OUT"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_BLINK, "eclipse4diac::utils::signals::E_BLINK"_STRID)

  FORTE_E_BLINK::FORTE_E_BLINK(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_E_TP("E_TP"_STRID, *this),
      fb_E_CYCLE("E_CYCLE"_STRID, *this),
      fb_ADD_2("ADD_2"_STRID, "ADD_2", *this),
      conn_CNF(*this, 0),
      conn_TIMELOW(nullptr),
      conn_TIMEHIGH(nullptr),
      conn_OUT(*this, 0, 0_BOOL),
      conn_if2in_TIMELOW(*this, 0, 0_TIME),
      conn_if2in_TIMEHIGH(*this, 1, 0_TIME) {};

  void FORTE_E_BLINK::setInitialValues() {
    conn_if2in_TIMELOW.getValue() = 0_TIME;
    conn_if2in_TIMEHIGH.getValue() = 0_TIME;
    fb_E_TP->conn_Q.getValue() = 0_BOOL;
  }

  void FORTE_E_BLINK::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, conn_if2in_TIMELOW.getValue(), conn_TIMELOW);
        readData(1, conn_if2in_TIMEHIGH.getValue(), conn_TIMEHIGH);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_BLINK::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_TP->conn_Q.getValue(), conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_BLINK::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_TIMELOW.getValue();
      case 1: return &conn_if2in_TIMEHIGH.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_BLINK::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_E_TP->conn_Q.getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_BLINK::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_BLINK::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_TIMELOW;
      case 1: return &conn_TIMEHIGH;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_BLINK::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_BLINK::getIf2InConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_TIMELOW;
      case 1: return &conn_if2in_TIMEHIGH;
    }
    return nullptr;
  }
} // namespace forte::eclipse4diac::utils::signals
