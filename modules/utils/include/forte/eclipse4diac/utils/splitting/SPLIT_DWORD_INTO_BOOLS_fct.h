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

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_DWORD_INTO_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_BOOLS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_DWORD_INTO_BOOLS(StringId paInstanceNameId, CFBContainer &paContainer);

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
  };

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
                                   CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_31);
} // namespace forte::eclipse4diac::utils::splitting
