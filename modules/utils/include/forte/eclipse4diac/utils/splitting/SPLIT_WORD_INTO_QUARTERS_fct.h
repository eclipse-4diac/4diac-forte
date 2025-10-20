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
 *** Name: SPLIT_WORD_INTO_QUARTERS
 *** Description: this Function extracts the 8 QUARTER BYTE from a word
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_WORD_INTO_QUARTERS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_WORD_INTO_QUARTERS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_WORD_INTO_QUARTERS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WORD var_IN;

      CIEC_BYTE var_QUARTER_BYTE_00;
      CIEC_BYTE var_QUARTER_BYTE_01;
      CIEC_BYTE var_QUARTER_BYTE_02;
      CIEC_BYTE var_QUARTER_BYTE_03;
      CIEC_BYTE var_QUARTER_BYTE_04;
      CIEC_BYTE var_QUARTER_BYTE_05;
      CIEC_BYTE var_QUARTER_BYTE_06;
      CIEC_BYTE var_QUARTER_BYTE_07;

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

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_WORD &paIN,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_00,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_01,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_02,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_03,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_04,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_05,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_06,
                   CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_07) {
        COutputGuard guard_paQUARTER_BYTE_00(paQUARTER_BYTE_00);
        COutputGuard guard_paQUARTER_BYTE_01(paQUARTER_BYTE_01);
        COutputGuard guard_paQUARTER_BYTE_02(paQUARTER_BYTE_02);
        COutputGuard guard_paQUARTER_BYTE_03(paQUARTER_BYTE_03);
        COutputGuard guard_paQUARTER_BYTE_04(paQUARTER_BYTE_04);
        COutputGuard guard_paQUARTER_BYTE_05(paQUARTER_BYTE_05);
        COutputGuard guard_paQUARTER_BYTE_06(paQUARTER_BYTE_06);
        COutputGuard guard_paQUARTER_BYTE_07(paQUARTER_BYTE_07);
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
      }

      void operator()(const CIEC_WORD &paIN,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_00,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_01,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_02,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_03,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_04,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_05,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_06,
                      CAnyBitOutputParameter<CIEC_BYTE> paQUARTER_BYTE_07) {
        evt_REQ(paIN, paQUARTER_BYTE_00, paQUARTER_BYTE_01, paQUARTER_BYTE_02, paQUARTER_BYTE_03, paQUARTER_BYTE_04,
                paQUARTER_BYTE_05, paQUARTER_BYTE_06, paQUARTER_BYTE_07);
      }
  };

  void func_SPLIT_WORD_INTO_QUARTERS(CIEC_WORD st_lv_IN,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_03,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_04,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_05,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_06,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_07);
} // namespace forte::eclipse4diac::utils::splitting
