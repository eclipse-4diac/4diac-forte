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

#include "forte/iec61131/bistableElements/FB_SR_fbt.h"

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
    const auto cDataInputNames = std::array{"S1"_STRID, "R"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_FB_SR, "iec61131::bistableElements::FB_SR"_STRID)

  FORTE_FB_SR::FORTE_FB_SR(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_S1(0_BOOL),
      var_R(0_BOOL),
      var_Q1(0_BOOL),
      conn_CNF(*this, 0),
      conn_S1(nullptr),
      conn_R(nullptr),
      conn_Q1(*this, 0, var_Q1) {
  }

  void FORTE_FB_SR::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_S1 = 0_BOOL;
    var_R = 0_BOOL;
    var_Q1 = 0_BOOL;
  }

  void FORTE_FB_SR::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_FB_SR::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_FB_SR::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_S1, conn_S1);
        readData(1, var_R, conn_R);
        break;
      }
      default: break;
    }
  }

  void FORTE_FB_SR::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_Q1, conn_Q1);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FB_SR::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_S1;
      case 1: return &var_R;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_SR::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q1;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FB_SR::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FB_SR::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_S1;
      case 1: return &conn_R;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FB_SR::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q1;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_SR::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_FB_SR::alg_REQ(void) {

#line 2 "FB_SR.fbt"
    var_Q1 = func_OR<CIEC_BOOL>(var_S1, func_AND<CIEC_BOOL>(func_NOT<CIEC_BOOL>(var_R), var_Q1));
  }

} // namespace forte::iec61131::bistableElements
