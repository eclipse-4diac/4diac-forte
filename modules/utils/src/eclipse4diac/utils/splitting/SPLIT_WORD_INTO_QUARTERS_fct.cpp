/*************************************************************************
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
 *** Name: SPLIT_WORD_INTO_QUARTERS
 *** Description: this Function extracts the 8 QUARTER BYTE from a word
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_WORD_INTO_QUARTERS_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_AND.h"
#include "forte/iec61131_functions/func_SHR.h"
#include "forte/iec61131_functions/func_WORD_TO_BYTE.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames =
        std::array{"QUARTER_BYTE_00"_STRID, "QUARTER_BYTE_01"_STRID, "QUARTER_BYTE_02"_STRID, "QUARTER_BYTE_03"_STRID,
                   "QUARTER_BYTE_04"_STRID, "QUARTER_BYTE_05"_STRID, "QUARTER_BYTE_06"_STRID, "QUARTER_BYTE_07"_STRID};
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

    void func_SPLIT_WORD_INTO_QUARTERS__impl(const CIEC_WORD &st_lv_IN,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_07);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_WORD_INTO_QUARTERS,
                     "eclipse4diac::utils::splitting::SPLIT_WORD_INTO_QUARTERS"_STRID,
                     TypeHash)

  FORTE_SPLIT_WORD_INTO_QUARTERS::FORTE_SPLIT_WORD_INTO_QUARTERS(const StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_WORD),
      var_QUARTER_BYTE_00(0_BYTE),
      var_QUARTER_BYTE_01(0_BYTE),
      var_QUARTER_BYTE_02(0_BYTE),
      var_QUARTER_BYTE_03(0_BYTE),
      var_QUARTER_BYTE_04(0_BYTE),
      var_QUARTER_BYTE_05(0_BYTE),
      var_QUARTER_BYTE_06(0_BYTE),
      var_QUARTER_BYTE_07(0_BYTE),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_QUARTER_BYTE_00(*this, 0, var_QUARTER_BYTE_00),
      conn_QUARTER_BYTE_01(*this, 1, var_QUARTER_BYTE_01),
      conn_QUARTER_BYTE_02(*this, 2, var_QUARTER_BYTE_02),
      conn_QUARTER_BYTE_03(*this, 3, var_QUARTER_BYTE_03),
      conn_QUARTER_BYTE_04(*this, 4, var_QUARTER_BYTE_04),
      conn_QUARTER_BYTE_05(*this, 5, var_QUARTER_BYTE_05),
      conn_QUARTER_BYTE_06(*this, 6, var_QUARTER_BYTE_06),
      conn_QUARTER_BYTE_07(*this, 7, var_QUARTER_BYTE_07) {
  }

  void FORTE_SPLIT_WORD_INTO_QUARTERS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_WORD;
    var_QUARTER_BYTE_00 = 0_BYTE;
    var_QUARTER_BYTE_01 = 0_BYTE;
    var_QUARTER_BYTE_02 = 0_BYTE;
    var_QUARTER_BYTE_03 = 0_BYTE;
    var_QUARTER_BYTE_04 = 0_BYTE;
    var_QUARTER_BYTE_05 = 0_BYTE;
    var_QUARTER_BYTE_06 = 0_BYTE;
    var_QUARTER_BYTE_07 = 0_BYTE;
  }

  void FORTE_SPLIT_WORD_INTO_QUARTERS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_WORD_INTO_QUARTERS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
        writeData(2, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
        writeData(3, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
        writeData(4, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
        writeData(5, var_QUARTER_BYTE_04, conn_QUARTER_BYTE_04);
        writeData(6, var_QUARTER_BYTE_05, conn_QUARTER_BYTE_05);
        writeData(7, var_QUARTER_BYTE_06, conn_QUARTER_BYTE_06);
        writeData(8, var_QUARTER_BYTE_07, conn_QUARTER_BYTE_07);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_WORD_INTO_QUARTERS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_WORD_INTO_QUARTERS::getDO(const size_t paIndex) {
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

  CEventConnection *FORTE_SPLIT_WORD_INTO_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_WORD_INTO_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_WORD_INTO_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
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

  void FORTE_SPLIT_WORD_INTO_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_WORD_INTO_QUARTERS__impl(var_IN, var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02,
                                        var_QUARTER_BYTE_03, var_QUARTER_BYTE_04, var_QUARTER_BYTE_05,
                                        var_QUARTER_BYTE_06, var_QUARTER_BYTE_07);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_WORD_INTO_QUARTERS(const CIEC_WORD &st_lv_IN,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_00,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_01,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_02,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_03,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_04,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_05,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_06,
                                     CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_07) {
    COutputGuard st_guard_QUARTER_BYTE_00(st_lv_QUARTER_BYTE_00);
    COutputGuard st_guard_QUARTER_BYTE_01(st_lv_QUARTER_BYTE_01);
    COutputGuard st_guard_QUARTER_BYTE_02(st_lv_QUARTER_BYTE_02);
    COutputGuard st_guard_QUARTER_BYTE_03(st_lv_QUARTER_BYTE_03);
    COutputGuard st_guard_QUARTER_BYTE_04(st_lv_QUARTER_BYTE_04);
    COutputGuard st_guard_QUARTER_BYTE_05(st_lv_QUARTER_BYTE_05);
    COutputGuard st_guard_QUARTER_BYTE_06(st_lv_QUARTER_BYTE_06);
    COutputGuard st_guard_QUARTER_BYTE_07(st_lv_QUARTER_BYTE_07);
    func_SPLIT_WORD_INTO_QUARTERS__impl(st_lv_IN, *st_lv_QUARTER_BYTE_00, *st_lv_QUARTER_BYTE_01,
                                        *st_lv_QUARTER_BYTE_02, *st_lv_QUARTER_BYTE_03, *st_lv_QUARTER_BYTE_04,
                                        *st_lv_QUARTER_BYTE_05, *st_lv_QUARTER_BYTE_06, *st_lv_QUARTER_BYTE_07);
  }

  namespace {
    void func_SPLIT_WORD_INTO_QUARTERS__impl(const CIEC_WORD &st_lv_IN,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                             CIEC_BYTE &st_lv_QUARTER_BYTE_07) {
      st_lv_QUARTER_BYTE_00 = 0_BYTE;
      st_lv_QUARTER_BYTE_01 = 0_BYTE;
      st_lv_QUARTER_BYTE_02 = 0_BYTE;
      st_lv_QUARTER_BYTE_03 = 0_BYTE;
      st_lv_QUARTER_BYTE_04 = 0_BYTE;
      st_lv_QUARTER_BYTE_05 = 0_BYTE;
      st_lv_QUARTER_BYTE_06 = 0_BYTE;
      st_lv_QUARTER_BYTE_07 = 0_BYTE;

#line 23 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_00 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_00),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00));
#line 24 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_01 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_01),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01));
#line 25 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_02 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_02),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02));
#line 26 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_03 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_03),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03));
#line 27 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_04 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_04),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_04));
#line 28 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_05 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_05),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_05));
#line 29 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_06 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_06),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_06));
#line 30 "SPLIT_WORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_07 = func_WORD_TO_BYTE(func_SHR(
          func_AND<CIEC_WORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_WORD_QUARTER_07),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_07));
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
