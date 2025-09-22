/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0

 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_NOW_MONOTONIC
 *** Description: return a clear monotonic time
 *** Version:
 ***     1.0: 2024-10-01/Franz Höpfinger - HR Agrartechnik GmbH    -
 *************************************************************************/

#include "forte/eclipse4diac/utils/timing/F_NOW_MONOTONIC_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/timing/F_NOW_MONOTONIC_fct.h"

namespace forte::eclipse4diac::utils::timing {
  namespace {
    const auto cDataOutputNames = std::array{""_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = {},
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_F_NOW_MONOTONIC, "eclipse4diac::utils::timing::F_NOW_MONOTONIC"_STRID)

  FORTE_F_NOW_MONOTONIC::FORTE_F_NOW_MONOTONIC(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_NOW_MONOTONIC::setInitialValues() {
    var_ = 0_TIME;
  }

  void FORTE_F_NOW_MONOTONIC::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_F_NOW_MONOTONIC::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_NOW_MONOTONIC::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_F_NOW_MONOTONIC::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_NOW_MONOTONIC::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_NOW_MONOTONIC::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_F_NOW_MONOTONIC::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_F_NOW_MONOTONIC::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_F_NOW_MONOTONIC();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_TIME func_F_NOW_MONOTONIC() {
    CIEC_TIME st_ret_val = 0_TIME;

#line 5 "F_NOW_MONOTONIC.fct"
    st_ret_val = func_NOW_MONOTONIC();

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::utils::timing
