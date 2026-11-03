/*************************************************************************
 *** Copyright (c) 2012, 2023 TU Wien ACIN, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: FIELDBUS_PERCENT_TO_WORD
 *** Description: Convert a REAL in the Range 0.0% to 100.0% (0.0 to 1.0) to a WORD Value Range 0-FAFF
 *** Version:
 ***     3.0.1: 2026-08-27/Franz Höpfinger - HR Agrartechnik GmbH - updated description, cleaned up formatting, and
 * qualified PACKAGE declaration with eclipse4diac:: prefix to match CompilerInfo packageName
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Update to a more Function like Interface
 ***     1.0: 2023-10-21/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/FIELDBUS_PERCENT_TO_WORD_fct.h"

#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_PERCENT_TO_WORD_fct.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_SIGNAL_gcf.h"
#include "forte/iec61131_functions/func_MUL.h"
#include "forte/iec61131_functions/func_REAL_TO_UDINT.h"
#include "forte/iec61131_functions/func_UDINT_TO_REAL.h"
#include "forte/iec61131_functions/func_UDINT_TO_WORD.h"
#include "forte/iec61131_functions/func_WORD_TO_UDINT.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"RI"_STRID};
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

    CIEC_WORD func_FIELDBUS_PERCENT_TO_WORD__impl(const CIEC_REAL &st_lv_RI);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_FIELDBUS_PERCENT_TO_WORD,
                     "eclipse4diac::signalprocessing::FIELDBUS_PERCENT_TO_WORD"_STRID,
                     TypeHash)

  FORTE_FIELDBUS_PERCENT_TO_WORD::FORTE_FIELDBUS_PERCENT_TO_WORD(const StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_RI(0_REAL),
      var_(0_WORD),
      conn_CNF(*this, 0),
      conn_RI(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_RI = 0_REAL;
    var_ = 0_WORD;
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_RI, conn_RI);
        break;
      }
      default: break;
    }
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FIELDBUS_PERCENT_TO_WORD::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_RI;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FIELDBUS_PERCENT_TO_WORD::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FIELDBUS_PERCENT_TO_WORD::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FIELDBUS_PERCENT_TO_WORD::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_RI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FIELDBUS_PERCENT_TO_WORD::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_FIELDBUS_PERCENT_TO_WORD__impl(var_RI);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WORD func_FIELDBUS_PERCENT_TO_WORD(const CIEC_REAL &st_lv_RI) {
    return func_FIELDBUS_PERCENT_TO_WORD__impl(st_lv_RI);
  }

  namespace {
    CIEC_WORD func_FIELDBUS_PERCENT_TO_WORD__impl(const CIEC_REAL &st_lv_RI) {
      CIEC_WORD st_ret_val = 0_WORD;

#line 9 "FIELDBUS_PERCENT_TO_WORD.fct"
      st_ret_val = func_UDINT_TO_WORD(func_REAL_TO_UDINT(func_MUL<CIEC_REAL>(
          st_lv_RI, func_UDINT_TO_REAL(func_WORD_TO_UDINT(
                        forte::eclipse4diac::signalprocessing::FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)))));

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::signalprocessing
