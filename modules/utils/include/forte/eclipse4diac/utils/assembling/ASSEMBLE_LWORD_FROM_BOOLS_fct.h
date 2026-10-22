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
 *** Name: ASSEMBLE_LWORD_FROM_BOOLS
 *** Description: this Function combines the 64 BOOLS to a LWORD
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_lword.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_LWORD_FROM_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_BOOLS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_LWORD_FROM_BOOLS(StringId paInstanceNameId, CFBContainer &paContainer);

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
      CIEC_BOOL var_BIT_32;
      CIEC_BOOL var_BIT_33;
      CIEC_BOOL var_BIT_34;
      CIEC_BOOL var_BIT_35;
      CIEC_BOOL var_BIT_36;
      CIEC_BOOL var_BIT_37;
      CIEC_BOOL var_BIT_38;
      CIEC_BOOL var_BIT_39;
      CIEC_BOOL var_BIT_40;
      CIEC_BOOL var_BIT_41;
      CIEC_BOOL var_BIT_42;
      CIEC_BOOL var_BIT_43;
      CIEC_BOOL var_BIT_44;
      CIEC_BOOL var_BIT_45;
      CIEC_BOOL var_BIT_46;
      CIEC_BOOL var_BIT_47;
      CIEC_BOOL var_BIT_48;
      CIEC_BOOL var_BIT_49;
      CIEC_BOOL var_BIT_50;
      CIEC_BOOL var_BIT_51;
      CIEC_BOOL var_BIT_52;
      CIEC_BOOL var_BIT_53;
      CIEC_BOOL var_BIT_54;
      CIEC_BOOL var_BIT_55;
      CIEC_BOOL var_BIT_56;
      CIEC_BOOL var_BIT_57;
      CIEC_BOOL var_BIT_58;
      CIEC_BOOL var_BIT_59;
      CIEC_BOOL var_BIT_60;
      CIEC_BOOL var_BIT_61;
      CIEC_BOOL var_BIT_62;
      CIEC_BOOL var_BIT_63;

      CIEC_LWORD var_;

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
      CDataConnection *conn_BIT_32;
      CDataConnection *conn_BIT_33;
      CDataConnection *conn_BIT_34;
      CDataConnection *conn_BIT_35;
      CDataConnection *conn_BIT_36;
      CDataConnection *conn_BIT_37;
      CDataConnection *conn_BIT_38;
      CDataConnection *conn_BIT_39;
      CDataConnection *conn_BIT_40;
      CDataConnection *conn_BIT_41;
      CDataConnection *conn_BIT_42;
      CDataConnection *conn_BIT_43;
      CDataConnection *conn_BIT_44;
      CDataConnection *conn_BIT_45;
      CDataConnection *conn_BIT_46;
      CDataConnection *conn_BIT_47;
      CDataConnection *conn_BIT_48;
      CDataConnection *conn_BIT_49;
      CDataConnection *conn_BIT_50;
      CDataConnection *conn_BIT_51;
      CDataConnection *conn_BIT_52;
      CDataConnection *conn_BIT_53;
      CDataConnection *conn_BIT_54;
      CDataConnection *conn_BIT_55;
      CDataConnection *conn_BIT_56;
      CDataConnection *conn_BIT_57;
      CDataConnection *conn_BIT_58;
      CDataConnection *conn_BIT_59;
      CDataConnection *conn_BIT_60;
      CDataConnection *conn_BIT_61;
      CDataConnection *conn_BIT_62;
      CDataConnection *conn_BIT_63;

      COutDataConnection<CIEC_LWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_BOOLS(const CIEC_BOOL &st_lv_BIT_00,
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
                                            const CIEC_BOOL &st_lv_BIT_31,
                                            const CIEC_BOOL &st_lv_BIT_32,
                                            const CIEC_BOOL &st_lv_BIT_33,
                                            const CIEC_BOOL &st_lv_BIT_34,
                                            const CIEC_BOOL &st_lv_BIT_35,
                                            const CIEC_BOOL &st_lv_BIT_36,
                                            const CIEC_BOOL &st_lv_BIT_37,
                                            const CIEC_BOOL &st_lv_BIT_38,
                                            const CIEC_BOOL &st_lv_BIT_39,
                                            const CIEC_BOOL &st_lv_BIT_40,
                                            const CIEC_BOOL &st_lv_BIT_41,
                                            const CIEC_BOOL &st_lv_BIT_42,
                                            const CIEC_BOOL &st_lv_BIT_43,
                                            const CIEC_BOOL &st_lv_BIT_44,
                                            const CIEC_BOOL &st_lv_BIT_45,
                                            const CIEC_BOOL &st_lv_BIT_46,
                                            const CIEC_BOOL &st_lv_BIT_47,
                                            const CIEC_BOOL &st_lv_BIT_48,
                                            const CIEC_BOOL &st_lv_BIT_49,
                                            const CIEC_BOOL &st_lv_BIT_50,
                                            const CIEC_BOOL &st_lv_BIT_51,
                                            const CIEC_BOOL &st_lv_BIT_52,
                                            const CIEC_BOOL &st_lv_BIT_53,
                                            const CIEC_BOOL &st_lv_BIT_54,
                                            const CIEC_BOOL &st_lv_BIT_55,
                                            const CIEC_BOOL &st_lv_BIT_56,
                                            const CIEC_BOOL &st_lv_BIT_57,
                                            const CIEC_BOOL &st_lv_BIT_58,
                                            const CIEC_BOOL &st_lv_BIT_59,
                                            const CIEC_BOOL &st_lv_BIT_60,
                                            const CIEC_BOOL &st_lv_BIT_61,
                                            const CIEC_BOOL &st_lv_BIT_62,
                                            const CIEC_BOOL &st_lv_BIT_63);
} // namespace forte::eclipse4diac::utils::assembling
