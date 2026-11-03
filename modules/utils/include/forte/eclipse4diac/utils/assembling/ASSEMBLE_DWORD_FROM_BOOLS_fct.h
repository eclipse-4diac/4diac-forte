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
 *** Name: ASSEMBLE_DWORD_FROM_BOOLS
 *** Description: this Function combines the 32 BOOLS to a DWORD
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dword.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_DWORD_FROM_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_BOOLS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_DWORD_FROM_BOOLS(StringId paInstanceNameId, CFBContainer &paContainer);

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

      CIEC_DWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_BIT_00;
      CDataConnection *conn_BIT_01;
      CDataConnection *conn_BIT_02;
      CDataConnection *conn_BIT_03;
      CDataConnection *conn_BIT_04;
      CDataConnection *conn_BIT_05;
      CDataConnection *conn_BIT_06;
      CDataConnection *conn_BIT_07;
      CDataConnection *conn_BIT_08;
      CDataConnection *conn_BIT_09;
      CDataConnection *conn_BIT_10;
      CDataConnection *conn_BIT_11;
      CDataConnection *conn_BIT_12;
      CDataConnection *conn_BIT_13;
      CDataConnection *conn_BIT_14;
      CDataConnection *conn_BIT_15;
      CDataConnection *conn_BIT_16;
      CDataConnection *conn_BIT_17;
      CDataConnection *conn_BIT_18;
      CDataConnection *conn_BIT_19;
      CDataConnection *conn_BIT_20;
      CDataConnection *conn_BIT_21;
      CDataConnection *conn_BIT_22;
      CDataConnection *conn_BIT_23;
      CDataConnection *conn_BIT_24;
      CDataConnection *conn_BIT_25;
      CDataConnection *conn_BIT_26;
      CDataConnection *conn_BIT_27;
      CDataConnection *conn_BIT_28;
      CDataConnection *conn_BIT_29;
      CDataConnection *conn_BIT_30;
      CDataConnection *conn_BIT_31;

      COutDataConnection<CIEC_DWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_DWORD func_ASSEMBLE_DWORD_FROM_BOOLS(const CIEC_BOOL &st_lv_BIT_00,
                                            const CIEC_BOOL &st_lv_BIT_01,
                                            const CIEC_BOOL &st_lv_BIT_02,
                                            const CIEC_BOOL &st_lv_BIT_03,
                                            const CIEC_BOOL &st_lv_BIT_04,
                                            const CIEC_BOOL &st_lv_BIT_05,
                                            const CIEC_BOOL &st_lv_BIT_06,
                                            const CIEC_BOOL &st_lv_BIT_07,
                                            const CIEC_BOOL &st_lv_BIT_08,
                                            const CIEC_BOOL &st_lv_BIT_09,
                                            const CIEC_BOOL &st_lv_BIT_10,
                                            const CIEC_BOOL &st_lv_BIT_11,
                                            const CIEC_BOOL &st_lv_BIT_12,
                                            const CIEC_BOOL &st_lv_BIT_13,
                                            const CIEC_BOOL &st_lv_BIT_14,
                                            const CIEC_BOOL &st_lv_BIT_15,
                                            const CIEC_BOOL &st_lv_BIT_16,
                                            const CIEC_BOOL &st_lv_BIT_17,
                                            const CIEC_BOOL &st_lv_BIT_18,
                                            const CIEC_BOOL &st_lv_BIT_19,
                                            const CIEC_BOOL &st_lv_BIT_20,
                                            const CIEC_BOOL &st_lv_BIT_21,
                                            const CIEC_BOOL &st_lv_BIT_22,
                                            const CIEC_BOOL &st_lv_BIT_23,
                                            const CIEC_BOOL &st_lv_BIT_24,
                                            const CIEC_BOOL &st_lv_BIT_25,
                                            const CIEC_BOOL &st_lv_BIT_26,
                                            const CIEC_BOOL &st_lv_BIT_27,
                                            const CIEC_BOOL &st_lv_BIT_28,
                                            const CIEC_BOOL &st_lv_BIT_29,
                                            const CIEC_BOOL &st_lv_BIT_30,
                                            const CIEC_BOOL &st_lv_BIT_31);
} // namespace forte::eclipse4diac::utils::assembling
