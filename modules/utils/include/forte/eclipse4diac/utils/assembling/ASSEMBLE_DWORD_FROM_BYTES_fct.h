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
 *** Name: ASSEMBLE_DWORD_FROM_BYTES
 *** Description: this Function combines the 4 BYTES to a DWORD
 *** Version:
 ***     3.0.2: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0.1: 2025-12-21/Franz Höpfinger - HR Agrartechnik - fix 2 --> 4
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_DWORD_FROM_BYTES final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_BYTES)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_DWORD_FROM_BYTES(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BYTE var_BYTE_00;
      CIEC_BYTE var_BYTE_01;
      CIEC_BYTE var_BYTE_02;
      CIEC_BYTE var_BYTE_03;

      CIEC_DWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_BYTE_00;
      CDataConnection *conn_BYTE_01;
      CDataConnection *conn_BYTE_02;
      CDataConnection *conn_BYTE_03;

      COutDataConnection<CIEC_DWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_DWORD func_ASSEMBLE_DWORD_FROM_BYTES(const CIEC_BYTE &st_lv_BYTE_00,
                                            const CIEC_BYTE &st_lv_BYTE_01,
                                            const CIEC_BYTE &st_lv_BYTE_02,
                                            const CIEC_BYTE &st_lv_BYTE_03);
} // namespace forte::eclipse4diac::utils::assembling
