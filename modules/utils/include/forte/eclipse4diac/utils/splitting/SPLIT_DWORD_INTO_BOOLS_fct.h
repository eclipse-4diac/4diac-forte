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
 *** Name: SPLIT_DWORD_INTO_BOOLS
 *** Description: this Function extracts the 32 BOOLS from a dword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_DWORD_INTO_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_BOOLS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_DWORD_INTO_BOOLS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DWORD var_IN;

      CIEC_BOOL var_BIT_00;
      CIEC_BOOL var_BIT_01;
      CIEC_BOOL var_BIT_02;
      CIEC_BOOL var_BIT_03;
      CIEC_BOOL var_BIT_04;
      CIEC_BOOL var_BIT_05;
      CIEC_BOOL var_BIT_06;
      CIEC_BOOL var_BIT_07;
      CIEC_BOOL var_BIT_08;
      CIEC_BOOL var_BIT_09;
      CIEC_BOOL var_BIT_10;
      CIEC_BOOL var_BIT_11;
      CIEC_BOOL var_BIT_12;
      CIEC_BOOL var_BIT_13;
      CIEC_BOOL var_BIT_14;
      CIEC_BOOL var_BIT_15;
      CIEC_BOOL var_BIT_16;
      CIEC_BOOL var_BIT_17;
      CIEC_BOOL var_BIT_18;
      CIEC_BOOL var_BIT_19;
      CIEC_BOOL var_BIT_20;
      CIEC_BOOL var_BIT_21;
      CIEC_BOOL var_BIT_22;
      CIEC_BOOL var_BIT_23;
      CIEC_BOOL var_BIT_24;
      CIEC_BOOL var_BIT_25;
      CIEC_BOOL var_BIT_26;
      CIEC_BOOL var_BIT_27;
      CIEC_BOOL var_BIT_28;
      CIEC_BOOL var_BIT_29;
      CIEC_BOOL var_BIT_30;
      CIEC_BOOL var_BIT_31;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_BOOL> conn_BIT_00;
      COutDataConnection<CIEC_BOOL> conn_BIT_01;
      COutDataConnection<CIEC_BOOL> conn_BIT_02;
      COutDataConnection<CIEC_BOOL> conn_BIT_03;
      COutDataConnection<CIEC_BOOL> conn_BIT_04;
      COutDataConnection<CIEC_BOOL> conn_BIT_05;
      COutDataConnection<CIEC_BOOL> conn_BIT_06;
      COutDataConnection<CIEC_BOOL> conn_BIT_07;
      COutDataConnection<CIEC_BOOL> conn_BIT_08;
      COutDataConnection<CIEC_BOOL> conn_BIT_09;
      COutDataConnection<CIEC_BOOL> conn_BIT_10;
      COutDataConnection<CIEC_BOOL> conn_BIT_11;
      COutDataConnection<CIEC_BOOL> conn_BIT_12;
      COutDataConnection<CIEC_BOOL> conn_BIT_13;
      COutDataConnection<CIEC_BOOL> conn_BIT_14;
      COutDataConnection<CIEC_BOOL> conn_BIT_15;
      COutDataConnection<CIEC_BOOL> conn_BIT_16;
      COutDataConnection<CIEC_BOOL> conn_BIT_17;
      COutDataConnection<CIEC_BOOL> conn_BIT_18;
      COutDataConnection<CIEC_BOOL> conn_BIT_19;
      COutDataConnection<CIEC_BOOL> conn_BIT_20;
      COutDataConnection<CIEC_BOOL> conn_BIT_21;
      COutDataConnection<CIEC_BOOL> conn_BIT_22;
      COutDataConnection<CIEC_BOOL> conn_BIT_23;
      COutDataConnection<CIEC_BOOL> conn_BIT_24;
      COutDataConnection<CIEC_BOOL> conn_BIT_25;
      COutDataConnection<CIEC_BOOL> conn_BIT_26;
      COutDataConnection<CIEC_BOOL> conn_BIT_27;
      COutDataConnection<CIEC_BOOL> conn_BIT_28;
      COutDataConnection<CIEC_BOOL> conn_BIT_29;
      COutDataConnection<CIEC_BOOL> conn_BIT_30;
      COutDataConnection<CIEC_BOOL> conn_BIT_31;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_DWORD &paIN,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_00,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_01,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_02,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_03,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_04,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_05,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_06,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_07,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_08,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_09,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_10,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_11,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_12,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_13,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_14,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_15,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_16,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_17,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_18,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_19,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_20,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_21,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_22,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_23,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_24,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_25,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_26,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_27,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_28,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_29,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_30,
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_31) {
        COutputGuard guard_paBIT_00(paBIT_00);
        COutputGuard guard_paBIT_01(paBIT_01);
        COutputGuard guard_paBIT_02(paBIT_02);
        COutputGuard guard_paBIT_03(paBIT_03);
        COutputGuard guard_paBIT_04(paBIT_04);
        COutputGuard guard_paBIT_05(paBIT_05);
        COutputGuard guard_paBIT_06(paBIT_06);
        COutputGuard guard_paBIT_07(paBIT_07);
        COutputGuard guard_paBIT_08(paBIT_08);
        COutputGuard guard_paBIT_09(paBIT_09);
        COutputGuard guard_paBIT_10(paBIT_10);
        COutputGuard guard_paBIT_11(paBIT_11);
        COutputGuard guard_paBIT_12(paBIT_12);
        COutputGuard guard_paBIT_13(paBIT_13);
        COutputGuard guard_paBIT_14(paBIT_14);
        COutputGuard guard_paBIT_15(paBIT_15);
        COutputGuard guard_paBIT_16(paBIT_16);
        COutputGuard guard_paBIT_17(paBIT_17);
        COutputGuard guard_paBIT_18(paBIT_18);
        COutputGuard guard_paBIT_19(paBIT_19);
        COutputGuard guard_paBIT_20(paBIT_20);
        COutputGuard guard_paBIT_21(paBIT_21);
        COutputGuard guard_paBIT_22(paBIT_22);
        COutputGuard guard_paBIT_23(paBIT_23);
        COutputGuard guard_paBIT_24(paBIT_24);
        COutputGuard guard_paBIT_25(paBIT_25);
        COutputGuard guard_paBIT_26(paBIT_26);
        COutputGuard guard_paBIT_27(paBIT_27);
        COutputGuard guard_paBIT_28(paBIT_28);
        COutputGuard guard_paBIT_29(paBIT_29);
        COutputGuard guard_paBIT_30(paBIT_30);
        COutputGuard guard_paBIT_31(paBIT_31);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paBIT_00 = var_BIT_00;
        *paBIT_01 = var_BIT_01;
        *paBIT_02 = var_BIT_02;
        *paBIT_03 = var_BIT_03;
        *paBIT_04 = var_BIT_04;
        *paBIT_05 = var_BIT_05;
        *paBIT_06 = var_BIT_06;
        *paBIT_07 = var_BIT_07;
        *paBIT_08 = var_BIT_08;
        *paBIT_09 = var_BIT_09;
        *paBIT_10 = var_BIT_10;
        *paBIT_11 = var_BIT_11;
        *paBIT_12 = var_BIT_12;
        *paBIT_13 = var_BIT_13;
        *paBIT_14 = var_BIT_14;
        *paBIT_15 = var_BIT_15;
        *paBIT_16 = var_BIT_16;
        *paBIT_17 = var_BIT_17;
        *paBIT_18 = var_BIT_18;
        *paBIT_19 = var_BIT_19;
        *paBIT_20 = var_BIT_20;
        *paBIT_21 = var_BIT_21;
        *paBIT_22 = var_BIT_22;
        *paBIT_23 = var_BIT_23;
        *paBIT_24 = var_BIT_24;
        *paBIT_25 = var_BIT_25;
        *paBIT_26 = var_BIT_26;
        *paBIT_27 = var_BIT_27;
        *paBIT_28 = var_BIT_28;
        *paBIT_29 = var_BIT_29;
        *paBIT_30 = var_BIT_30;
        *paBIT_31 = var_BIT_31;
      }

      void operator()(const CIEC_DWORD &paIN,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_00,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_01,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_02,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_03,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_04,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_05,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_06,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_07,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_08,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_09,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_10,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_11,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_12,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_13,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_14,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_15,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_16,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_17,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_18,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_19,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_20,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_21,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_22,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_23,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_24,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_25,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_26,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_27,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_28,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_29,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_30,
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_31) {
        evt_REQ(paIN, paBIT_00, paBIT_01, paBIT_02, paBIT_03, paBIT_04, paBIT_05, paBIT_06, paBIT_07, paBIT_08,
                paBIT_09, paBIT_10, paBIT_11, paBIT_12, paBIT_13, paBIT_14, paBIT_15, paBIT_16, paBIT_17, paBIT_18,
                paBIT_19, paBIT_20, paBIT_21, paBIT_22, paBIT_23, paBIT_24, paBIT_25, paBIT_26, paBIT_27, paBIT_28,
                paBIT_29, paBIT_30, paBIT_31);
      }
  };

  void func_SPLIT_DWORD_INTO_BOOLS(CIEC_DWORD st_lv_IN,
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
} // namespace forte::eclipse4diac::utils::splitting
