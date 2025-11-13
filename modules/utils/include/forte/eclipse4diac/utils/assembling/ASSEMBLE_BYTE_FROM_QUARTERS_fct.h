/*******************************************************************************
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
 *** Name: ASSEMBLE_BYTE_FROM_QUARTERS
 *** Description: this Function combines the 4 QUARTER BYTES to a BYTE
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_BYTE_FROM_QUARTERS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_BYTE_FROM_QUARTERS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_BYTE_FROM_QUARTERS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BYTE var_QUARTER_BYTE_00;
      CIEC_BYTE var_QUARTER_BYTE_01;
      CIEC_BYTE var_QUARTER_BYTE_02;
      CIEC_BYTE var_QUARTER_BYTE_03;

      CIEC_BYTE var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_QUARTER_BYTE_00;
      CDataConnection *conn_QUARTER_BYTE_01;
      CDataConnection *conn_QUARTER_BYTE_02;
      CDataConnection *conn_QUARTER_BYTE_03;

      COutDataConnection<CIEC_BYTE> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BYTE &paQUARTER_BYTE_00,
                   const CIEC_BYTE &paQUARTER_BYTE_01,
                   const CIEC_BYTE &paQUARTER_BYTE_02,
                   const CIEC_BYTE &paQUARTER_BYTE_03,
                   CAnyBitOutputParameter<CIEC_BYTE> pa) {
        COutputGuard guard_pa(pa);
        var_QUARTER_BYTE_00 = paQUARTER_BYTE_00;
        var_QUARTER_BYTE_01 = paQUARTER_BYTE_01;
        var_QUARTER_BYTE_02 = paQUARTER_BYTE_02;
        var_QUARTER_BYTE_03 = paQUARTER_BYTE_03;
        executeEvent(scmEventREQID, nullptr);
        *pa = var_;
      }

      void operator()(const CIEC_BYTE &paQUARTER_BYTE_00,
                      const CIEC_BYTE &paQUARTER_BYTE_01,
                      const CIEC_BYTE &paQUARTER_BYTE_02,
                      const CIEC_BYTE &paQUARTER_BYTE_03,
                      CAnyBitOutputParameter<CIEC_BYTE> &pa) {
        evt_REQ(paQUARTER_BYTE_00, paQUARTER_BYTE_01, paQUARTER_BYTE_02, paQUARTER_BYTE_03, pa);
      }
  };

  CIEC_BYTE func_ASSEMBLE_BYTE_FROM_QUARTERS(CIEC_BYTE st_lv_QUARTER_BYTE_00,
                                             CIEC_BYTE st_lv_QUARTER_BYTE_01,
                                             CIEC_BYTE st_lv_QUARTER_BYTE_02,
                                             CIEC_BYTE st_lv_QUARTER_BYTE_03);
} // namespace forte::eclipse4diac::utils::assembling
