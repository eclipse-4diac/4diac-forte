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
 *** Name: ASSEMBLE_BYTE_FROM_BOOLS
 *** Description: this Function combines the 8 BOOLS to a BYTE
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_BYTE_FROM_BOOLS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_BYTE_FROM_BOOLS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_BYTE_FROM_BOOLS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_BIT_00;
      CIEC_BOOL var_BIT_01;
      CIEC_BOOL var_BIT_02;
      CIEC_BOOL var_BIT_03;
      CIEC_BOOL var_BIT_04;
      CIEC_BOOL var_BIT_05;
      CIEC_BOOL var_BIT_06;
      CIEC_BOOL var_BIT_07;

      CIEC_BYTE var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_BIT_00;
      CDataConnection *conn_BIT_01;
      CDataConnection *conn_BIT_02;
      CDataConnection *conn_BIT_03;
      CDataConnection *conn_BIT_04;
      CDataConnection *conn_BIT_05;
      CDataConnection *conn_BIT_06;
      CDataConnection *conn_BIT_07;

      COutDataConnection<CIEC_BYTE> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_BYTE func_ASSEMBLE_BYTE_FROM_BOOLS(const CIEC_BOOL &st_lv_BIT_00,
                                          const CIEC_BOOL &st_lv_BIT_01,
                                          const CIEC_BOOL &st_lv_BIT_02,
                                          const CIEC_BOOL &st_lv_BIT_03,
                                          const CIEC_BOOL &st_lv_BIT_04,
                                          const CIEC_BOOL &st_lv_BIT_05,
                                          const CIEC_BOOL &st_lv_BIT_06,
                                          const CIEC_BOOL &st_lv_BIT_07);
} // namespace forte::eclipse4diac::utils::assembling
