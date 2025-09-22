/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61131/counters/FB_CTD_ULINT_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::counters {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"CD"_STRID, "LD"_STRID, "PV"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID, "CV"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_FB_CTD_ULINT, "iec61131::counters::FB_CTD_ULINT"_STRID)

  FORTE_FB_CTD_ULINT::FORTE_FB_CTD_ULINT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_CD(0_BOOL),
      var_LD(0_BOOL),
      var_PV(0_ULINT),
      var_Q(0_BOOL),
      var_CV(0_ULINT),
      conn_CNF(*this, 0),
      conn_CD(nullptr),
      conn_LD(nullptr),
      conn_PV(nullptr),
      conn_Q(*this, 0, var_Q),
      conn_CV(*this, 1, var_CV) {
  }

  void FORTE_FB_CTD_ULINT::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_CD = 0_BOOL;
    var_LD = 0_BOOL;
    var_PV = 0_ULINT;
    var_Q = 0_BOOL;
    var_CV = 0_ULINT;
  }

  void FORTE_FB_CTD_ULINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_FB_CTD_ULINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_FB_CTD_ULINT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_CD, conn_CD);
        readData(2, var_PV, conn_PV);
        readData(1, var_LD, conn_LD);
        break;
      }
      default: break;
    }
  }

  void FORTE_FB_CTD_ULINT::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(3, var_Q, conn_Q);
        writeData(4, var_CV, conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FB_CTD_ULINT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_CD;
      case 1: return &var_LD;
      case 2: return &var_PV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_CTD_ULINT::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
      case 1: return &var_CV;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FB_CTD_ULINT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FB_CTD_ULINT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CD;
      case 1: return &conn_LD;
      case 2: return &conn_PV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FB_CTD_ULINT::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
      case 1: return &conn_CV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_CTD_ULINT::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_FB_CTD_ULINT::alg_REQ(void) {

#line 2 "FB_CTD_ULINT.fbt"
    if (var_LD) {
#line 3 "FB_CTD_ULINT.fbt"
      var_CV = var_PV;
    } else if (func_AND<CIEC_BOOL>(var_CD, func_GT(var_CV, 0_ULINT))) {
#line 5 "FB_CTD_ULINT.fbt"
      var_CV = func_SUB<CIEC_ULINT>(var_CV, 1_ULINT);
    }
#line 7 "FB_CTD_ULINT.fbt"
    var_Q = func_LE(var_CV, 0_ULINT);
  }

} // namespace forte::iec61131::counters
