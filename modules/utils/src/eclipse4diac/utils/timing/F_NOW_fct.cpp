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
 *** Name: F_NOW
 *** Description: return the current time
 *** Version:
 ***     1.0: 2024-10-01/Franz Höpfinger - HR Agrartechnik GmbH    -
 *************************************************************************/

#include "forte/eclipse4diac/utils/timing/F_NOW_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_date_and_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/timing/F_NOW_fct.h"

namespace forte::eclipse4diac::utils::timing {
  DEFINE_FIRMWARE_FB(FORTE_F_NOW, "eclipse4diac::utils::timing::F_NOW"_STRID)
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

  FORTE_F_NOW::FORTE_F_NOW(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_NOW::setInitialValues() {
    var_ = 0_DATE_AND_TIME;
  }

  void FORTE_F_NOW::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_F_NOW::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_NOW::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_F_NOW::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_NOW::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_NOW::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_F_NOW::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_F_NOW::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_F_NOW();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_DATE_AND_TIME func_F_NOW() {
    CIEC_DATE_AND_TIME st_ret_val = 0_DATE_AND_TIME;

#line 5 "F_NOW.fct"
    st_ret_val = func_NOW();

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::utils::timing
