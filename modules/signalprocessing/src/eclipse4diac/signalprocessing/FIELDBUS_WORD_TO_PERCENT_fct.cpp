/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: FIELDBUS_WORD_TO_PERCENT
 *** Description: Convert a WORD Value Range 0-FAFF to a REAL in the Range 0.0% to 100.0% (0.0 to 1.0)
 *** Version:
 ***     3.0.1: 2026-08-27/Franz Höpfinger - HR Agrartechnik GmbH - updated description, cleaned up formatting, and
 * qualified PACKAGE declaration with eclipse4diac:: prefix to match CompilerInfo packageName
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Update to a more Function like Interface
 ***     1.0: 2023-10-21/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/FIELDBUS_WORD_TO_PERCENT_fct.h"

#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_SIGNAL_gcf.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_WORD_TO_PERCENT_fct.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_DIV.h"
#include "forte/iec61131_functions/func_LE.h"
#include "forte/iec61131_functions/func_UDINT_TO_REAL.h"
#include "forte/iec61131_functions/func_WORD_TO_UDINT.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"WI"_STRID};
    const auto cDataOutputNames = std::array{""_STRID, "WO"_STRID};
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

    CIEC_REAL func_FIELDBUS_WORD_TO_PERCENT__impl(const CIEC_WORD &st_lv_WI, CIEC_WORD &st_lv_WO);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_FIELDBUS_WORD_TO_PERCENT,
                     "eclipse4diac::signalprocessing::FIELDBUS_WORD_TO_PERCENT"_STRID,
                     TypeHash)

  FORTE_FIELDBUS_WORD_TO_PERCENT::FORTE_FIELDBUS_WORD_TO_PERCENT(const StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_WI(0_WORD),
      var_(0_REAL),
      var_WO(0_WORD),
      conn_CNF(*this, 0),
      conn_WI(nullptr),
      conn_(*this, 0, var_),
      conn_WO(*this, 1, var_WO) {
  }

  void FORTE_FIELDBUS_WORD_TO_PERCENT::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_WI = 0_WORD;
    var_ = 0_REAL;
    var_WO = 0_WORD;
  }

  void FORTE_FIELDBUS_WORD_TO_PERCENT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_WI, conn_WI);
        break;
      }
      default: break;
    }
  }

  void FORTE_FIELDBUS_WORD_TO_PERCENT::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_, conn_);
        writeData(2, var_WO, conn_WO);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FIELDBUS_WORD_TO_PERCENT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_WI;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FIELDBUS_WORD_TO_PERCENT::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
      case 1: return &var_WO;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FIELDBUS_WORD_TO_PERCENT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FIELDBUS_WORD_TO_PERCENT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_WI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FIELDBUS_WORD_TO_PERCENT::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
      case 1: return &conn_WO;
    }
    return nullptr;
  }

  void FORTE_FIELDBUS_WORD_TO_PERCENT::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_FIELDBUS_WORD_TO_PERCENT__impl(var_WI, var_WO);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_REAL func_FIELDBUS_WORD_TO_PERCENT(const CIEC_WORD &st_lv_WI, CAnyBitOutputParameter<CIEC_WORD> st_lv_WO) {
    COutputGuard st_guard_WO(st_lv_WO);
    return func_FIELDBUS_WORD_TO_PERCENT__impl(st_lv_WI, *st_lv_WO);
  }

  namespace {
    CIEC_REAL func_FIELDBUS_WORD_TO_PERCENT__impl(const CIEC_WORD &st_lv_WI, CIEC_WORD &st_lv_WO) {
      CIEC_REAL st_ret_val = 0_REAL;
      st_lv_WO = 0_WORD;

#line 13 "FIELDBUS_WORD_TO_PERCENT.fct"
      if (func_LE(st_lv_WI, forte::eclipse4diac::signalprocessing::FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)) {
#line 14 "FIELDBUS_WORD_TO_PERCENT.fct"
        st_lv_WO = st_lv_WI;
#line 15 "FIELDBUS_WORD_TO_PERCENT.fct"
        st_ret_val =
            func_DIV<CIEC_REAL>(func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_lv_WO)),
                                func_UDINT_TO_REAL(func_WORD_TO_UDINT(
                                    forte::eclipse4diac::signalprocessing::FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)));
      }

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::signalprocessing
