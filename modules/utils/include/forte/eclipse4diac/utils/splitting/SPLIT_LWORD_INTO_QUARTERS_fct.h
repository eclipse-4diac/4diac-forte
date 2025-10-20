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
 *** Name: SPLIT_LWORD_INTO_QUARTERS
 *** Description: this Function extracts the 32 QUARTER BYTE from a lword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_LWORD_INTO_QUARTERS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_QUARTERS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_LWORD_INTO_QUARTERS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_LWORD var_IN;

      CIEC_BYTE var_QUARTER_BYTE_00;
      CIEC_BYTE var_QUARTER_BYTE_01;
      CIEC_BYTE var_QUARTER_BYTE_02;
      CIEC_BYTE var_QUARTER_BYTE_03;
      CIEC_BYTE var_QUARTER_BYTE_04;
      CIEC_BYTE var_QUARTER_BYTE_05;
      CIEC_BYTE var_QUARTER_BYTE_06;
      CIEC_BYTE var_QUARTER_BYTE_07;
      CIEC_BYTE var_QUARTER_BYTE_08;
      CIEC_BYTE var_QUARTER_BYTE_09;
      CIEC_BYTE var_QUARTER_BYTE_10;
      CIEC_BYTE var_QUARTER_BYTE_11;
      CIEC_BYTE var_QUARTER_BYTE_12;
      CIEC_BYTE var_QUARTER_BYTE_13;
      CIEC_BYTE var_QUARTER_BYTE_14;
      CIEC_BYTE var_QUARTER_BYTE_15;
      CIEC_BYTE var_QUARTER_BYTE_16;
      CIEC_BYTE var_QUARTER_BYTE_17;
      CIEC_BYTE var_QUARTER_BYTE_18;
      CIEC_BYTE var_QUARTER_BYTE_19;
      CIEC_BYTE var_QUARTER_BYTE_20;
      CIEC_BYTE var_QUARTER_BYTE_21;
      CIEC_BYTE var_QUARTER_BYTE_22;
      CIEC_BYTE var_QUARTER_BYTE_23;
      CIEC_BYTE var_QUARTER_BYTE_24;
      CIEC_BYTE var_QUARTER_BYTE_25;
      CIEC_BYTE var_QUARTER_BYTE_26;
      CIEC_BYTE var_QUARTER_BYTE_27;
      CIEC_BYTE var_QUARTER_BYTE_28;
      CIEC_BYTE var_QUARTER_BYTE_29;
      CIEC_BYTE var_QUARTER_BYTE_30;
      CIEC_BYTE var_QUARTER_BYTE_31;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_00;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_01;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_02;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_03;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_04;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_05;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_06;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_07;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_08;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_09;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_10;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_11;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_12;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_13;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_14;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_15;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_16;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_17;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_18;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_19;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_20;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_21;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_22;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_23;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_24;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_25;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_26;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_27;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_28;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_29;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_30;
      COutDataConnection<CIEC_BYTE> conn_QUARTER_BYTE_31;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_LWORD &paIN,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_00,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_01,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_02,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_03,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_04,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_05,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_06,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_07,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_08,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_09,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_10,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_11,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_12,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_13,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_14,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_15,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_16,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_17,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_18,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_19,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_20,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_21,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_22,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_23,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_24,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_25,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_26,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_27,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_28,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_29,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_30,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_31) {
        COutputGuard guard_paQUARTER_BYTE_00(paQUARTER_BYTE_00);
        COutputGuard guard_paQUARTER_BYTE_01(paQUARTER_BYTE_01);
        COutputGuard guard_paQUARTER_BYTE_02(paQUARTER_BYTE_02);
        COutputGuard guard_paQUARTER_BYTE_03(paQUARTER_BYTE_03);
        COutputGuard guard_paQUARTER_BYTE_04(paQUARTER_BYTE_04);
        COutputGuard guard_paQUARTER_BYTE_05(paQUARTER_BYTE_05);
        COutputGuard guard_paQUARTER_BYTE_06(paQUARTER_BYTE_06);
        COutputGuard guard_paQUARTER_BYTE_07(paQUARTER_BYTE_07);
        COutputGuard guard_paQUARTER_BYTE_08(paQUARTER_BYTE_08);
        COutputGuard guard_paQUARTER_BYTE_09(paQUARTER_BYTE_09);
        COutputGuard guard_paQUARTER_BYTE_10(paQUARTER_BYTE_10);
        COutputGuard guard_paQUARTER_BYTE_11(paQUARTER_BYTE_11);
        COutputGuard guard_paQUARTER_BYTE_12(paQUARTER_BYTE_12);
        COutputGuard guard_paQUARTER_BYTE_13(paQUARTER_BYTE_13);
        COutputGuard guard_paQUARTER_BYTE_14(paQUARTER_BYTE_14);
        COutputGuard guard_paQUARTER_BYTE_15(paQUARTER_BYTE_15);
        COutputGuard guard_paQUARTER_BYTE_16(paQUARTER_BYTE_16);
        COutputGuard guard_paQUARTER_BYTE_17(paQUARTER_BYTE_17);
        COutputGuard guard_paQUARTER_BYTE_18(paQUARTER_BYTE_18);
        COutputGuard guard_paQUARTER_BYTE_19(paQUARTER_BYTE_19);
        COutputGuard guard_paQUARTER_BYTE_20(paQUARTER_BYTE_20);
        COutputGuard guard_paQUARTER_BYTE_21(paQUARTER_BYTE_21);
        COutputGuard guard_paQUARTER_BYTE_22(paQUARTER_BYTE_22);
        COutputGuard guard_paQUARTER_BYTE_23(paQUARTER_BYTE_23);
        COutputGuard guard_paQUARTER_BYTE_24(paQUARTER_BYTE_24);
        COutputGuard guard_paQUARTER_BYTE_25(paQUARTER_BYTE_25);
        COutputGuard guard_paQUARTER_BYTE_26(paQUARTER_BYTE_26);
        COutputGuard guard_paQUARTER_BYTE_27(paQUARTER_BYTE_27);
        COutputGuard guard_paQUARTER_BYTE_28(paQUARTER_BYTE_28);
        COutputGuard guard_paQUARTER_BYTE_29(paQUARTER_BYTE_29);
        COutputGuard guard_paQUARTER_BYTE_30(paQUARTER_BYTE_30);
        COutputGuard guard_paQUARTER_BYTE_31(paQUARTER_BYTE_31);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paQUARTER_BYTE_00 = var_QUARTER_BYTE_00;
        *paQUARTER_BYTE_01 = var_QUARTER_BYTE_01;
        *paQUARTER_BYTE_02 = var_QUARTER_BYTE_02;
        *paQUARTER_BYTE_03 = var_QUARTER_BYTE_03;
        *paQUARTER_BYTE_04 = var_QUARTER_BYTE_04;
        *paQUARTER_BYTE_05 = var_QUARTER_BYTE_05;
        *paQUARTER_BYTE_06 = var_QUARTER_BYTE_06;
        *paQUARTER_BYTE_07 = var_QUARTER_BYTE_07;
        *paQUARTER_BYTE_08 = var_QUARTER_BYTE_08;
        *paQUARTER_BYTE_09 = var_QUARTER_BYTE_09;
        *paQUARTER_BYTE_10 = var_QUARTER_BYTE_10;
        *paQUARTER_BYTE_11 = var_QUARTER_BYTE_11;
        *paQUARTER_BYTE_12 = var_QUARTER_BYTE_12;
        *paQUARTER_BYTE_13 = var_QUARTER_BYTE_13;
        *paQUARTER_BYTE_14 = var_QUARTER_BYTE_14;
        *paQUARTER_BYTE_15 = var_QUARTER_BYTE_15;
        *paQUARTER_BYTE_16 = var_QUARTER_BYTE_16;
        *paQUARTER_BYTE_17 = var_QUARTER_BYTE_17;
        *paQUARTER_BYTE_18 = var_QUARTER_BYTE_18;
        *paQUARTER_BYTE_19 = var_QUARTER_BYTE_19;
        *paQUARTER_BYTE_20 = var_QUARTER_BYTE_20;
        *paQUARTER_BYTE_21 = var_QUARTER_BYTE_21;
        *paQUARTER_BYTE_22 = var_QUARTER_BYTE_22;
        *paQUARTER_BYTE_23 = var_QUARTER_BYTE_23;
        *paQUARTER_BYTE_24 = var_QUARTER_BYTE_24;
        *paQUARTER_BYTE_25 = var_QUARTER_BYTE_25;
        *paQUARTER_BYTE_26 = var_QUARTER_BYTE_26;
        *paQUARTER_BYTE_27 = var_QUARTER_BYTE_27;
        *paQUARTER_BYTE_28 = var_QUARTER_BYTE_28;
        *paQUARTER_BYTE_29 = var_QUARTER_BYTE_29;
        *paQUARTER_BYTE_30 = var_QUARTER_BYTE_30;
        *paQUARTER_BYTE_31 = var_QUARTER_BYTE_31;
      }

      void operator()(const CIEC_LWORD &paIN,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_00,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_01,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_02,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_03,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_04,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_05,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_06,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_07,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_08,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_09,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_10,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_11,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_12,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_13,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_14,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_15,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_16,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_17,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_18,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_19,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_20,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_21,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_22,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_23,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_24,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_25,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_26,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_27,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_28,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_29,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_30,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_31) {
        evt_REQ(paIN, paQUARTER_BYTE_00, paQUARTER_BYTE_01, paQUARTER_BYTE_02, paQUARTER_BYTE_03, paQUARTER_BYTE_04,
                paQUARTER_BYTE_05, paQUARTER_BYTE_06, paQUARTER_BYTE_07, paQUARTER_BYTE_08, paQUARTER_BYTE_09,
                paQUARTER_BYTE_10, paQUARTER_BYTE_11, paQUARTER_BYTE_12, paQUARTER_BYTE_13, paQUARTER_BYTE_14,
                paQUARTER_BYTE_15, paQUARTER_BYTE_16, paQUARTER_BYTE_17, paQUARTER_BYTE_18, paQUARTER_BYTE_19,
                paQUARTER_BYTE_20, paQUARTER_BYTE_21, paQUARTER_BYTE_22, paQUARTER_BYTE_23, paQUARTER_BYTE_24,
                paQUARTER_BYTE_25, paQUARTER_BYTE_26, paQUARTER_BYTE_27, paQUARTER_BYTE_28, paQUARTER_BYTE_29,
                paQUARTER_BYTE_30, paQUARTER_BYTE_31);
      }
  };

  void func_SPLIT_LWORD_INTO_QUARTERS(CIEC_LWORD st_lv_IN,
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
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_15,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_16,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_17,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_18,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_19,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_20,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_21,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_22,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_23,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_24,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_25,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_26,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_27,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_28,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_29,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_30,
                                      CIEC_BYTE &st_lv_QUARTER_BYTE_31);
} // namespace forte::eclipse4diac::utils::splitting
