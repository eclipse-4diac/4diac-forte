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
 *** Name: SPLIT_WORD_INTO_BOOLS
 *** Description: this Function extracts the 16 BOOLS from a word
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_WORD_INTO_BOOLS_fct.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_word.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames =
        std::array{"BIT_00"_STRID, "BIT_01"_STRID, "BIT_02"_STRID, "BIT_03"_STRID, "BIT_04"_STRID, "BIT_05"_STRID,
                   "BIT_06"_STRID, "BIT_07"_STRID, "BIT_08"_STRID, "BIT_09"_STRID, "BIT_10"_STRID, "BIT_11"_STRID,
                   "BIT_12"_STRID, "BIT_13"_STRID, "BIT_14"_STRID, "BIT_15"_STRID};
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

    void func_SPLIT_WORD_INTO_BOOLS__impl(const CIEC_WORD &st_lv_IN,
                                          CIEC_BOOL &st_lv_BIT_00,
                                          CIEC_BOOL &st_lv_BIT_01,
                                          CIEC_BOOL &st_lv_BIT_02,
                                          CIEC_BOOL &st_lv_BIT_03,
                                          CIEC_BOOL &st_lv_BIT_04,
                                          CIEC_BOOL &st_lv_BIT_05,
                                          CIEC_BOOL &st_lv_BIT_06,
                                          CIEC_BOOL &st_lv_BIT_07,
                                          CIEC_BOOL &st_lv_BIT_08,
                                          CIEC_BOOL &st_lv_BIT_09,
                                          CIEC_BOOL &st_lv_BIT_10,
                                          CIEC_BOOL &st_lv_BIT_11,
                                          CIEC_BOOL &st_lv_BIT_12,
                                          CIEC_BOOL &st_lv_BIT_13,
                                          CIEC_BOOL &st_lv_BIT_14,
                                          CIEC_BOOL &st_lv_BIT_15);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_WORD_INTO_BOOLS,
                     "eclipse4diac::utils::splitting::SPLIT_WORD_INTO_BOOLS"_STRID,
                     TypeHash)

  FORTE_SPLIT_WORD_INTO_BOOLS::FORTE_SPLIT_WORD_INTO_BOOLS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_WORD),
      var_BIT_00(0_BOOL),
      var_BIT_01(0_BOOL),
      var_BIT_02(0_BOOL),
      var_BIT_03(0_BOOL),
      var_BIT_04(0_BOOL),
      var_BIT_05(0_BOOL),
      var_BIT_06(0_BOOL),
      var_BIT_07(0_BOOL),
      var_BIT_08(0_BOOL),
      var_BIT_09(0_BOOL),
      var_BIT_10(0_BOOL),
      var_BIT_11(0_BOOL),
      var_BIT_12(0_BOOL),
      var_BIT_13(0_BOOL),
      var_BIT_14(0_BOOL),
      var_BIT_15(0_BOOL),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_BIT_00(*this, 0, var_BIT_00),
      conn_BIT_01(*this, 1, var_BIT_01),
      conn_BIT_02(*this, 2, var_BIT_02),
      conn_BIT_03(*this, 3, var_BIT_03),
      conn_BIT_04(*this, 4, var_BIT_04),
      conn_BIT_05(*this, 5, var_BIT_05),
      conn_BIT_06(*this, 6, var_BIT_06),
      conn_BIT_07(*this, 7, var_BIT_07),
      conn_BIT_08(*this, 8, var_BIT_08),
      conn_BIT_09(*this, 9, var_BIT_09),
      conn_BIT_10(*this, 10, var_BIT_10),
      conn_BIT_11(*this, 11, var_BIT_11),
      conn_BIT_12(*this, 12, var_BIT_12),
      conn_BIT_13(*this, 13, var_BIT_13),
      conn_BIT_14(*this, 14, var_BIT_14),
      conn_BIT_15(*this, 15, var_BIT_15) {
  }

  void FORTE_SPLIT_WORD_INTO_BOOLS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_WORD;
    var_BIT_00 = 0_BOOL;
    var_BIT_01 = 0_BOOL;
    var_BIT_02 = 0_BOOL;
    var_BIT_03 = 0_BOOL;
    var_BIT_04 = 0_BOOL;
    var_BIT_05 = 0_BOOL;
    var_BIT_06 = 0_BOOL;
    var_BIT_07 = 0_BOOL;
    var_BIT_08 = 0_BOOL;
    var_BIT_09 = 0_BOOL;
    var_BIT_10 = 0_BOOL;
    var_BIT_11 = 0_BOOL;
    var_BIT_12 = 0_BOOL;
    var_BIT_13 = 0_BOOL;
    var_BIT_14 = 0_BOOL;
    var_BIT_15 = 0_BOOL;
  }

  void FORTE_SPLIT_WORD_INTO_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_WORD_INTO_BOOLS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_BIT_00, conn_BIT_00);
        writeData(2, var_BIT_01, conn_BIT_01);
        writeData(3, var_BIT_02, conn_BIT_02);
        writeData(4, var_BIT_03, conn_BIT_03);
        writeData(5, var_BIT_04, conn_BIT_04);
        writeData(6, var_BIT_05, conn_BIT_05);
        writeData(7, var_BIT_06, conn_BIT_06);
        writeData(8, var_BIT_07, conn_BIT_07);
        writeData(9, var_BIT_08, conn_BIT_08);
        writeData(10, var_BIT_09, conn_BIT_09);
        writeData(11, var_BIT_10, conn_BIT_10);
        writeData(12, var_BIT_11, conn_BIT_11);
        writeData(13, var_BIT_12, conn_BIT_12);
        writeData(14, var_BIT_13, conn_BIT_13);
        writeData(15, var_BIT_14, conn_BIT_14);
        writeData(16, var_BIT_15, conn_BIT_15);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_WORD_INTO_BOOLS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_WORD_INTO_BOOLS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_BIT_00;
      case 1: return &var_BIT_01;
      case 2: return &var_BIT_02;
      case 3: return &var_BIT_03;
      case 4: return &var_BIT_04;
      case 5: return &var_BIT_05;
      case 6: return &var_BIT_06;
      case 7: return &var_BIT_07;
      case 8: return &var_BIT_08;
      case 9: return &var_BIT_09;
      case 10: return &var_BIT_10;
      case 11: return &var_BIT_11;
      case 12: return &var_BIT_12;
      case 13: return &var_BIT_13;
      case 14: return &var_BIT_14;
      case 15: return &var_BIT_15;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_WORD_INTO_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_WORD_INTO_BOOLS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_WORD_INTO_BOOLS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_BIT_00;
      case 1: return &conn_BIT_01;
      case 2: return &conn_BIT_02;
      case 3: return &conn_BIT_03;
      case 4: return &conn_BIT_04;
      case 5: return &conn_BIT_05;
      case 6: return &conn_BIT_06;
      case 7: return &conn_BIT_07;
      case 8: return &conn_BIT_08;
      case 9: return &conn_BIT_09;
      case 10: return &conn_BIT_10;
      case 11: return &conn_BIT_11;
      case 12: return &conn_BIT_12;
      case 13: return &conn_BIT_13;
      case 14: return &conn_BIT_14;
      case 15: return &conn_BIT_15;
    }
    return nullptr;
  }

  void FORTE_SPLIT_WORD_INTO_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_WORD_INTO_BOOLS__impl(var_IN, var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05,
                                     var_BIT_06, var_BIT_07, var_BIT_08, var_BIT_09, var_BIT_10, var_BIT_11, var_BIT_12,
                                     var_BIT_13, var_BIT_14, var_BIT_15);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_WORD_INTO_BOOLS(const CIEC_WORD &st_lv_IN,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_00,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_01,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_02,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_03,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_04,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_05,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_06,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_07,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_08,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_09,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_10,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_11,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_12,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_13,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_14,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_15) {
    COutputGuard st_guard_BIT_00(st_lv_BIT_00);
    COutputGuard st_guard_BIT_01(st_lv_BIT_01);
    COutputGuard st_guard_BIT_02(st_lv_BIT_02);
    COutputGuard st_guard_BIT_03(st_lv_BIT_03);
    COutputGuard st_guard_BIT_04(st_lv_BIT_04);
    COutputGuard st_guard_BIT_05(st_lv_BIT_05);
    COutputGuard st_guard_BIT_06(st_lv_BIT_06);
    COutputGuard st_guard_BIT_07(st_lv_BIT_07);
    COutputGuard st_guard_BIT_08(st_lv_BIT_08);
    COutputGuard st_guard_BIT_09(st_lv_BIT_09);
    COutputGuard st_guard_BIT_10(st_lv_BIT_10);
    COutputGuard st_guard_BIT_11(st_lv_BIT_11);
    COutputGuard st_guard_BIT_12(st_lv_BIT_12);
    COutputGuard st_guard_BIT_13(st_lv_BIT_13);
    COutputGuard st_guard_BIT_14(st_lv_BIT_14);
    COutputGuard st_guard_BIT_15(st_lv_BIT_15);
    func_SPLIT_WORD_INTO_BOOLS__impl(st_lv_IN, *st_lv_BIT_00, *st_lv_BIT_01, *st_lv_BIT_02, *st_lv_BIT_03,
                                     *st_lv_BIT_04, *st_lv_BIT_05, *st_lv_BIT_06, *st_lv_BIT_07, *st_lv_BIT_08,
                                     *st_lv_BIT_09, *st_lv_BIT_10, *st_lv_BIT_11, *st_lv_BIT_12, *st_lv_BIT_13,
                                     *st_lv_BIT_14, *st_lv_BIT_15);
  }

  namespace {
    void func_SPLIT_WORD_INTO_BOOLS__impl(const CIEC_WORD &st_lv_IN,
                                          CIEC_BOOL &st_lv_BIT_00,
                                          CIEC_BOOL &st_lv_BIT_01,
                                          CIEC_BOOL &st_lv_BIT_02,
                                          CIEC_BOOL &st_lv_BIT_03,
                                          CIEC_BOOL &st_lv_BIT_04,
                                          CIEC_BOOL &st_lv_BIT_05,
                                          CIEC_BOOL &st_lv_BIT_06,
                                          CIEC_BOOL &st_lv_BIT_07,
                                          CIEC_BOOL &st_lv_BIT_08,
                                          CIEC_BOOL &st_lv_BIT_09,
                                          CIEC_BOOL &st_lv_BIT_10,
                                          CIEC_BOOL &st_lv_BIT_11,
                                          CIEC_BOOL &st_lv_BIT_12,
                                          CIEC_BOOL &st_lv_BIT_13,
                                          CIEC_BOOL &st_lv_BIT_14,
                                          CIEC_BOOL &st_lv_BIT_15) {
      st_lv_BIT_00 = 0_BOOL;
      st_lv_BIT_01 = 0_BOOL;
      st_lv_BIT_02 = 0_BOOL;
      st_lv_BIT_03 = 0_BOOL;
      st_lv_BIT_04 = 0_BOOL;
      st_lv_BIT_05 = 0_BOOL;
      st_lv_BIT_06 = 0_BOOL;
      st_lv_BIT_07 = 0_BOOL;
      st_lv_BIT_08 = 0_BOOL;
      st_lv_BIT_09 = 0_BOOL;
      st_lv_BIT_10 = 0_BOOL;
      st_lv_BIT_11 = 0_BOOL;
      st_lv_BIT_12 = 0_BOOL;
      st_lv_BIT_13 = 0_BOOL;
      st_lv_BIT_14 = 0_BOOL;
      st_lv_BIT_15 = 0_BOOL;

#line 29 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_00 = st_lv_IN.cpartial<CIEC_BOOL>(0);
#line 30 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
#line 31 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_02 = st_lv_IN.cpartial<CIEC_BOOL>(2);
#line 32 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_03 = st_lv_IN.cpartial<CIEC_BOOL>(3);
#line 33 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_04 = st_lv_IN.cpartial<CIEC_BOOL>(4);
#line 34 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_05 = st_lv_IN.cpartial<CIEC_BOOL>(5);
#line 35 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_06 = st_lv_IN.cpartial<CIEC_BOOL>(6);
#line 36 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_07 = st_lv_IN.cpartial<CIEC_BOOL>(7);
#line 37 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_08 = st_lv_IN.cpartial<CIEC_BOOL>(8);
#line 38 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_09 = st_lv_IN.cpartial<CIEC_BOOL>(9);
#line 39 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_10 = st_lv_IN.cpartial<CIEC_BOOL>(10);
#line 40 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_11 = st_lv_IN.cpartial<CIEC_BOOL>(11);
#line 41 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_12 = st_lv_IN.cpartial<CIEC_BOOL>(12);
#line 42 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_13 = st_lv_IN.cpartial<CIEC_BOOL>(13);
#line 43 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_14 = st_lv_IN.cpartial<CIEC_BOOL>(14);
#line 44 "SPLIT_WORD_INTO_BOOLS.fct"
      st_lv_BIT_15 = st_lv_IN.cpartial<CIEC_BOOL>(15);
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
