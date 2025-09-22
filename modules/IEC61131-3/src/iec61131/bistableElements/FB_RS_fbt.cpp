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

#include "forte/iec61131/bistableElements/FB_RS_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::bistableElements {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"S"_STRID, "R1"_STRID};
    const auto cDataOutputNames = std::array{"Q1"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_FB_RS, "iec61131::bistableElements::FB_RS"_STRID)

  FORTE_FB_RS::FORTE_FB_RS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_S(0_BOOL),
      var_R1(0_BOOL),
      var_Q1(0_BOOL),
      conn_CNF(*this, 0),
      conn_S(nullptr),
      conn_R1(nullptr),
      conn_Q1(*this, 0, var_Q1) {
  }

  void FORTE_FB_RS::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_S = 0_BOOL;
    var_R1 = 0_BOOL;
    var_Q1 = 0_BOOL;
  }

  void FORTE_FB_RS::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_FB_RS::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_FB_RS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_S, conn_S);
        readData(1, var_R1, conn_R1);
        break;
      }
      default: break;
    }
  }

  void FORTE_FB_RS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_Q1, conn_Q1);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FB_RS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_S;
      case 1: return &var_R1;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_RS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q1;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FB_RS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FB_RS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_S;
      case 1: return &conn_R1;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FB_RS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q1;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_RS::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_FB_RS::alg_REQ(void) {

#line 2 "FB_RS.fbt"
    var_Q1 = func_AND<CIEC_BOOL>(func_NOT<CIEC_BOOL>(var_R1), func_OR<CIEC_BOOL>(var_S, var_Q1));
  }

} // namespace forte::iec61131::bistableElements
