/*******************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: ASSEMBLE_WORD_FROM_QUARTERS
 *** Description: this Function combines the 8 QUARTER BYTES to a WORD
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_WORD_FROM_QUARTERS_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_WORD_FROM_QUARTERS_fct.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"
#include "forte/iec61131_functions/func_BYTE_TO_WORD.h"
#include "forte/iec61131_functions/func_OR.h"
#include "forte/iec61131_functions/func_SHL.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames =
        std::array{"QUARTER_BYTE_00"_STRID, "QUARTER_BYTE_01"_STRID, "QUARTER_BYTE_02"_STRID, "QUARTER_BYTE_03"_STRID,
                   "QUARTER_BYTE_04"_STRID, "QUARTER_BYTE_05"_STRID, "QUARTER_BYTE_06"_STRID, "QUARTER_BYTE_07"_STRID};
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

    CIEC_WORD func_ASSEMBLE_WORD_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_07);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_WORD_FROM_QUARTERS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_WORD_FROM_QUARTERS"_STRID,
                     TypeHash)

  FORTE_ASSEMBLE_WORD_FROM_QUARTERS::FORTE_ASSEMBLE_WORD_FROM_QUARTERS(const StringId paInstanceNameId,
                                                                       CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QUARTER_BYTE_00(0_BYTE),
      var_QUARTER_BYTE_01(0_BYTE),
      var_QUARTER_BYTE_02(0_BYTE),
      var_QUARTER_BYTE_03(0_BYTE),
      var_QUARTER_BYTE_04(0_BYTE),
      var_QUARTER_BYTE_05(0_BYTE),
      var_QUARTER_BYTE_06(0_BYTE),
      var_QUARTER_BYTE_07(0_BYTE),
      var_(0_WORD),
      conn_CNF(*this, 0),
      conn_QUARTER_BYTE_00(nullptr),
      conn_QUARTER_BYTE_01(nullptr),
      conn_QUARTER_BYTE_02(nullptr),
      conn_QUARTER_BYTE_03(nullptr),
      conn_QUARTER_BYTE_04(nullptr),
      conn_QUARTER_BYTE_05(nullptr),
      conn_QUARTER_BYTE_06(nullptr),
      conn_QUARTER_BYTE_07(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QUARTER_BYTE_00 = 0_BYTE;
    var_QUARTER_BYTE_01 = 0_BYTE;
    var_QUARTER_BYTE_02 = 0_BYTE;
    var_QUARTER_BYTE_03 = 0_BYTE;
    var_QUARTER_BYTE_04 = 0_BYTE;
    var_QUARTER_BYTE_05 = 0_BYTE;
    var_QUARTER_BYTE_06 = 0_BYTE;
    var_QUARTER_BYTE_07 = 0_BYTE;
    var_ = 0_WORD;
  }

  void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
        readData(1, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
        readData(2, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
        readData(3, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
        readData(4, var_QUARTER_BYTE_04, conn_QUARTER_BYTE_04);
        readData(5, var_QUARTER_BYTE_05, conn_QUARTER_BYTE_05);
        readData(6, var_QUARTER_BYTE_06, conn_QUARTER_BYTE_06);
        readData(7, var_QUARTER_BYTE_07, conn_QUARTER_BYTE_07);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(8, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QUARTER_BYTE_00;
      case 1: return &var_QUARTER_BYTE_01;
      case 2: return &var_QUARTER_BYTE_02;
      case 3: return &var_QUARTER_BYTE_03;
      case 4: return &var_QUARTER_BYTE_04;
      case 5: return &var_QUARTER_BYTE_05;
      case 6: return &var_QUARTER_BYTE_06;
      case 7: return &var_QUARTER_BYTE_07;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QUARTER_BYTE_00;
      case 1: return &conn_QUARTER_BYTE_01;
      case 2: return &conn_QUARTER_BYTE_02;
      case 3: return &conn_QUARTER_BYTE_03;
      case 4: return &conn_QUARTER_BYTE_04;
      case 5: return &conn_QUARTER_BYTE_05;
      case 6: return &conn_QUARTER_BYTE_06;
      case 7: return &conn_QUARTER_BYTE_07;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_WORD_FROM_QUARTERS__impl(var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02,
                                                  var_QUARTER_BYTE_03, var_QUARTER_BYTE_04, var_QUARTER_BYTE_05,
                                                  var_QUARTER_BYTE_06, var_QUARTER_BYTE_07);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WORD func_ASSEMBLE_WORD_FROM_QUARTERS(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_07) {
    return func_ASSEMBLE_WORD_FROM_QUARTERS__impl(st_lv_QUARTER_BYTE_00, st_lv_QUARTER_BYTE_01, st_lv_QUARTER_BYTE_02,
                                                  st_lv_QUARTER_BYTE_03, st_lv_QUARTER_BYTE_04, st_lv_QUARTER_BYTE_05,
                                                  st_lv_QUARTER_BYTE_06, st_lv_QUARTER_BYTE_07);
  }

  namespace {
    CIEC_WORD func_ASSEMBLE_WORD_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_07) {
      CIEC_WORD st_ret_val = 0_WORD;

#line 19 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_ZZ;
#line 20 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_07),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_07));
#line 22 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_06),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_06));
#line 24 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_05),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_05));
#line 26 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_04),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_04));
#line 28 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_03),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03));
#line 30 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_02),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02));
#line 32 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_01),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01));
#line 34 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_WORD>(
          st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_00),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00));

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::assembling
