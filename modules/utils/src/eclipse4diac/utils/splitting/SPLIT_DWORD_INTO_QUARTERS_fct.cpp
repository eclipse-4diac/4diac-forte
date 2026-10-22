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
 *** Name: SPLIT_DWORD_INTO_QUARTERS
 *** Description: this Function extracts the 16 QUARTER BYTE from a dword
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_DWORD_INTO_QUARTERS_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_AND.h"
#include "forte/iec61131_functions/func_DWORD_TO_BYTE.h"
#include "forte/iec61131_functions/func_SHR.h"

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
                   "QUARTER_BYTE_04"_STRID, "QUARTER_BYTE_05"_STRID, "QUARTER_BYTE_06"_STRID, "QUARTER_BYTE_07"_STRID,
                   "QUARTER_BYTE_08"_STRID, "QUARTER_BYTE_09"_STRID, "QUARTER_BYTE_10"_STRID, "QUARTER_BYTE_11"_STRID,
                   "QUARTER_BYTE_12"_STRID, "QUARTER_BYTE_13"_STRID, "QUARTER_BYTE_14"_STRID, "QUARTER_BYTE_15"_STRID};
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

    void func_SPLIT_DWORD_INTO_QUARTERS__impl(const CIEC_DWORD &st_lv_IN,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_07,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_08,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_09,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_10,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_11,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_12,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_13,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_14,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_15);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_QUARTERS,
                     "eclipse4diac::utils::splitting::SPLIT_DWORD_INTO_QUARTERS"_STRID,
                     TypeHash)

  FORTE_SPLIT_DWORD_INTO_QUARTERS::FORTE_SPLIT_DWORD_INTO_QUARTERS(const StringId paInstanceNameId,
                                                                   CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_DWORD),
      var_QUARTER_BYTE_00(0_BYTE),
      var_QUARTER_BYTE_01(0_BYTE),
      var_QUARTER_BYTE_02(0_BYTE),
      var_QUARTER_BYTE_03(0_BYTE),
      var_QUARTER_BYTE_04(0_BYTE),
      var_QUARTER_BYTE_05(0_BYTE),
      var_QUARTER_BYTE_06(0_BYTE),
      var_QUARTER_BYTE_07(0_BYTE),
      var_QUARTER_BYTE_08(0_BYTE),
      var_QUARTER_BYTE_09(0_BYTE),
      var_QUARTER_BYTE_10(0_BYTE),
      var_QUARTER_BYTE_11(0_BYTE),
      var_QUARTER_BYTE_12(0_BYTE),
      var_QUARTER_BYTE_13(0_BYTE),
      var_QUARTER_BYTE_14(0_BYTE),
      var_QUARTER_BYTE_15(0_BYTE),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_QUARTER_BYTE_00(*this, 0, var_QUARTER_BYTE_00),
      conn_QUARTER_BYTE_01(*this, 1, var_QUARTER_BYTE_01),
      conn_QUARTER_BYTE_02(*this, 2, var_QUARTER_BYTE_02),
      conn_QUARTER_BYTE_03(*this, 3, var_QUARTER_BYTE_03),
      conn_QUARTER_BYTE_04(*this, 4, var_QUARTER_BYTE_04),
      conn_QUARTER_BYTE_05(*this, 5, var_QUARTER_BYTE_05),
      conn_QUARTER_BYTE_06(*this, 6, var_QUARTER_BYTE_06),
      conn_QUARTER_BYTE_07(*this, 7, var_QUARTER_BYTE_07),
      conn_QUARTER_BYTE_08(*this, 8, var_QUARTER_BYTE_08),
      conn_QUARTER_BYTE_09(*this, 9, var_QUARTER_BYTE_09),
      conn_QUARTER_BYTE_10(*this, 10, var_QUARTER_BYTE_10),
      conn_QUARTER_BYTE_11(*this, 11, var_QUARTER_BYTE_11),
      conn_QUARTER_BYTE_12(*this, 12, var_QUARTER_BYTE_12),
      conn_QUARTER_BYTE_13(*this, 13, var_QUARTER_BYTE_13),
      conn_QUARTER_BYTE_14(*this, 14, var_QUARTER_BYTE_14),
      conn_QUARTER_BYTE_15(*this, 15, var_QUARTER_BYTE_15) {
  }

  void FORTE_SPLIT_DWORD_INTO_QUARTERS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_DWORD;
    var_QUARTER_BYTE_00 = 0_BYTE;
    var_QUARTER_BYTE_01 = 0_BYTE;
    var_QUARTER_BYTE_02 = 0_BYTE;
    var_QUARTER_BYTE_03 = 0_BYTE;
    var_QUARTER_BYTE_04 = 0_BYTE;
    var_QUARTER_BYTE_05 = 0_BYTE;
    var_QUARTER_BYTE_06 = 0_BYTE;
    var_QUARTER_BYTE_07 = 0_BYTE;
    var_QUARTER_BYTE_08 = 0_BYTE;
    var_QUARTER_BYTE_09 = 0_BYTE;
    var_QUARTER_BYTE_10 = 0_BYTE;
    var_QUARTER_BYTE_11 = 0_BYTE;
    var_QUARTER_BYTE_12 = 0_BYTE;
    var_QUARTER_BYTE_13 = 0_BYTE;
    var_QUARTER_BYTE_14 = 0_BYTE;
    var_QUARTER_BYTE_15 = 0_BYTE;
  }

  void FORTE_SPLIT_DWORD_INTO_QUARTERS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_DWORD_INTO_QUARTERS::writeOutputData(const TEventID paEIID) {
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
        writeData(9, var_QUARTER_BYTE_08, conn_QUARTER_BYTE_08);
        writeData(10, var_QUARTER_BYTE_09, conn_QUARTER_BYTE_09);
        writeData(11, var_QUARTER_BYTE_10, conn_QUARTER_BYTE_10);
        writeData(12, var_QUARTER_BYTE_11, conn_QUARTER_BYTE_11);
        writeData(13, var_QUARTER_BYTE_12, conn_QUARTER_BYTE_12);
        writeData(14, var_QUARTER_BYTE_13, conn_QUARTER_BYTE_13);
        writeData(15, var_QUARTER_BYTE_14, conn_QUARTER_BYTE_14);
        writeData(16, var_QUARTER_BYTE_15, conn_QUARTER_BYTE_15);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_QUARTERS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_QUARTERS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QUARTER_BYTE_00;
      case 1: return &var_QUARTER_BYTE_01;
      case 2: return &var_QUARTER_BYTE_02;
      case 3: return &var_QUARTER_BYTE_03;
      case 4: return &var_QUARTER_BYTE_04;
      case 5: return &var_QUARTER_BYTE_05;
      case 6: return &var_QUARTER_BYTE_06;
      case 7: return &var_QUARTER_BYTE_07;
      case 8: return &var_QUARTER_BYTE_08;
      case 9: return &var_QUARTER_BYTE_09;
      case 10: return &var_QUARTER_BYTE_10;
      case 11: return &var_QUARTER_BYTE_11;
      case 12: return &var_QUARTER_BYTE_12;
      case 13: return &var_QUARTER_BYTE_13;
      case 14: return &var_QUARTER_BYTE_14;
      case 15: return &var_QUARTER_BYTE_15;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_DWORD_INTO_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_DWORD_INTO_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_DWORD_INTO_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QUARTER_BYTE_00;
      case 1: return &conn_QUARTER_BYTE_01;
      case 2: return &conn_QUARTER_BYTE_02;
      case 3: return &conn_QUARTER_BYTE_03;
      case 4: return &conn_QUARTER_BYTE_04;
      case 5: return &conn_QUARTER_BYTE_05;
      case 6: return &conn_QUARTER_BYTE_06;
      case 7: return &conn_QUARTER_BYTE_07;
      case 8: return &conn_QUARTER_BYTE_08;
      case 9: return &conn_QUARTER_BYTE_09;
      case 10: return &conn_QUARTER_BYTE_10;
      case 11: return &conn_QUARTER_BYTE_11;
      case 12: return &conn_QUARTER_BYTE_12;
      case 13: return &conn_QUARTER_BYTE_13;
      case 14: return &conn_QUARTER_BYTE_14;
      case 15: return &conn_QUARTER_BYTE_15;
    }
    return nullptr;
  }

  void FORTE_SPLIT_DWORD_INTO_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_DWORD_INTO_QUARTERS__impl(
        var_IN, var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02, var_QUARTER_BYTE_03, var_QUARTER_BYTE_04,
        var_QUARTER_BYTE_05, var_QUARTER_BYTE_06, var_QUARTER_BYTE_07, var_QUARTER_BYTE_08, var_QUARTER_BYTE_09,
        var_QUARTER_BYTE_10, var_QUARTER_BYTE_11, var_QUARTER_BYTE_12, var_QUARTER_BYTE_13, var_QUARTER_BYTE_14,
        var_QUARTER_BYTE_15);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_DWORD_INTO_QUARTERS(const CIEC_DWORD &st_lv_IN,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_00,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_01,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_02,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_03,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_04,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_05,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_06,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_07,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_08,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_09,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_10,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_11,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_12,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_13,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_14,
                                      CAnyBitOutputParameter<CIEC_BYTE> st_lv_QUARTER_BYTE_15) {
    COutputGuard st_guard_QUARTER_BYTE_00(st_lv_QUARTER_BYTE_00);
    COutputGuard st_guard_QUARTER_BYTE_01(st_lv_QUARTER_BYTE_01);
    COutputGuard st_guard_QUARTER_BYTE_02(st_lv_QUARTER_BYTE_02);
    COutputGuard st_guard_QUARTER_BYTE_03(st_lv_QUARTER_BYTE_03);
    COutputGuard st_guard_QUARTER_BYTE_04(st_lv_QUARTER_BYTE_04);
    COutputGuard st_guard_QUARTER_BYTE_05(st_lv_QUARTER_BYTE_05);
    COutputGuard st_guard_QUARTER_BYTE_06(st_lv_QUARTER_BYTE_06);
    COutputGuard st_guard_QUARTER_BYTE_07(st_lv_QUARTER_BYTE_07);
    COutputGuard st_guard_QUARTER_BYTE_08(st_lv_QUARTER_BYTE_08);
    COutputGuard st_guard_QUARTER_BYTE_09(st_lv_QUARTER_BYTE_09);
    COutputGuard st_guard_QUARTER_BYTE_10(st_lv_QUARTER_BYTE_10);
    COutputGuard st_guard_QUARTER_BYTE_11(st_lv_QUARTER_BYTE_11);
    COutputGuard st_guard_QUARTER_BYTE_12(st_lv_QUARTER_BYTE_12);
    COutputGuard st_guard_QUARTER_BYTE_13(st_lv_QUARTER_BYTE_13);
    COutputGuard st_guard_QUARTER_BYTE_14(st_lv_QUARTER_BYTE_14);
    COutputGuard st_guard_QUARTER_BYTE_15(st_lv_QUARTER_BYTE_15);
    func_SPLIT_DWORD_INTO_QUARTERS__impl(
        st_lv_IN, *st_lv_QUARTER_BYTE_00, *st_lv_QUARTER_BYTE_01, *st_lv_QUARTER_BYTE_02, *st_lv_QUARTER_BYTE_03,
        *st_lv_QUARTER_BYTE_04, *st_lv_QUARTER_BYTE_05, *st_lv_QUARTER_BYTE_06, *st_lv_QUARTER_BYTE_07,
        *st_lv_QUARTER_BYTE_08, *st_lv_QUARTER_BYTE_09, *st_lv_QUARTER_BYTE_10, *st_lv_QUARTER_BYTE_11,
        *st_lv_QUARTER_BYTE_12, *st_lv_QUARTER_BYTE_13, *st_lv_QUARTER_BYTE_14, *st_lv_QUARTER_BYTE_15);
  }

  namespace {
    void func_SPLIT_DWORD_INTO_QUARTERS__impl(const CIEC_DWORD &st_lv_IN,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_07,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_08,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_09,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_10,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_11,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_12,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_13,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_14,
                                              CIEC_BYTE &st_lv_QUARTER_BYTE_15) {
      st_lv_QUARTER_BYTE_00 = 0_BYTE;
      st_lv_QUARTER_BYTE_01 = 0_BYTE;
      st_lv_QUARTER_BYTE_02 = 0_BYTE;
      st_lv_QUARTER_BYTE_03 = 0_BYTE;
      st_lv_QUARTER_BYTE_04 = 0_BYTE;
      st_lv_QUARTER_BYTE_05 = 0_BYTE;
      st_lv_QUARTER_BYTE_06 = 0_BYTE;
      st_lv_QUARTER_BYTE_07 = 0_BYTE;
      st_lv_QUARTER_BYTE_08 = 0_BYTE;
      st_lv_QUARTER_BYTE_09 = 0_BYTE;
      st_lv_QUARTER_BYTE_10 = 0_BYTE;
      st_lv_QUARTER_BYTE_11 = 0_BYTE;
      st_lv_QUARTER_BYTE_12 = 0_BYTE;
      st_lv_QUARTER_BYTE_13 = 0_BYTE;
      st_lv_QUARTER_BYTE_14 = 0_BYTE;
      st_lv_QUARTER_BYTE_15 = 0_BYTE;

#line 31 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_00 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_00),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00));
#line 32 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_01 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_01),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01));
#line 33 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_02 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_02),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02));
#line 34 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_03 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_03),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03));
#line 35 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_04 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_04),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_04));
#line 36 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_05 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_05),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_05));
#line 37 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_06 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_06),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_06));
#line 38 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_07 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_07),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_07));
#line 39 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_08 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_08),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_08));
#line 40 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_09 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_09),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_09));
#line 41 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_10 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_10),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_10));
#line 42 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_11 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_11),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_11));
#line 43 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_12 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_12),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_12));
#line 44 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_13 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_13),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_13));
#line 45 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_14 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_14),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_14));
#line 46 "SPLIT_DWORD_INTO_QUARTERS.fct"
      st_lv_QUARTER_BYTE_15 = func_DWORD_TO_BYTE(func_SHR(
          func_AND<CIEC_DWORD>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_DWORD_QUARTER_15),
          forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_15));
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
