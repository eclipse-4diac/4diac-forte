/*************************************************************************
 * Copyright (c) 2024
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 *
 * Name: TIMESTAMP_NS
 * Description: Creates by default a Unix-Epoch-Timestamp in nanoseconds. Use other start dates for other timestamp
 * types. Version: 3.0.1: 2026-09-02/Franz Höpfinger - HR Agrartechnik GmbH - add missing PACKAGE statement, improved
 * comments 3.0: 2025-04-14/Patrick Aigner -  - changed package 1.0: 2024-10-14/Monika Wenger -  -
 *************************************************************************/

#include "forte/eclipse4diac/utils/timing/TIMESTAMP_NS_fct.h"

#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/eclipse4diac/utils/timing/TIMESTAMP_NS_fct.h"
#include "forte/iec61131_functions/func_NOW.h"
#include "forte/iec61131_functions/func_SUB.h"
#include "forte/iec61131_functions/func_TIME_IN_NS_TO_ULINT.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::timing {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"startDate"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
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

    CIEC_ULINT func_TIMESTAMP_NS__impl(const CIEC_DATE_AND_TIME &st_lv_startDate);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_TIMESTAMP_NS, "eclipse4diac::utils::timing::TIMESTAMP_NS"_STRID, TypeHash)

  FORTE_TIMESTAMP_NS::FORTE_TIMESTAMP_NS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_startDate(0_DATE_AND_TIME),
      var_(0_ULINT),
      conn_CNF(*this, 0),
      conn_startDate(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_TIMESTAMP_NS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_startDate = 0_DATE_AND_TIME;
    var_ = 0_ULINT;
  }

  void FORTE_TIMESTAMP_NS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_startDate, conn_startDate);
        break;
      }
      default: break;
    }
  }

  void FORTE_TIMESTAMP_NS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_TIMESTAMP_NS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_startDate;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_TIMESTAMP_NS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_TIMESTAMP_NS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_TIMESTAMP_NS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_startDate;
    }
    return nullptr;
  }

  CDataConnection *FORTE_TIMESTAMP_NS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_TIMESTAMP_NS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_TIMESTAMP_NS__impl(var_startDate);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_ULINT func_TIMESTAMP_NS(const CIEC_DATE_AND_TIME &st_lv_startDate) {
    return func_TIMESTAMP_NS__impl(st_lv_startDate);
  }

  namespace {
    CIEC_ULINT func_TIMESTAMP_NS__impl(const CIEC_DATE_AND_TIME &st_lv_startDate) {
      CIEC_ULINT st_ret_val = 0_ULINT;

#line 8 "TIMESTAMP_NS.fct"
      st_ret_val = func_TIME_IN_NS_TO_ULINT(func_SUB<CIEC_TIME>(func_NOW(), st_lv_startDate));

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::timing
