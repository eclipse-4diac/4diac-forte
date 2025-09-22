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
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ASSEMBLE_LWORD_FROM_BOOLS
 *** Description: this Function combines the 64 BOOLS to a LWORD
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_BOOLS_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_BOOLS_fct.h"

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    const auto cDataInputNames = std::array{
        "BIT_00"_STRID, "BIT_01"_STRID, "BIT_02"_STRID, "BIT_03"_STRID, "BIT_04"_STRID, "BIT_05"_STRID, "BIT_06"_STRID,
        "BIT_07"_STRID, "BIT_08"_STRID, "BIT_09"_STRID, "BIT_10"_STRID, "BIT_11"_STRID, "BIT_12"_STRID, "BIT_13"_STRID,
        "BIT_14"_STRID, "BIT_15"_STRID, "BIT_16"_STRID, "BIT_17"_STRID, "BIT_18"_STRID, "BIT_19"_STRID, "BIT_20"_STRID,
        "BIT_21"_STRID, "BIT_22"_STRID, "BIT_23"_STRID, "BIT_24"_STRID, "BIT_25"_STRID, "BIT_26"_STRID, "BIT_27"_STRID,
        "BIT_28"_STRID, "BIT_29"_STRID, "BIT_30"_STRID, "BIT_31"_STRID, "BIT_32"_STRID, "BIT_33"_STRID, "BIT_34"_STRID,
        "BIT_35"_STRID, "BIT_36"_STRID, "BIT_37"_STRID, "BIT_38"_STRID, "BIT_39"_STRID, "BIT_40"_STRID, "BIT_41"_STRID,
        "BIT_42"_STRID, "BIT_43"_STRID, "BIT_44"_STRID, "BIT_45"_STRID, "BIT_46"_STRID, "BIT_47"_STRID, "BIT_48"_STRID,
        "BIT_49"_STRID, "BIT_50"_STRID, "BIT_51"_STRID, "BIT_52"_STRID, "BIT_53"_STRID, "BIT_54"_STRID, "BIT_55"_STRID,
        "BIT_56"_STRID, "BIT_57"_STRID, "BIT_58"_STRID, "BIT_59"_STRID, "BIT_60"_STRID, "BIT_61"_STRID, "BIT_62"_STRID,
        "BIT_63"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_BOOLS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_LWORD_FROM_BOOLS"_STRID)

  FORTE_ASSEMBLE_LWORD_FROM_BOOLS::FORTE_ASSEMBLE_LWORD_FROM_BOOLS(const forte::StringId paInstanceNameId,
                                                                   CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_BIT_00(nullptr),
      conn_BIT_01(nullptr),
      conn_BIT_02(nullptr),
      conn_BIT_03(nullptr),
      conn_BIT_04(nullptr),
      conn_BIT_05(nullptr),
      conn_BIT_06(nullptr),
      conn_BIT_07(nullptr),
      conn_BIT_08(nullptr),
      conn_BIT_09(nullptr),
      conn_BIT_10(nullptr),
      conn_BIT_11(nullptr),
      conn_BIT_12(nullptr),
      conn_BIT_13(nullptr),
      conn_BIT_14(nullptr),
      conn_BIT_15(nullptr),
      conn_BIT_16(nullptr),
      conn_BIT_17(nullptr),
      conn_BIT_18(nullptr),
      conn_BIT_19(nullptr),
      conn_BIT_20(nullptr),
      conn_BIT_21(nullptr),
      conn_BIT_22(nullptr),
      conn_BIT_23(nullptr),
      conn_BIT_24(nullptr),
      conn_BIT_25(nullptr),
      conn_BIT_26(nullptr),
      conn_BIT_27(nullptr),
      conn_BIT_28(nullptr),
      conn_BIT_29(nullptr),
      conn_BIT_30(nullptr),
      conn_BIT_31(nullptr),
      conn_BIT_32(nullptr),
      conn_BIT_33(nullptr),
      conn_BIT_34(nullptr),
      conn_BIT_35(nullptr),
      conn_BIT_36(nullptr),
      conn_BIT_37(nullptr),
      conn_BIT_38(nullptr),
      conn_BIT_39(nullptr),
      conn_BIT_40(nullptr),
      conn_BIT_41(nullptr),
      conn_BIT_42(nullptr),
      conn_BIT_43(nullptr),
      conn_BIT_44(nullptr),
      conn_BIT_45(nullptr),
      conn_BIT_46(nullptr),
      conn_BIT_47(nullptr),
      conn_BIT_48(nullptr),
      conn_BIT_49(nullptr),
      conn_BIT_50(nullptr),
      conn_BIT_51(nullptr),
      conn_BIT_52(nullptr),
      conn_BIT_53(nullptr),
      conn_BIT_54(nullptr),
      conn_BIT_55(nullptr),
      conn_BIT_56(nullptr),
      conn_BIT_57(nullptr),
      conn_BIT_58(nullptr),
      conn_BIT_59(nullptr),
      conn_BIT_60(nullptr),
      conn_BIT_61(nullptr),
      conn_BIT_62(nullptr),
      conn_BIT_63(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_LWORD_FROM_BOOLS::setInitialValues() {
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
    var_BIT_32 = 0_BOOL;
    var_BIT_33 = 0_BOOL;
    var_BIT_34 = 0_BOOL;
    var_BIT_35 = 0_BOOL;
    var_BIT_36 = 0_BOOL;
    var_BIT_37 = 0_BOOL;
    var_BIT_38 = 0_BOOL;
    var_BIT_39 = 0_BOOL;
    var_BIT_40 = 0_BOOL;
    var_BIT_41 = 0_BOOL;
    var_BIT_42 = 0_BOOL;
    var_BIT_43 = 0_BOOL;
    var_BIT_44 = 0_BOOL;
    var_BIT_45 = 0_BOOL;
    var_BIT_46 = 0_BOOL;
    var_BIT_47 = 0_BOOL;
    var_BIT_48 = 0_BOOL;
    var_BIT_49 = 0_BOOL;
    var_BIT_50 = 0_BOOL;
    var_BIT_51 = 0_BOOL;
    var_BIT_52 = 0_BOOL;
    var_BIT_53 = 0_BOOL;
    var_BIT_54 = 0_BOOL;
    var_BIT_55 = 0_BOOL;
    var_BIT_56 = 0_BOOL;
    var_BIT_57 = 0_BOOL;
    var_BIT_58 = 0_BOOL;
    var_BIT_59 = 0_BOOL;
    var_BIT_60 = 0_BOOL;
    var_BIT_61 = 0_BOOL;
    var_BIT_62 = 0_BOOL;
    var_BIT_63 = 0_BOOL;
    var_ = 0_LWORD;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_BIT_00, conn_BIT_00);
        readData(1, var_BIT_01, conn_BIT_01);
        readData(2, var_BIT_02, conn_BIT_02);
        readData(3, var_BIT_03, conn_BIT_03);
        readData(4, var_BIT_04, conn_BIT_04);
        readData(5, var_BIT_05, conn_BIT_05);
        readData(6, var_BIT_06, conn_BIT_06);
        readData(7, var_BIT_07, conn_BIT_07);
        readData(8, var_BIT_08, conn_BIT_08);
        readData(9, var_BIT_09, conn_BIT_09);
        readData(10, var_BIT_10, conn_BIT_10);
        readData(11, var_BIT_11, conn_BIT_11);
        readData(12, var_BIT_12, conn_BIT_12);
        readData(13, var_BIT_13, conn_BIT_13);
        readData(14, var_BIT_14, conn_BIT_14);
        readData(15, var_BIT_15, conn_BIT_15);
        readData(16, var_BIT_16, conn_BIT_16);
        readData(17, var_BIT_17, conn_BIT_17);
        readData(18, var_BIT_18, conn_BIT_18);
        readData(19, var_BIT_19, conn_BIT_19);
        readData(20, var_BIT_20, conn_BIT_20);
        readData(21, var_BIT_21, conn_BIT_21);
        readData(22, var_BIT_22, conn_BIT_22);
        readData(23, var_BIT_23, conn_BIT_23);
        readData(24, var_BIT_24, conn_BIT_24);
        readData(25, var_BIT_25, conn_BIT_25);
        readData(26, var_BIT_26, conn_BIT_26);
        readData(27, var_BIT_27, conn_BIT_27);
        readData(28, var_BIT_28, conn_BIT_28);
        readData(29, var_BIT_29, conn_BIT_29);
        readData(30, var_BIT_30, conn_BIT_30);
        readData(31, var_BIT_31, conn_BIT_31);
        readData(32, var_BIT_32, conn_BIT_32);
        readData(33, var_BIT_33, conn_BIT_33);
        readData(34, var_BIT_34, conn_BIT_34);
        readData(35, var_BIT_35, conn_BIT_35);
        readData(36, var_BIT_36, conn_BIT_36);
        readData(37, var_BIT_37, conn_BIT_37);
        readData(38, var_BIT_38, conn_BIT_38);
        readData(39, var_BIT_39, conn_BIT_39);
        readData(40, var_BIT_40, conn_BIT_40);
        readData(41, var_BIT_41, conn_BIT_41);
        readData(42, var_BIT_42, conn_BIT_42);
        readData(43, var_BIT_43, conn_BIT_43);
        readData(44, var_BIT_44, conn_BIT_44);
        readData(45, var_BIT_45, conn_BIT_45);
        readData(46, var_BIT_46, conn_BIT_46);
        readData(47, var_BIT_47, conn_BIT_47);
        readData(48, var_BIT_48, conn_BIT_48);
        readData(49, var_BIT_49, conn_BIT_49);
        readData(50, var_BIT_50, conn_BIT_50);
        readData(51, var_BIT_51, conn_BIT_51);
        readData(52, var_BIT_52, conn_BIT_52);
        readData(53, var_BIT_53, conn_BIT_53);
        readData(54, var_BIT_54, conn_BIT_54);
        readData(55, var_BIT_55, conn_BIT_55);
        readData(56, var_BIT_56, conn_BIT_56);
        readData(57, var_BIT_57, conn_BIT_57);
        readData(58, var_BIT_58, conn_BIT_58);
        readData(59, var_BIT_59, conn_BIT_59);
        readData(60, var_BIT_60, conn_BIT_60);
        readData(61, var_BIT_61, conn_BIT_61);
        readData(62, var_BIT_62, conn_BIT_62);
        readData(63, var_BIT_63, conn_BIT_63);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_LWORD_FROM_BOOLS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_BOOLS::getDI(const size_t paIndex) {
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
      case 32: return &var_BIT_32;
      case 33: return &var_BIT_33;
      case 34: return &var_BIT_34;
      case 35: return &var_BIT_35;
      case 36: return &var_BIT_36;
      case 37: return &var_BIT_37;
      case 38: return &var_BIT_38;
      case 39: return &var_BIT_39;
      case 40: return &var_BIT_40;
      case 41: return &var_BIT_41;
      case 42: return &var_BIT_42;
      case 43: return &var_BIT_43;
      case 44: return &var_BIT_44;
      case 45: return &var_BIT_45;
      case 46: return &var_BIT_46;
      case 47: return &var_BIT_47;
      case 48: return &var_BIT_48;
      case 49: return &var_BIT_49;
      case 50: return &var_BIT_50;
      case 51: return &var_BIT_51;
      case 52: return &var_BIT_52;
      case 53: return &var_BIT_53;
      case 54: return &var_BIT_54;
      case 55: return &var_BIT_55;
      case 56: return &var_BIT_56;
      case 57: return &var_BIT_57;
      case 58: return &var_BIT_58;
      case 59: return &var_BIT_59;
      case 60: return &var_BIT_60;
      case 61: return &var_BIT_61;
      case 62: return &var_BIT_62;
      case 63: return &var_BIT_63;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_BOOLS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_LWORD_FROM_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_LWORD_FROM_BOOLS::getDIConUnchecked(const TPortId paIndex) {
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
      case 32: return &conn_BIT_32;
      case 33: return &conn_BIT_33;
      case 34: return &conn_BIT_34;
      case 35: return &conn_BIT_35;
      case 36: return &conn_BIT_36;
      case 37: return &conn_BIT_37;
      case 38: return &conn_BIT_38;
      case 39: return &conn_BIT_39;
      case 40: return &conn_BIT_40;
      case 41: return &conn_BIT_41;
      case 42: return &conn_BIT_42;
      case 43: return &conn_BIT_43;
      case 44: return &conn_BIT_44;
      case 45: return &conn_BIT_45;
      case 46: return &conn_BIT_46;
      case 47: return &conn_BIT_47;
      case 48: return &conn_BIT_48;
      case 49: return &conn_BIT_49;
      case 50: return &conn_BIT_50;
      case 51: return &conn_BIT_51;
      case 52: return &conn_BIT_52;
      case 53: return &conn_BIT_53;
      case 54: return &conn_BIT_54;
      case 55: return &conn_BIT_55;
      case 56: return &conn_BIT_56;
      case 57: return &conn_BIT_57;
      case 58: return &conn_BIT_58;
      case 59: return &conn_BIT_59;
      case 60: return &conn_BIT_60;
      case 61: return &conn_BIT_61;
      case 62: return &conn_BIT_62;
      case 63: return &conn_BIT_63;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_LWORD_FROM_BOOLS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_LWORD_FROM_BOOLS(
        var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05, var_BIT_06, var_BIT_07, var_BIT_08,
        var_BIT_09, var_BIT_10, var_BIT_11, var_BIT_12, var_BIT_13, var_BIT_14, var_BIT_15, var_BIT_16, var_BIT_17,
        var_BIT_18, var_BIT_19, var_BIT_20, var_BIT_21, var_BIT_22, var_BIT_23, var_BIT_24, var_BIT_25, var_BIT_26,
        var_BIT_27, var_BIT_28, var_BIT_29, var_BIT_30, var_BIT_31, var_BIT_32, var_BIT_33, var_BIT_34, var_BIT_35,
        var_BIT_36, var_BIT_37, var_BIT_38, var_BIT_39, var_BIT_40, var_BIT_41, var_BIT_42, var_BIT_43, var_BIT_44,
        var_BIT_45, var_BIT_46, var_BIT_47, var_BIT_48, var_BIT_49, var_BIT_50, var_BIT_51, var_BIT_52, var_BIT_53,
        var_BIT_54, var_BIT_55, var_BIT_56, var_BIT_57, var_BIT_58, var_BIT_59, var_BIT_60, var_BIT_61, var_BIT_62,
        var_BIT_63);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_BOOLS(CIEC_BOOL st_lv_BIT_00,
                                            CIEC_BOOL st_lv_BIT_01,
                                            CIEC_BOOL st_lv_BIT_02,
                                            CIEC_BOOL st_lv_BIT_03,
                                            CIEC_BOOL st_lv_BIT_04,
                                            CIEC_BOOL st_lv_BIT_05,
                                            CIEC_BOOL st_lv_BIT_06,
                                            CIEC_BOOL st_lv_BIT_07,
                                            CIEC_BOOL st_lv_BIT_08,
                                            CIEC_BOOL st_lv_BIT_09,
                                            CIEC_BOOL st_lv_BIT_10,
                                            CIEC_BOOL st_lv_BIT_11,
                                            CIEC_BOOL st_lv_BIT_12,
                                            CIEC_BOOL st_lv_BIT_13,
                                            CIEC_BOOL st_lv_BIT_14,
                                            CIEC_BOOL st_lv_BIT_15,
                                            CIEC_BOOL st_lv_BIT_16,
                                            CIEC_BOOL st_lv_BIT_17,
                                            CIEC_BOOL st_lv_BIT_18,
                                            CIEC_BOOL st_lv_BIT_19,
                                            CIEC_BOOL st_lv_BIT_20,
                                            CIEC_BOOL st_lv_BIT_21,
                                            CIEC_BOOL st_lv_BIT_22,
                                            CIEC_BOOL st_lv_BIT_23,
                                            CIEC_BOOL st_lv_BIT_24,
                                            CIEC_BOOL st_lv_BIT_25,
                                            CIEC_BOOL st_lv_BIT_26,
                                            CIEC_BOOL st_lv_BIT_27,
                                            CIEC_BOOL st_lv_BIT_28,
                                            CIEC_BOOL st_lv_BIT_29,
                                            CIEC_BOOL st_lv_BIT_30,
                                            CIEC_BOOL st_lv_BIT_31,
                                            CIEC_BOOL st_lv_BIT_32,
                                            CIEC_BOOL st_lv_BIT_33,
                                            CIEC_BOOL st_lv_BIT_34,
                                            CIEC_BOOL st_lv_BIT_35,
                                            CIEC_BOOL st_lv_BIT_36,
                                            CIEC_BOOL st_lv_BIT_37,
                                            CIEC_BOOL st_lv_BIT_38,
                                            CIEC_BOOL st_lv_BIT_39,
                                            CIEC_BOOL st_lv_BIT_40,
                                            CIEC_BOOL st_lv_BIT_41,
                                            CIEC_BOOL st_lv_BIT_42,
                                            CIEC_BOOL st_lv_BIT_43,
                                            CIEC_BOOL st_lv_BIT_44,
                                            CIEC_BOOL st_lv_BIT_45,
                                            CIEC_BOOL st_lv_BIT_46,
                                            CIEC_BOOL st_lv_BIT_47,
                                            CIEC_BOOL st_lv_BIT_48,
                                            CIEC_BOOL st_lv_BIT_49,
                                            CIEC_BOOL st_lv_BIT_50,
                                            CIEC_BOOL st_lv_BIT_51,
                                            CIEC_BOOL st_lv_BIT_52,
                                            CIEC_BOOL st_lv_BIT_53,
                                            CIEC_BOOL st_lv_BIT_54,
                                            CIEC_BOOL st_lv_BIT_55,
                                            CIEC_BOOL st_lv_BIT_56,
                                            CIEC_BOOL st_lv_BIT_57,
                                            CIEC_BOOL st_lv_BIT_58,
                                            CIEC_BOOL st_lv_BIT_59,
                                            CIEC_BOOL st_lv_BIT_60,
                                            CIEC_BOOL st_lv_BIT_61,
                                            CIEC_BOOL st_lv_BIT_62,
                                            CIEC_BOOL st_lv_BIT_63) {
    CIEC_LWORD st_ret_val = 0_LWORD;

#line 71 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(0) = st_lv_BIT_00;
#line 72 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(1) = st_lv_BIT_01;
#line 73 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(2) = st_lv_BIT_02;
#line 74 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(3) = st_lv_BIT_03;
#line 75 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(4) = st_lv_BIT_04;
#line 76 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(5) = st_lv_BIT_05;
#line 77 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(6) = st_lv_BIT_06;
#line 78 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(7) = st_lv_BIT_07;
#line 79 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(8) = st_lv_BIT_08;
#line 80 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(9) = st_lv_BIT_09;
#line 81 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(10) = st_lv_BIT_10;
#line 82 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(11) = st_lv_BIT_11;
#line 83 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(12) = st_lv_BIT_12;
#line 84 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(13) = st_lv_BIT_13;
#line 85 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(14) = st_lv_BIT_14;
#line 86 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(15) = st_lv_BIT_15;
#line 87 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(16) = st_lv_BIT_16;
#line 88 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(17) = st_lv_BIT_17;
#line 89 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(18) = st_lv_BIT_18;
#line 90 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(19) = st_lv_BIT_19;
#line 91 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(20) = st_lv_BIT_20;
#line 92 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(21) = st_lv_BIT_21;
#line 93 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(22) = st_lv_BIT_22;
#line 94 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(23) = st_lv_BIT_23;
#line 95 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(24) = st_lv_BIT_24;
#line 96 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(25) = st_lv_BIT_25;
#line 97 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(26) = st_lv_BIT_26;
#line 98 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(27) = st_lv_BIT_27;
#line 99 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(28) = st_lv_BIT_28;
#line 100 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(29) = st_lv_BIT_29;
#line 101 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(30) = st_lv_BIT_30;
#line 102 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(31) = st_lv_BIT_31;
#line 103 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(32) = st_lv_BIT_32;
#line 104 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(33) = st_lv_BIT_33;
#line 105 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(34) = st_lv_BIT_34;
#line 106 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(35) = st_lv_BIT_35;
#line 107 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(36) = st_lv_BIT_36;
#line 108 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(37) = st_lv_BIT_37;
#line 109 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(38) = st_lv_BIT_38;
#line 110 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(39) = st_lv_BIT_39;
#line 111 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(40) = st_lv_BIT_40;
#line 112 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(41) = st_lv_BIT_41;
#line 113 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(42) = st_lv_BIT_42;
#line 114 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(43) = st_lv_BIT_43;
#line 115 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(44) = st_lv_BIT_44;
#line 116 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(45) = st_lv_BIT_45;
#line 117 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(46) = st_lv_BIT_46;
#line 118 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(47) = st_lv_BIT_47;
#line 119 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(48) = st_lv_BIT_48;
#line 120 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(49) = st_lv_BIT_49;
#line 121 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(50) = st_lv_BIT_50;
#line 122 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(51) = st_lv_BIT_51;
#line 123 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(52) = st_lv_BIT_52;
#line 124 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(53) = st_lv_BIT_53;
#line 125 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(54) = st_lv_BIT_54;
#line 126 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(55) = st_lv_BIT_55;
#line 127 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(56) = st_lv_BIT_56;
#line 128 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(57) = st_lv_BIT_57;
#line 129 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(58) = st_lv_BIT_58;
#line 130 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(59) = st_lv_BIT_59;
#line 131 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(60) = st_lv_BIT_60;
#line 132 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(61) = st_lv_BIT_61;
#line 133 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(62) = st_lv_BIT_62;
#line 134 "ASSEMBLE_LWORD_FROM_BOOLS.fct"
    st_ret_val.partial<CIEC_BOOL>(63) = st_lv_BIT_63;

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::utils::assembling
