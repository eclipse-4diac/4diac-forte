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
 *** Name: SPLIT_WORD_INTO_BOOLS
 *** Description: this Function extracts the 16 BOOLS from a word
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_WORD_INTO_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_WORD_INTO_BOOLS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_WORD_INTO_BOOLS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WORD var_IN;

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

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_WORD &paIN,
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
                   CAnyBitOutputParameter<CIEC_BOOL> paBIT_15) {
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
      }

      void operator()(const CIEC_WORD &paIN,
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
                      CAnyBitOutputParameter<CIEC_BOOL> paBIT_15) {
        evt_REQ(paIN, paBIT_00, paBIT_01, paBIT_02, paBIT_03, paBIT_04, paBIT_05, paBIT_06, paBIT_07, paBIT_08,
                paBIT_09, paBIT_10, paBIT_11, paBIT_12, paBIT_13, paBIT_14, paBIT_15);
      }
  };

  void func_SPLIT_WORD_INTO_BOOLS(CIEC_WORD st_lv_IN,
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
} // namespace forte::eclipse4diac::utils::splitting
