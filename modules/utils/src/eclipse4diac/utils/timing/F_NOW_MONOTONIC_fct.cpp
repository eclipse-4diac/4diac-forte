/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: F_NOW_MONOTONIC
 *** Description: returns current monotonic clock value
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2024-10-01/Franz Höpfinger - HR Agrartechnik GmbH    -
 *************************************************************************/

#include "forte/eclipse4diac/utils/timing/F_NOW_MONOTONIC_fct.h"

#include "forte/datatypes/forte_time.h"
#include "forte/eclipse4diac/utils/timing/F_NOW_MONOTONIC_fct.h"
#include "forte/iec61131_functions/func_NOW_MONOTONIC.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::timing {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    CIEC_TIME func_F_NOW_MONOTONIC__impl();
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_F_NOW_MONOTONIC, "eclipse4diac::utils::timing::F_NOW_MONOTONIC"_STRID, TypeHash)

  FORTE_F_NOW_MONOTONIC::FORTE_F_NOW_MONOTONIC(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_(0_TIME),
      conn_CNF(*this, 0),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_NOW_MONOTONIC::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_ = 0_TIME;
  }

  void FORTE_F_NOW_MONOTONIC::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_F_NOW_MONOTONIC::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(0, var_, conn_);
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
    var_ = func_F_NOW_MONOTONIC__impl();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_TIME func_F_NOW_MONOTONIC() {
    return func_F_NOW_MONOTONIC__impl();
  }

  namespace {
    CIEC_TIME func_F_NOW_MONOTONIC__impl() {
      CIEC_TIME st_ret_val = 0_TIME;

#line 5 "F_NOW_MONOTONIC.fct"
      st_ret_val = func_NOW_MONOTONIC();

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::timing
