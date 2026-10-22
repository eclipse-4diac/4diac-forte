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
 *** Name: ASSEMBLE_LWORD_FROM_QUARTERS
 *** Description: this Function combines the 31 QUARTER BYTES to a LWORD
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_QUARTERS_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_QUARTERS_fct.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"
#include "forte/iec61131_functions/func_BYTE_TO_LWORD.h"
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
                   "QUARTER_BYTE_04"_STRID, "QUARTER_BYTE_05"_STRID, "QUARTER_BYTE_06"_STRID, "QUARTER_BYTE_07"_STRID,
                   "QUARTER_BYTE_08"_STRID, "QUARTER_BYTE_09"_STRID, "QUARTER_BYTE_10"_STRID, "QUARTER_BYTE_11"_STRID,
                   "QUARTER_BYTE_12"_STRID, "QUARTER_BYTE_13"_STRID, "QUARTER_BYTE_14"_STRID, "QUARTER_BYTE_15"_STRID,
                   "QUARTER_BYTE_16"_STRID, "QUARTER_BYTE_17"_STRID, "QUARTER_BYTE_18"_STRID, "QUARTER_BYTE_19"_STRID,
                   "QUARTER_BYTE_20"_STRID, "QUARTER_BYTE_21"_STRID, "QUARTER_BYTE_22"_STRID, "QUARTER_BYTE_23"_STRID,
                   "QUARTER_BYTE_24"_STRID, "QUARTER_BYTE_25"_STRID, "QUARTER_BYTE_26"_STRID, "QUARTER_BYTE_27"_STRID,
                   "QUARTER_BYTE_28"_STRID, "QUARTER_BYTE_29"_STRID, "QUARTER_BYTE_30"_STRID, "QUARTER_BYTE_31"_STRID};
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

    CIEC_LWORD func_ASSEMBLE_LWORD_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_07,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_08,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_09,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_10,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_11,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_12,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_13,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_14,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_15,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_16,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_17,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_18,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_19,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_20,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_21,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_22,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_23,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_24,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_25,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_26,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_27,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_28,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_29,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_30,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_31);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_QUARTERS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_LWORD_FROM_QUARTERS"_STRID,
                     TypeHash)

  FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::FORTE_ASSEMBLE_LWORD_FROM_QUARTERS(const StringId paInstanceNameId,
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
      var_QUARTER_BYTE_08(0_BYTE),
      var_QUARTER_BYTE_09(0_BYTE),
      var_QUARTER_BYTE_10(0_BYTE),
      var_QUARTER_BYTE_11(0_BYTE),
      var_QUARTER_BYTE_12(0_BYTE),
      var_QUARTER_BYTE_13(0_BYTE),
      var_QUARTER_BYTE_14(0_BYTE),
      var_QUARTER_BYTE_15(0_BYTE),
      var_QUARTER_BYTE_16(0_BYTE),
      var_QUARTER_BYTE_17(0_BYTE),
      var_QUARTER_BYTE_18(0_BYTE),
      var_QUARTER_BYTE_19(0_BYTE),
      var_QUARTER_BYTE_20(0_BYTE),
      var_QUARTER_BYTE_21(0_BYTE),
      var_QUARTER_BYTE_22(0_BYTE),
      var_QUARTER_BYTE_23(0_BYTE),
      var_QUARTER_BYTE_24(0_BYTE),
      var_QUARTER_BYTE_25(0_BYTE),
      var_QUARTER_BYTE_26(0_BYTE),
      var_QUARTER_BYTE_27(0_BYTE),
      var_QUARTER_BYTE_28(0_BYTE),
      var_QUARTER_BYTE_29(0_BYTE),
      var_QUARTER_BYTE_30(0_BYTE),
      var_QUARTER_BYTE_31(0_BYTE),
      var_(0_LWORD),
      conn_CNF(*this, 0),
      conn_QUARTER_BYTE_00(nullptr),
      conn_QUARTER_BYTE_01(nullptr),
      conn_QUARTER_BYTE_02(nullptr),
      conn_QUARTER_BYTE_03(nullptr),
      conn_QUARTER_BYTE_04(nullptr),
      conn_QUARTER_BYTE_05(nullptr),
      conn_QUARTER_BYTE_06(nullptr),
      conn_QUARTER_BYTE_07(nullptr),
      conn_QUARTER_BYTE_08(nullptr),
      conn_QUARTER_BYTE_09(nullptr),
      conn_QUARTER_BYTE_10(nullptr),
      conn_QUARTER_BYTE_11(nullptr),
      conn_QUARTER_BYTE_12(nullptr),
      conn_QUARTER_BYTE_13(nullptr),
      conn_QUARTER_BYTE_14(nullptr),
      conn_QUARTER_BYTE_15(nullptr),
      conn_QUARTER_BYTE_16(nullptr),
      conn_QUARTER_BYTE_17(nullptr),
      conn_QUARTER_BYTE_18(nullptr),
      conn_QUARTER_BYTE_19(nullptr),
      conn_QUARTER_BYTE_20(nullptr),
      conn_QUARTER_BYTE_21(nullptr),
      conn_QUARTER_BYTE_22(nullptr),
      conn_QUARTER_BYTE_23(nullptr),
      conn_QUARTER_BYTE_24(nullptr),
      conn_QUARTER_BYTE_25(nullptr),
      conn_QUARTER_BYTE_26(nullptr),
      conn_QUARTER_BYTE_27(nullptr),
      conn_QUARTER_BYTE_28(nullptr),
      conn_QUARTER_BYTE_29(nullptr),
      conn_QUARTER_BYTE_30(nullptr),
      conn_QUARTER_BYTE_31(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::setInitialValues() {
    CFunctionBlock::setInitialValues();
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
    var_QUARTER_BYTE_16 = 0_BYTE;
    var_QUARTER_BYTE_17 = 0_BYTE;
    var_QUARTER_BYTE_18 = 0_BYTE;
    var_QUARTER_BYTE_19 = 0_BYTE;
    var_QUARTER_BYTE_20 = 0_BYTE;
    var_QUARTER_BYTE_21 = 0_BYTE;
    var_QUARTER_BYTE_22 = 0_BYTE;
    var_QUARTER_BYTE_23 = 0_BYTE;
    var_QUARTER_BYTE_24 = 0_BYTE;
    var_QUARTER_BYTE_25 = 0_BYTE;
    var_QUARTER_BYTE_26 = 0_BYTE;
    var_QUARTER_BYTE_27 = 0_BYTE;
    var_QUARTER_BYTE_28 = 0_BYTE;
    var_QUARTER_BYTE_29 = 0_BYTE;
    var_QUARTER_BYTE_30 = 0_BYTE;
    var_QUARTER_BYTE_31 = 0_BYTE;
    var_ = 0_LWORD;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::readInputData(const TEventID paEIID) {
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
        readData(8, var_QUARTER_BYTE_08, conn_QUARTER_BYTE_08);
        readData(9, var_QUARTER_BYTE_09, conn_QUARTER_BYTE_09);
        readData(10, var_QUARTER_BYTE_10, conn_QUARTER_BYTE_10);
        readData(11, var_QUARTER_BYTE_11, conn_QUARTER_BYTE_11);
        readData(12, var_QUARTER_BYTE_12, conn_QUARTER_BYTE_12);
        readData(13, var_QUARTER_BYTE_13, conn_QUARTER_BYTE_13);
        readData(14, var_QUARTER_BYTE_14, conn_QUARTER_BYTE_14);
        readData(15, var_QUARTER_BYTE_15, conn_QUARTER_BYTE_15);
        readData(16, var_QUARTER_BYTE_16, conn_QUARTER_BYTE_16);
        readData(17, var_QUARTER_BYTE_17, conn_QUARTER_BYTE_17);
        readData(18, var_QUARTER_BYTE_18, conn_QUARTER_BYTE_18);
        readData(19, var_QUARTER_BYTE_19, conn_QUARTER_BYTE_19);
        readData(20, var_QUARTER_BYTE_20, conn_QUARTER_BYTE_20);
        readData(21, var_QUARTER_BYTE_21, conn_QUARTER_BYTE_21);
        readData(22, var_QUARTER_BYTE_22, conn_QUARTER_BYTE_22);
        readData(23, var_QUARTER_BYTE_23, conn_QUARTER_BYTE_23);
        readData(24, var_QUARTER_BYTE_24, conn_QUARTER_BYTE_24);
        readData(25, var_QUARTER_BYTE_25, conn_QUARTER_BYTE_25);
        readData(26, var_QUARTER_BYTE_26, conn_QUARTER_BYTE_26);
        readData(27, var_QUARTER_BYTE_27, conn_QUARTER_BYTE_27);
        readData(28, var_QUARTER_BYTE_28, conn_QUARTER_BYTE_28);
        readData(29, var_QUARTER_BYTE_29, conn_QUARTER_BYTE_29);
        readData(30, var_QUARTER_BYTE_30, conn_QUARTER_BYTE_30);
        readData(31, var_QUARTER_BYTE_31, conn_QUARTER_BYTE_31);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(32, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::getDI(const size_t paIndex) {
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
      case 16: return &var_QUARTER_BYTE_16;
      case 17: return &var_QUARTER_BYTE_17;
      case 18: return &var_QUARTER_BYTE_18;
      case 19: return &var_QUARTER_BYTE_19;
      case 20: return &var_QUARTER_BYTE_20;
      case 21: return &var_QUARTER_BYTE_21;
      case 22: return &var_QUARTER_BYTE_22;
      case 23: return &var_QUARTER_BYTE_23;
      case 24: return &var_QUARTER_BYTE_24;
      case 25: return &var_QUARTER_BYTE_25;
      case 26: return &var_QUARTER_BYTE_26;
      case 27: return &var_QUARTER_BYTE_27;
      case 28: return &var_QUARTER_BYTE_28;
      case 29: return &var_QUARTER_BYTE_29;
      case 30: return &var_QUARTER_BYTE_30;
      case 31: return &var_QUARTER_BYTE_31;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
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
      case 16: return &conn_QUARTER_BYTE_16;
      case 17: return &conn_QUARTER_BYTE_17;
      case 18: return &conn_QUARTER_BYTE_18;
      case 19: return &conn_QUARTER_BYTE_19;
      case 20: return &conn_QUARTER_BYTE_20;
      case 21: return &conn_QUARTER_BYTE_21;
      case 22: return &conn_QUARTER_BYTE_22;
      case 23: return &conn_QUARTER_BYTE_23;
      case 24: return &conn_QUARTER_BYTE_24;
      case 25: return &conn_QUARTER_BYTE_25;
      case 26: return &conn_QUARTER_BYTE_26;
      case 27: return &conn_QUARTER_BYTE_27;
      case 28: return &conn_QUARTER_BYTE_28;
      case 29: return &conn_QUARTER_BYTE_29;
      case 30: return &conn_QUARTER_BYTE_30;
      case 31: return &conn_QUARTER_BYTE_31;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_LWORD_FROM_QUARTERS__impl(
        var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02, var_QUARTER_BYTE_03, var_QUARTER_BYTE_04,
        var_QUARTER_BYTE_05, var_QUARTER_BYTE_06, var_QUARTER_BYTE_07, var_QUARTER_BYTE_08, var_QUARTER_BYTE_09,
        var_QUARTER_BYTE_10, var_QUARTER_BYTE_11, var_QUARTER_BYTE_12, var_QUARTER_BYTE_13, var_QUARTER_BYTE_14,
        var_QUARTER_BYTE_15, var_QUARTER_BYTE_16, var_QUARTER_BYTE_17, var_QUARTER_BYTE_18, var_QUARTER_BYTE_19,
        var_QUARTER_BYTE_20, var_QUARTER_BYTE_21, var_QUARTER_BYTE_22, var_QUARTER_BYTE_23, var_QUARTER_BYTE_24,
        var_QUARTER_BYTE_25, var_QUARTER_BYTE_26, var_QUARTER_BYTE_27, var_QUARTER_BYTE_28, var_QUARTER_BYTE_29,
        var_QUARTER_BYTE_30, var_QUARTER_BYTE_31);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_QUARTERS(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_07,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_08,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_09,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_10,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_11,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_12,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_13,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_14,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_15,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_16,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_17,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_18,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_19,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_20,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_21,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_22,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_23,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_24,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_25,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_26,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_27,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_28,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_29,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_30,
                                               const CIEC_BYTE &st_lv_QUARTER_BYTE_31) {
    return func_ASSEMBLE_LWORD_FROM_QUARTERS__impl(
        st_lv_QUARTER_BYTE_00, st_lv_QUARTER_BYTE_01, st_lv_QUARTER_BYTE_02, st_lv_QUARTER_BYTE_03,
        st_lv_QUARTER_BYTE_04, st_lv_QUARTER_BYTE_05, st_lv_QUARTER_BYTE_06, st_lv_QUARTER_BYTE_07,
        st_lv_QUARTER_BYTE_08, st_lv_QUARTER_BYTE_09, st_lv_QUARTER_BYTE_10, st_lv_QUARTER_BYTE_11,
        st_lv_QUARTER_BYTE_12, st_lv_QUARTER_BYTE_13, st_lv_QUARTER_BYTE_14, st_lv_QUARTER_BYTE_15,
        st_lv_QUARTER_BYTE_16, st_lv_QUARTER_BYTE_17, st_lv_QUARTER_BYTE_18, st_lv_QUARTER_BYTE_19,
        st_lv_QUARTER_BYTE_20, st_lv_QUARTER_BYTE_21, st_lv_QUARTER_BYTE_22, st_lv_QUARTER_BYTE_23,
        st_lv_QUARTER_BYTE_24, st_lv_QUARTER_BYTE_25, st_lv_QUARTER_BYTE_26, st_lv_QUARTER_BYTE_27,
        st_lv_QUARTER_BYTE_28, st_lv_QUARTER_BYTE_29, st_lv_QUARTER_BYTE_30, st_lv_QUARTER_BYTE_31);
  }

  namespace {
    CIEC_LWORD func_ASSEMBLE_LWORD_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_07,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_08,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_09,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_10,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_11,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_12,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_13,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_14,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_15,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_16,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_17,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_18,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_19,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_20,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_21,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_22,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_23,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_24,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_25,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_26,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_27,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_28,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_29,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_30,
                                                       const CIEC_BYTE &st_lv_QUARTER_BYTE_31) {
      CIEC_LWORD st_ret_val = 0_LWORD;

#line 43 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_LWORD_QUARTER_ZZ;
#line 44 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_31),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_31));
#line 46 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_30),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_30));
#line 48 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_29),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_29));
#line 50 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_28),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_28));
#line 52 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_27),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_27));
#line 54 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_26),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_26));
#line 56 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_25),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_25));
#line 58 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_24),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_24));
#line 60 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_23),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_23));
#line 62 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_22),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_22));
#line 64 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_21),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_21));
#line 66 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_20),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_20));
#line 68 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_19),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_19));
#line 70 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_18),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_18));
#line 72 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_17),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_17));
#line 74 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_16),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_16));
#line 76 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_15),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_15));
#line 78 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_14),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_14));
#line 80 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_13),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_13));
#line 82 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_12),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_12));
#line 84 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_11),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_11));
#line 86 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_10),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_10));
#line 88 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_09),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_09));
#line 90 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_08),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_08));
#line 92 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_07),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_07));
#line 94 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_06),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_06));
#line 96 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_05),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_05));
#line 98 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_04),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_04));
#line 100 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_03),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03));
#line 102 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_02),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02));
#line 104 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_01),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01));
#line 106 "ASSEMBLE_LWORD_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_LWORD>(
          st_ret_val, func_SHL(func_BYTE_TO_LWORD(st_lv_QUARTER_BYTE_00),
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00));

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::assembling
