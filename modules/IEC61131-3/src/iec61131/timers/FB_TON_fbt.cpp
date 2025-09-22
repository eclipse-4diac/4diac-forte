/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl,  Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/
#include "forte/iec61131/timers/FB_TON_fbt.h"

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::timers {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID, "PT"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID, "ET"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {.mEINames = cEventInputNames,
                                               .mEITypeNames = {},
                                               .mEONames = cEventOutputNames,
                                               .mEOTypeNames = {},
                                               .mDINames = cDataInputNames,
                                               .mDONames = cDataOutputNames,
                                               .mDIONames = {},
                                               .mSocketNames = {},
                                               .mPlugNames = {}};
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_FB_TON, "iec61131::timers::FB_TON"_STRID)

  FORTE_FB_TON::FORTE_FB_TON(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_BOOL),
      var_PT(0_TIME),
      var_Q(0_BOOL),
      var_ET(0_TIME),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_PT(nullptr),
      conn_Q(*this, 0, var_Q),
      conn_ET(*this, 1, var_ET) {};

  void FORTE_FB_TON::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_BOOL;
    var_PT = 0_TIME;
    var_Q = 0_BOOL;
    var_ET = 0_TIME;
  }

  void FORTE_FB_TON::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        if (!var_IN) {
          var_Q = false_BOOL;
          var_ET = 0_TIME;
          risingEdge = false;
          start = 0_TIME;
        } else {
          if (!risingEdge) {
            risingEdge = true;
            start = func_NOW_MONOTONIC();
          } else {
            count = func_SUB(func_NOW_MONOTONIC(), start);
            if (func_LE(var_PT, count)) {
              var_Q = true_BOOL;
              var_ET = var_PT;
            } else {
              var_ET = count;
            }
          }
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_FB_TON::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_PT, conn_PT);
        break;
      }
      default: break;
    }
  }

  void FORTE_FB_TON::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_Q, conn_Q);
        writeData(3, var_ET, conn_ET);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FB_TON::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
      case 1: return &var_PT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_TON::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
      case 1: return &var_ET;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FB_TON::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FB_TON::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_PT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FB_TON::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
      case 1: return &conn_ET;
    }
    return nullptr;
  }

} // namespace forte::iec61131::timers
