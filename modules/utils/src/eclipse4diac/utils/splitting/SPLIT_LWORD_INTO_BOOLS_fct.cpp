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
 *** Name: SPLIT_LWORD_INTO_BOOLS
 *** Description: this Function extracts the 64 BOOLS from a lword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_LWORD_INTO_BOOLS_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{
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

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_BOOLS, "eclipse4diac::utils::splitting::SPLIT_LWORD_INTO_BOOLS"_STRID)

  FORTE_SPLIT_LWORD_INTO_BOOLS::FORTE_SPLIT_LWORD_INTO_BOOLS(const forte::StringId paInstanceNameId,
                                                             CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
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
      conn_BIT_31(*this, 31, var_BIT_31),
      conn_BIT_32(*this, 32, var_BIT_32),
      conn_BIT_33(*this, 33, var_BIT_33),
      conn_BIT_34(*this, 34, var_BIT_34),
      conn_BIT_35(*this, 35, var_BIT_35),
      conn_BIT_36(*this, 36, var_BIT_36),
      conn_BIT_37(*this, 37, var_BIT_37),
      conn_BIT_38(*this, 38, var_BIT_38),
      conn_BIT_39(*this, 39, var_BIT_39),
      conn_BIT_40(*this, 40, var_BIT_40),
      conn_BIT_41(*this, 41, var_BIT_41),
      conn_BIT_42(*this, 42, var_BIT_42),
      conn_BIT_43(*this, 43, var_BIT_43),
      conn_BIT_44(*this, 44, var_BIT_44),
      conn_BIT_45(*this, 45, var_BIT_45),
      conn_BIT_46(*this, 46, var_BIT_46),
      conn_BIT_47(*this, 47, var_BIT_47),
      conn_BIT_48(*this, 48, var_BIT_48),
      conn_BIT_49(*this, 49, var_BIT_49),
      conn_BIT_50(*this, 50, var_BIT_50),
      conn_BIT_51(*this, 51, var_BIT_51),
      conn_BIT_52(*this, 52, var_BIT_52),
      conn_BIT_53(*this, 53, var_BIT_53),
      conn_BIT_54(*this, 54, var_BIT_54),
      conn_BIT_55(*this, 55, var_BIT_55),
      conn_BIT_56(*this, 56, var_BIT_56),
      conn_BIT_57(*this, 57, var_BIT_57),
      conn_BIT_58(*this, 58, var_BIT_58),
      conn_BIT_59(*this, 59, var_BIT_59),
      conn_BIT_60(*this, 60, var_BIT_60),
      conn_BIT_61(*this, 61, var_BIT_61),
      conn_BIT_62(*this, 62, var_BIT_62),
      conn_BIT_63(*this, 63, var_BIT_63) {
  }

  void FORTE_SPLIT_LWORD_INTO_BOOLS::setInitialValues() {
    var_IN = 0_LWORD;
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
  }

  void FORTE_SPLIT_LWORD_INTO_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_LWORD_INTO_BOOLS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_BIT_00, conn_BIT_00);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_BIT_01, conn_BIT_01);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_BIT_02, conn_BIT_02);
        writeData(cFBInterfaceSpec.getNumDIs() + 3, var_BIT_03, conn_BIT_03);
        writeData(cFBInterfaceSpec.getNumDIs() + 4, var_BIT_04, conn_BIT_04);
        writeData(cFBInterfaceSpec.getNumDIs() + 5, var_BIT_05, conn_BIT_05);
        writeData(cFBInterfaceSpec.getNumDIs() + 6, var_BIT_06, conn_BIT_06);
        writeData(cFBInterfaceSpec.getNumDIs() + 7, var_BIT_07, conn_BIT_07);
        writeData(cFBInterfaceSpec.getNumDIs() + 8, var_BIT_08, conn_BIT_08);
        writeData(cFBInterfaceSpec.getNumDIs() + 9, var_BIT_09, conn_BIT_09);
        writeData(cFBInterfaceSpec.getNumDIs() + 10, var_BIT_10, conn_BIT_10);
        writeData(cFBInterfaceSpec.getNumDIs() + 11, var_BIT_11, conn_BIT_11);
        writeData(cFBInterfaceSpec.getNumDIs() + 12, var_BIT_12, conn_BIT_12);
        writeData(cFBInterfaceSpec.getNumDIs() + 13, var_BIT_13, conn_BIT_13);
        writeData(cFBInterfaceSpec.getNumDIs() + 14, var_BIT_14, conn_BIT_14);
        writeData(cFBInterfaceSpec.getNumDIs() + 15, var_BIT_15, conn_BIT_15);
        writeData(cFBInterfaceSpec.getNumDIs() + 16, var_BIT_16, conn_BIT_16);
        writeData(cFBInterfaceSpec.getNumDIs() + 17, var_BIT_17, conn_BIT_17);
        writeData(cFBInterfaceSpec.getNumDIs() + 18, var_BIT_18, conn_BIT_18);
        writeData(cFBInterfaceSpec.getNumDIs() + 19, var_BIT_19, conn_BIT_19);
        writeData(cFBInterfaceSpec.getNumDIs() + 20, var_BIT_20, conn_BIT_20);
        writeData(cFBInterfaceSpec.getNumDIs() + 21, var_BIT_21, conn_BIT_21);
        writeData(cFBInterfaceSpec.getNumDIs() + 22, var_BIT_22, conn_BIT_22);
        writeData(cFBInterfaceSpec.getNumDIs() + 23, var_BIT_23, conn_BIT_23);
        writeData(cFBInterfaceSpec.getNumDIs() + 24, var_BIT_24, conn_BIT_24);
        writeData(cFBInterfaceSpec.getNumDIs() + 25, var_BIT_25, conn_BIT_25);
        writeData(cFBInterfaceSpec.getNumDIs() + 26, var_BIT_26, conn_BIT_26);
        writeData(cFBInterfaceSpec.getNumDIs() + 27, var_BIT_27, conn_BIT_27);
        writeData(cFBInterfaceSpec.getNumDIs() + 28, var_BIT_28, conn_BIT_28);
        writeData(cFBInterfaceSpec.getNumDIs() + 29, var_BIT_29, conn_BIT_29);
        writeData(cFBInterfaceSpec.getNumDIs() + 30, var_BIT_30, conn_BIT_30);
        writeData(cFBInterfaceSpec.getNumDIs() + 31, var_BIT_31, conn_BIT_31);
        writeData(cFBInterfaceSpec.getNumDIs() + 32, var_BIT_32, conn_BIT_32);
        writeData(cFBInterfaceSpec.getNumDIs() + 33, var_BIT_33, conn_BIT_33);
        writeData(cFBInterfaceSpec.getNumDIs() + 34, var_BIT_34, conn_BIT_34);
        writeData(cFBInterfaceSpec.getNumDIs() + 35, var_BIT_35, conn_BIT_35);
        writeData(cFBInterfaceSpec.getNumDIs() + 36, var_BIT_36, conn_BIT_36);
        writeData(cFBInterfaceSpec.getNumDIs() + 37, var_BIT_37, conn_BIT_37);
        writeData(cFBInterfaceSpec.getNumDIs() + 38, var_BIT_38, conn_BIT_38);
        writeData(cFBInterfaceSpec.getNumDIs() + 39, var_BIT_39, conn_BIT_39);
        writeData(cFBInterfaceSpec.getNumDIs() + 40, var_BIT_40, conn_BIT_40);
        writeData(cFBInterfaceSpec.getNumDIs() + 41, var_BIT_41, conn_BIT_41);
        writeData(cFBInterfaceSpec.getNumDIs() + 42, var_BIT_42, conn_BIT_42);
        writeData(cFBInterfaceSpec.getNumDIs() + 43, var_BIT_43, conn_BIT_43);
        writeData(cFBInterfaceSpec.getNumDIs() + 44, var_BIT_44, conn_BIT_44);
        writeData(cFBInterfaceSpec.getNumDIs() + 45, var_BIT_45, conn_BIT_45);
        writeData(cFBInterfaceSpec.getNumDIs() + 46, var_BIT_46, conn_BIT_46);
        writeData(cFBInterfaceSpec.getNumDIs() + 47, var_BIT_47, conn_BIT_47);
        writeData(cFBInterfaceSpec.getNumDIs() + 48, var_BIT_48, conn_BIT_48);
        writeData(cFBInterfaceSpec.getNumDIs() + 49, var_BIT_49, conn_BIT_49);
        writeData(cFBInterfaceSpec.getNumDIs() + 50, var_BIT_50, conn_BIT_50);
        writeData(cFBInterfaceSpec.getNumDIs() + 51, var_BIT_51, conn_BIT_51);
        writeData(cFBInterfaceSpec.getNumDIs() + 52, var_BIT_52, conn_BIT_52);
        writeData(cFBInterfaceSpec.getNumDIs() + 53, var_BIT_53, conn_BIT_53);
        writeData(cFBInterfaceSpec.getNumDIs() + 54, var_BIT_54, conn_BIT_54);
        writeData(cFBInterfaceSpec.getNumDIs() + 55, var_BIT_55, conn_BIT_55);
        writeData(cFBInterfaceSpec.getNumDIs() + 56, var_BIT_56, conn_BIT_56);
        writeData(cFBInterfaceSpec.getNumDIs() + 57, var_BIT_57, conn_BIT_57);
        writeData(cFBInterfaceSpec.getNumDIs() + 58, var_BIT_58, conn_BIT_58);
        writeData(cFBInterfaceSpec.getNumDIs() + 59, var_BIT_59, conn_BIT_59);
        writeData(cFBInterfaceSpec.getNumDIs() + 60, var_BIT_60, conn_BIT_60);
        writeData(cFBInterfaceSpec.getNumDIs() + 61, var_BIT_61, conn_BIT_61);
        writeData(cFBInterfaceSpec.getNumDIs() + 62, var_BIT_62, conn_BIT_62);
        writeData(cFBInterfaceSpec.getNumDIs() + 63, var_BIT_63, conn_BIT_63);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_BOOLS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_BOOLS::getDO(const size_t paIndex) {
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

  CEventConnection *FORTE_SPLIT_LWORD_INTO_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_LWORD_INTO_BOOLS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_LWORD_INTO_BOOLS::getDOConUnchecked(const TPortId paIndex) {
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

  void FORTE_SPLIT_LWORD_INTO_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_LWORD_INTO_BOOLS(
        var_IN, var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05, var_BIT_06, var_BIT_07,
        var_BIT_08, var_BIT_09, var_BIT_10, var_BIT_11, var_BIT_12, var_BIT_13, var_BIT_14, var_BIT_15, var_BIT_16,
        var_BIT_17, var_BIT_18, var_BIT_19, var_BIT_20, var_BIT_21, var_BIT_22, var_BIT_23, var_BIT_24, var_BIT_25,
        var_BIT_26, var_BIT_27, var_BIT_28, var_BIT_29, var_BIT_30, var_BIT_31, var_BIT_32, var_BIT_33, var_BIT_34,
        var_BIT_35, var_BIT_36, var_BIT_37, var_BIT_38, var_BIT_39, var_BIT_40, var_BIT_41, var_BIT_42, var_BIT_43,
        var_BIT_44, var_BIT_45, var_BIT_46, var_BIT_47, var_BIT_48, var_BIT_49, var_BIT_50, var_BIT_51, var_BIT_52,
        var_BIT_53, var_BIT_54, var_BIT_55, var_BIT_56, var_BIT_57, var_BIT_58, var_BIT_59, var_BIT_60, var_BIT_61,
        var_BIT_62, var_BIT_63);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_LWORD_INTO_BOOLS(CIEC_LWORD st_lv_IN,
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
                                   CIEC_BOOL &st_lv_BIT_31,
                                   CIEC_BOOL &st_lv_BIT_32,
                                   CIEC_BOOL &st_lv_BIT_33,
                                   CIEC_BOOL &st_lv_BIT_34,
                                   CIEC_BOOL &st_lv_BIT_35,
                                   CIEC_BOOL &st_lv_BIT_36,
                                   CIEC_BOOL &st_lv_BIT_37,
                                   CIEC_BOOL &st_lv_BIT_38,
                                   CIEC_BOOL &st_lv_BIT_39,
                                   CIEC_BOOL &st_lv_BIT_40,
                                   CIEC_BOOL &st_lv_BIT_41,
                                   CIEC_BOOL &st_lv_BIT_42,
                                   CIEC_BOOL &st_lv_BIT_43,
                                   CIEC_BOOL &st_lv_BIT_44,
                                   CIEC_BOOL &st_lv_BIT_45,
                                   CIEC_BOOL &st_lv_BIT_46,
                                   CIEC_BOOL &st_lv_BIT_47,
                                   CIEC_BOOL &st_lv_BIT_48,
                                   CIEC_BOOL &st_lv_BIT_49,
                                   CIEC_BOOL &st_lv_BIT_50,
                                   CIEC_BOOL &st_lv_BIT_51,
                                   CIEC_BOOL &st_lv_BIT_52,
                                   CIEC_BOOL &st_lv_BIT_53,
                                   CIEC_BOOL &st_lv_BIT_54,
                                   CIEC_BOOL &st_lv_BIT_55,
                                   CIEC_BOOL &st_lv_BIT_56,
                                   CIEC_BOOL &st_lv_BIT_57,
                                   CIEC_BOOL &st_lv_BIT_58,
                                   CIEC_BOOL &st_lv_BIT_59,
                                   CIEC_BOOL &st_lv_BIT_60,
                                   CIEC_BOOL &st_lv_BIT_61,
                                   CIEC_BOOL &st_lv_BIT_62,
                                   CIEC_BOOL &st_lv_BIT_63) {
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
    st_lv_BIT_32 = 0_BOOL;
    st_lv_BIT_33 = 0_BOOL;
    st_lv_BIT_34 = 0_BOOL;
    st_lv_BIT_35 = 0_BOOL;
    st_lv_BIT_36 = 0_BOOL;
    st_lv_BIT_37 = 0_BOOL;
    st_lv_BIT_38 = 0_BOOL;
    st_lv_BIT_39 = 0_BOOL;
    st_lv_BIT_40 = 0_BOOL;
    st_lv_BIT_41 = 0_BOOL;
    st_lv_BIT_42 = 0_BOOL;
    st_lv_BIT_43 = 0_BOOL;
    st_lv_BIT_44 = 0_BOOL;
    st_lv_BIT_45 = 0_BOOL;
    st_lv_BIT_46 = 0_BOOL;
    st_lv_BIT_47 = 0_BOOL;
    st_lv_BIT_48 = 0_BOOL;
    st_lv_BIT_49 = 0_BOOL;
    st_lv_BIT_50 = 0_BOOL;
    st_lv_BIT_51 = 0_BOOL;
    st_lv_BIT_52 = 0_BOOL;
    st_lv_BIT_53 = 0_BOOL;
    st_lv_BIT_54 = 0_BOOL;
    st_lv_BIT_55 = 0_BOOL;
    st_lv_BIT_56 = 0_BOOL;
    st_lv_BIT_57 = 0_BOOL;
    st_lv_BIT_58 = 0_BOOL;
    st_lv_BIT_59 = 0_BOOL;
    st_lv_BIT_60 = 0_BOOL;
    st_lv_BIT_61 = 0_BOOL;
    st_lv_BIT_62 = 0_BOOL;
    st_lv_BIT_63 = 0_BOOL;

#line 75 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_00 = st_lv_IN.cpartial<CIEC_BOOL>(0);
#line 76 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
#line 77 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_02 = st_lv_IN.cpartial<CIEC_BOOL>(2);
#line 78 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_03 = st_lv_IN.cpartial<CIEC_BOOL>(3);
#line 79 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_04 = st_lv_IN.cpartial<CIEC_BOOL>(4);
#line 80 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_05 = st_lv_IN.cpartial<CIEC_BOOL>(5);
#line 81 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_06 = st_lv_IN.cpartial<CIEC_BOOL>(6);
#line 82 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_07 = st_lv_IN.cpartial<CIEC_BOOL>(7);
#line 83 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_08 = st_lv_IN.cpartial<CIEC_BOOL>(8);
#line 84 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_09 = st_lv_IN.cpartial<CIEC_BOOL>(9);
#line 85 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_10 = st_lv_IN.cpartial<CIEC_BOOL>(10);
#line 86 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_11 = st_lv_IN.cpartial<CIEC_BOOL>(11);
#line 87 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_12 = st_lv_IN.cpartial<CIEC_BOOL>(12);
#line 88 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_13 = st_lv_IN.cpartial<CIEC_BOOL>(13);
#line 89 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_14 = st_lv_IN.cpartial<CIEC_BOOL>(14);
#line 90 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_15 = st_lv_IN.cpartial<CIEC_BOOL>(15);
#line 91 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_16 = st_lv_IN.cpartial<CIEC_BOOL>(16);
#line 92 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_17 = st_lv_IN.cpartial<CIEC_BOOL>(17);
#line 93 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_18 = st_lv_IN.cpartial<CIEC_BOOL>(18);
#line 94 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_19 = st_lv_IN.cpartial<CIEC_BOOL>(19);
#line 95 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_20 = st_lv_IN.cpartial<CIEC_BOOL>(20);
#line 96 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_21 = st_lv_IN.cpartial<CIEC_BOOL>(21);
#line 97 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_22 = st_lv_IN.cpartial<CIEC_BOOL>(22);
#line 98 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_23 = st_lv_IN.cpartial<CIEC_BOOL>(23);
#line 99 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_24 = st_lv_IN.cpartial<CIEC_BOOL>(24);
#line 100 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_25 = st_lv_IN.cpartial<CIEC_BOOL>(25);
#line 101 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_26 = st_lv_IN.cpartial<CIEC_BOOL>(26);
#line 102 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_27 = st_lv_IN.cpartial<CIEC_BOOL>(27);
#line 103 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_28 = st_lv_IN.cpartial<CIEC_BOOL>(28);
#line 104 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_29 = st_lv_IN.cpartial<CIEC_BOOL>(29);
#line 105 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_30 = st_lv_IN.cpartial<CIEC_BOOL>(30);
#line 106 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_31 = st_lv_IN.cpartial<CIEC_BOOL>(31);
#line 107 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_32 = st_lv_IN.cpartial<CIEC_BOOL>(32);
#line 108 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_33 = st_lv_IN.cpartial<CIEC_BOOL>(33);
#line 109 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_34 = st_lv_IN.cpartial<CIEC_BOOL>(34);
#line 110 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_35 = st_lv_IN.cpartial<CIEC_BOOL>(35);
#line 111 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_36 = st_lv_IN.cpartial<CIEC_BOOL>(36);
#line 112 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_37 = st_lv_IN.cpartial<CIEC_BOOL>(37);
#line 113 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_38 = st_lv_IN.cpartial<CIEC_BOOL>(38);
#line 114 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_39 = st_lv_IN.cpartial<CIEC_BOOL>(39);
#line 115 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_40 = st_lv_IN.cpartial<CIEC_BOOL>(40);
#line 116 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_41 = st_lv_IN.cpartial<CIEC_BOOL>(41);
#line 117 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_42 = st_lv_IN.cpartial<CIEC_BOOL>(42);
#line 118 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_43 = st_lv_IN.cpartial<CIEC_BOOL>(43);
#line 119 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_44 = st_lv_IN.cpartial<CIEC_BOOL>(44);
#line 120 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_45 = st_lv_IN.cpartial<CIEC_BOOL>(45);
#line 121 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_46 = st_lv_IN.cpartial<CIEC_BOOL>(46);
#line 122 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_47 = st_lv_IN.cpartial<CIEC_BOOL>(47);
#line 123 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_48 = st_lv_IN.cpartial<CIEC_BOOL>(48);
#line 124 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_49 = st_lv_IN.cpartial<CIEC_BOOL>(49);
#line 125 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_50 = st_lv_IN.cpartial<CIEC_BOOL>(50);
#line 126 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_51 = st_lv_IN.cpartial<CIEC_BOOL>(51);
#line 127 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_52 = st_lv_IN.cpartial<CIEC_BOOL>(52);
#line 128 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_53 = st_lv_IN.cpartial<CIEC_BOOL>(53);
#line 129 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_54 = st_lv_IN.cpartial<CIEC_BOOL>(54);
#line 130 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_55 = st_lv_IN.cpartial<CIEC_BOOL>(55);
#line 131 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_56 = st_lv_IN.cpartial<CIEC_BOOL>(56);
#line 132 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_57 = st_lv_IN.cpartial<CIEC_BOOL>(57);
#line 133 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_58 = st_lv_IN.cpartial<CIEC_BOOL>(58);
#line 134 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_59 = st_lv_IN.cpartial<CIEC_BOOL>(59);
#line 135 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_60 = st_lv_IN.cpartial<CIEC_BOOL>(60);
#line 136 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_61 = st_lv_IN.cpartial<CIEC_BOOL>(61);
#line 137 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_62 = st_lv_IN.cpartial<CIEC_BOOL>(62);
#line 138 "SPLIT_LWORD_INTO_BOOLS.fct"
    st_lv_BIT_63 = st_lv_IN.cpartial<CIEC_BOOL>(63);
  }
} // namespace forte::eclipse4diac::utils::splitting
