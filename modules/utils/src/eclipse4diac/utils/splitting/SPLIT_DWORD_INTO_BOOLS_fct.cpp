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
 *** Name: SPLIT_DWORD_INTO_BOOLS
 *** Description: this Function extracts the 32 BOOLS from a dword
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_DWORD_INTO_BOOLS_fct.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{
        "BIT_00"_STRID, "BIT_01"_STRID, "BIT_02"_STRID, "BIT_03"_STRID, "BIT_04"_STRID, "BIT_05"_STRID, "BIT_06"_STRID,
        "BIT_07"_STRID, "BIT_08"_STRID, "BIT_09"_STRID, "BIT_10"_STRID, "BIT_11"_STRID, "BIT_12"_STRID, "BIT_13"_STRID,
        "BIT_14"_STRID, "BIT_15"_STRID, "BIT_16"_STRID, "BIT_17"_STRID, "BIT_18"_STRID, "BIT_19"_STRID, "BIT_20"_STRID,
        "BIT_21"_STRID, "BIT_22"_STRID, "BIT_23"_STRID, "BIT_24"_STRID, "BIT_25"_STRID, "BIT_26"_STRID, "BIT_27"_STRID,
        "BIT_28"_STRID, "BIT_29"_STRID, "BIT_30"_STRID, "BIT_31"_STRID};
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

    void func_SPLIT_DWORD_INTO_BOOLS__impl(const CIEC_DWORD &st_lv_IN,
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
                                           CIEC_BOOL &st_lv_BIT_15,
                                           CIEC_BOOL &st_lv_BIT_16,
                                           CIEC_BOOL &st_lv_BIT_17,
                                           CIEC_BOOL &st_lv_BIT_18,
                                           CIEC_BOOL &st_lv_BIT_19,
                                           CIEC_BOOL &st_lv_BIT_20,
                                           CIEC_BOOL &st_lv_BIT_21,
                                           CIEC_BOOL &st_lv_BIT_22,
                                           CIEC_BOOL &st_lv_BIT_23,
                                           CIEC_BOOL &st_lv_BIT_24,
                                           CIEC_BOOL &st_lv_BIT_25,
                                           CIEC_BOOL &st_lv_BIT_26,
                                           CIEC_BOOL &st_lv_BIT_27,
                                           CIEC_BOOL &st_lv_BIT_28,
                                           CIEC_BOOL &st_lv_BIT_29,
                                           CIEC_BOOL &st_lv_BIT_30,
                                           CIEC_BOOL &st_lv_BIT_31);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_BOOLS,
                     "eclipse4diac::utils::splitting::SPLIT_DWORD_INTO_BOOLS"_STRID,
                     TypeHash)

  FORTE_SPLIT_DWORD_INTO_BOOLS::FORTE_SPLIT_DWORD_INTO_BOOLS(const StringId paInstanceNameId,
                                                             CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_DWORD),
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
      var_BIT_16(0_BOOL),
      var_BIT_17(0_BOOL),
      var_BIT_18(0_BOOL),
      var_BIT_19(0_BOOL),
      var_BIT_20(0_BOOL),
      var_BIT_21(0_BOOL),
      var_BIT_22(0_BOOL),
      var_BIT_23(0_BOOL),
      var_BIT_24(0_BOOL),
      var_BIT_25(0_BOOL),
      var_BIT_26(0_BOOL),
      var_BIT_27(0_BOOL),
      var_BIT_28(0_BOOL),
      var_BIT_29(0_BOOL),
      var_BIT_30(0_BOOL),
      var_BIT_31(0_BOOL),
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
      conn_BIT_15(*this, 15, var_BIT_15),
      conn_BIT_16(*this, 16, var_BIT_16),
      conn_BIT_17(*this, 17, var_BIT_17),
      conn_BIT_18(*this, 18, var_BIT_18),
      conn_BIT_19(*this, 19, var_BIT_19),
      conn_BIT_20(*this, 20, var_BIT_20),
      conn_BIT_21(*this, 21, var_BIT_21),
      conn_BIT_22(*this, 22, var_BIT_22),
      conn_BIT_23(*this, 23, var_BIT_23),
      conn_BIT_24(*this, 24, var_BIT_24),
      conn_BIT_25(*this, 25, var_BIT_25),
      conn_BIT_26(*this, 26, var_BIT_26),
      conn_BIT_27(*this, 27, var_BIT_27),
      conn_BIT_28(*this, 28, var_BIT_28),
      conn_BIT_29(*this, 29, var_BIT_29),
      conn_BIT_30(*this, 30, var_BIT_30),
      conn_BIT_31(*this, 31, var_BIT_31) {
  }

  void FORTE_SPLIT_DWORD_INTO_BOOLS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_DWORD;
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
    var_BIT_16 = 0_BOOL;
    var_BIT_17 = 0_BOOL;
    var_BIT_18 = 0_BOOL;
    var_BIT_19 = 0_BOOL;
    var_BIT_20 = 0_BOOL;
    var_BIT_21 = 0_BOOL;
    var_BIT_22 = 0_BOOL;
    var_BIT_23 = 0_BOOL;
    var_BIT_24 = 0_BOOL;
    var_BIT_25 = 0_BOOL;
    var_BIT_26 = 0_BOOL;
    var_BIT_27 = 0_BOOL;
    var_BIT_28 = 0_BOOL;
    var_BIT_29 = 0_BOOL;
    var_BIT_30 = 0_BOOL;
    var_BIT_31 = 0_BOOL;
  }

  void FORTE_SPLIT_DWORD_INTO_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_DWORD_INTO_BOOLS::writeOutputData(const TEventID paEIID) {
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
        writeData(17, var_BIT_16, conn_BIT_16);
        writeData(18, var_BIT_17, conn_BIT_17);
        writeData(19, var_BIT_18, conn_BIT_18);
        writeData(20, var_BIT_19, conn_BIT_19);
        writeData(21, var_BIT_20, conn_BIT_20);
        writeData(22, var_BIT_21, conn_BIT_21);
        writeData(23, var_BIT_22, conn_BIT_22);
        writeData(24, var_BIT_23, conn_BIT_23);
        writeData(25, var_BIT_24, conn_BIT_24);
        writeData(26, var_BIT_25, conn_BIT_25);
        writeData(27, var_BIT_26, conn_BIT_26);
        writeData(28, var_BIT_27, conn_BIT_27);
        writeData(29, var_BIT_28, conn_BIT_28);
        writeData(30, var_BIT_29, conn_BIT_29);
        writeData(31, var_BIT_30, conn_BIT_30);
        writeData(32, var_BIT_31, conn_BIT_31);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_BOOLS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_BOOLS::getDO(const size_t paIndex) {
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
      case 16: return &var_BIT_16;
      case 17: return &var_BIT_17;
      case 18: return &var_BIT_18;
      case 19: return &var_BIT_19;
      case 20: return &var_BIT_20;
      case 21: return &var_BIT_21;
      case 22: return &var_BIT_22;
      case 23: return &var_BIT_23;
      case 24: return &var_BIT_24;
      case 25: return &var_BIT_25;
      case 26: return &var_BIT_26;
      case 27: return &var_BIT_27;
      case 28: return &var_BIT_28;
      case 29: return &var_BIT_29;
      case 30: return &var_BIT_30;
      case 31: return &var_BIT_31;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_DWORD_INTO_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_DWORD_INTO_BOOLS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_DWORD_INTO_BOOLS::getDOConUnchecked(const TPortId paIndex) {
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
      case 16: return &conn_BIT_16;
      case 17: return &conn_BIT_17;
      case 18: return &conn_BIT_18;
      case 19: return &conn_BIT_19;
      case 20: return &conn_BIT_20;
      case 21: return &conn_BIT_21;
      case 22: return &conn_BIT_22;
      case 23: return &conn_BIT_23;
      case 24: return &conn_BIT_24;
      case 25: return &conn_BIT_25;
      case 26: return &conn_BIT_26;
      case 27: return &conn_BIT_27;
      case 28: return &conn_BIT_28;
      case 29: return &conn_BIT_29;
      case 30: return &conn_BIT_30;
      case 31: return &conn_BIT_31;
    }
    return nullptr;
  }

  void FORTE_SPLIT_DWORD_INTO_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_DWORD_INTO_BOOLS__impl(
        var_IN, var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05, var_BIT_06, var_BIT_07,
        var_BIT_08, var_BIT_09, var_BIT_10, var_BIT_11, var_BIT_12, var_BIT_13, var_BIT_14, var_BIT_15, var_BIT_16,
        var_BIT_17, var_BIT_18, var_BIT_19, var_BIT_20, var_BIT_21, var_BIT_22, var_BIT_23, var_BIT_24, var_BIT_25,
        var_BIT_26, var_BIT_27, var_BIT_28, var_BIT_29, var_BIT_30, var_BIT_31);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_DWORD_INTO_BOOLS(const CIEC_DWORD &st_lv_IN,
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
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_15,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_16,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_17,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_18,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_19,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_20,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_21,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_22,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_23,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_24,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_25,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_26,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_27,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_28,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_29,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_30,
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_31) {
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
    COutputGuard st_guard_BIT_16(st_lv_BIT_16);
    COutputGuard st_guard_BIT_17(st_lv_BIT_17);
    COutputGuard st_guard_BIT_18(st_lv_BIT_18);
    COutputGuard st_guard_BIT_19(st_lv_BIT_19);
    COutputGuard st_guard_BIT_20(st_lv_BIT_20);
    COutputGuard st_guard_BIT_21(st_lv_BIT_21);
    COutputGuard st_guard_BIT_22(st_lv_BIT_22);
    COutputGuard st_guard_BIT_23(st_lv_BIT_23);
    COutputGuard st_guard_BIT_24(st_lv_BIT_24);
    COutputGuard st_guard_BIT_25(st_lv_BIT_25);
    COutputGuard st_guard_BIT_26(st_lv_BIT_26);
    COutputGuard st_guard_BIT_27(st_lv_BIT_27);
    COutputGuard st_guard_BIT_28(st_lv_BIT_28);
    COutputGuard st_guard_BIT_29(st_lv_BIT_29);
    COutputGuard st_guard_BIT_30(st_lv_BIT_30);
    COutputGuard st_guard_BIT_31(st_lv_BIT_31);
    func_SPLIT_DWORD_INTO_BOOLS__impl(
        st_lv_IN, *st_lv_BIT_00, *st_lv_BIT_01, *st_lv_BIT_02, *st_lv_BIT_03, *st_lv_BIT_04, *st_lv_BIT_05,
        *st_lv_BIT_06, *st_lv_BIT_07, *st_lv_BIT_08, *st_lv_BIT_09, *st_lv_BIT_10, *st_lv_BIT_11, *st_lv_BIT_12,
        *st_lv_BIT_13, *st_lv_BIT_14, *st_lv_BIT_15, *st_lv_BIT_16, *st_lv_BIT_17, *st_lv_BIT_18, *st_lv_BIT_19,
        *st_lv_BIT_20, *st_lv_BIT_21, *st_lv_BIT_22, *st_lv_BIT_23, *st_lv_BIT_24, *st_lv_BIT_25, *st_lv_BIT_26,
        *st_lv_BIT_27, *st_lv_BIT_28, *st_lv_BIT_29, *st_lv_BIT_30, *st_lv_BIT_31);
  }

  namespace {
    void func_SPLIT_DWORD_INTO_BOOLS__impl(const CIEC_DWORD &st_lv_IN,
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
                                           CIEC_BOOL &st_lv_BIT_15,
                                           CIEC_BOOL &st_lv_BIT_16,
                                           CIEC_BOOL &st_lv_BIT_17,
                                           CIEC_BOOL &st_lv_BIT_18,
                                           CIEC_BOOL &st_lv_BIT_19,
                                           CIEC_BOOL &st_lv_BIT_20,
                                           CIEC_BOOL &st_lv_BIT_21,
                                           CIEC_BOOL &st_lv_BIT_22,
                                           CIEC_BOOL &st_lv_BIT_23,
                                           CIEC_BOOL &st_lv_BIT_24,
                                           CIEC_BOOL &st_lv_BIT_25,
                                           CIEC_BOOL &st_lv_BIT_26,
                                           CIEC_BOOL &st_lv_BIT_27,
                                           CIEC_BOOL &st_lv_BIT_28,
                                           CIEC_BOOL &st_lv_BIT_29,
                                           CIEC_BOOL &st_lv_BIT_30,
                                           CIEC_BOOL &st_lv_BIT_31) {
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
      st_lv_BIT_16 = 0_BOOL;
      st_lv_BIT_17 = 0_BOOL;
      st_lv_BIT_18 = 0_BOOL;
      st_lv_BIT_19 = 0_BOOL;
      st_lv_BIT_20 = 0_BOOL;
      st_lv_BIT_21 = 0_BOOL;
      st_lv_BIT_22 = 0_BOOL;
      st_lv_BIT_23 = 0_BOOL;
      st_lv_BIT_24 = 0_BOOL;
      st_lv_BIT_25 = 0_BOOL;
      st_lv_BIT_26 = 0_BOOL;
      st_lv_BIT_27 = 0_BOOL;
      st_lv_BIT_28 = 0_BOOL;
      st_lv_BIT_29 = 0_BOOL;
      st_lv_BIT_30 = 0_BOOL;
      st_lv_BIT_31 = 0_BOOL;

#line 45 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_00 = st_lv_IN.cpartial<CIEC_BOOL>(0);
#line 46 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
#line 47 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_02 = st_lv_IN.cpartial<CIEC_BOOL>(2);
#line 48 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_03 = st_lv_IN.cpartial<CIEC_BOOL>(3);
#line 49 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_04 = st_lv_IN.cpartial<CIEC_BOOL>(4);
#line 50 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_05 = st_lv_IN.cpartial<CIEC_BOOL>(5);
#line 51 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_06 = st_lv_IN.cpartial<CIEC_BOOL>(6);
#line 52 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_07 = st_lv_IN.cpartial<CIEC_BOOL>(7);
#line 53 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_08 = st_lv_IN.cpartial<CIEC_BOOL>(8);
#line 54 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_09 = st_lv_IN.cpartial<CIEC_BOOL>(9);
#line 55 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_10 = st_lv_IN.cpartial<CIEC_BOOL>(10);
#line 56 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_11 = st_lv_IN.cpartial<CIEC_BOOL>(11);
#line 57 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_12 = st_lv_IN.cpartial<CIEC_BOOL>(12);
#line 58 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_13 = st_lv_IN.cpartial<CIEC_BOOL>(13);
#line 59 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_14 = st_lv_IN.cpartial<CIEC_BOOL>(14);
#line 60 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_15 = st_lv_IN.cpartial<CIEC_BOOL>(15);
#line 61 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_16 = st_lv_IN.cpartial<CIEC_BOOL>(16);
#line 62 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_17 = st_lv_IN.cpartial<CIEC_BOOL>(17);
#line 63 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_18 = st_lv_IN.cpartial<CIEC_BOOL>(18);
#line 64 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_19 = st_lv_IN.cpartial<CIEC_BOOL>(19);
#line 65 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_20 = st_lv_IN.cpartial<CIEC_BOOL>(20);
#line 66 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_21 = st_lv_IN.cpartial<CIEC_BOOL>(21);
#line 67 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_22 = st_lv_IN.cpartial<CIEC_BOOL>(22);
#line 68 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_23 = st_lv_IN.cpartial<CIEC_BOOL>(23);
#line 69 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_24 = st_lv_IN.cpartial<CIEC_BOOL>(24);
#line 70 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_25 = st_lv_IN.cpartial<CIEC_BOOL>(25);
#line 71 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_26 = st_lv_IN.cpartial<CIEC_BOOL>(26);
#line 72 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_27 = st_lv_IN.cpartial<CIEC_BOOL>(27);
#line 73 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_28 = st_lv_IN.cpartial<CIEC_BOOL>(28);
#line 74 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_29 = st_lv_IN.cpartial<CIEC_BOOL>(29);
#line 75 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_30 = st_lv_IN.cpartial<CIEC_BOOL>(30);
#line 76 "SPLIT_DWORD_INTO_BOOLS.fct"
      st_lv_BIT_31 = st_lv_IN.cpartial<CIEC_BOOL>(31);
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
