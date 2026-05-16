/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202605112003!
 ***
 *** Name: ASSEMBLE_LWORD_FROM_BYTES
 *** Description: this Function combines the 8 BYTES to a LWORD
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_lword.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_LWORD_FROM_BYTES final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_BYTES)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_LWORD_FROM_BYTES(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BYTE var_BYTE_00;
      CIEC_BYTE var_BYTE_01;
      CIEC_BYTE var_BYTE_02;
      CIEC_BYTE var_BYTE_03;
      CIEC_BYTE var_BYTE_04;
      CIEC_BYTE var_BYTE_05;
      CIEC_BYTE var_BYTE_06;
      CIEC_BYTE var_BYTE_07;

      CIEC_LWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_BYTE_00;
      CDataConnection *conn_BYTE_01;
      CDataConnection *conn_BYTE_02;
      CDataConnection *conn_BYTE_03;
      CDataConnection *conn_BYTE_04;
      CDataConnection *conn_BYTE_05;
      CDataConnection *conn_BYTE_06;
      CDataConnection *conn_BYTE_07;

      COutDataConnection<CIEC_LWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BYTE &paBYTE_00, const CIEC_BYTE &paBYTE_01, const CIEC_BYTE &paBYTE_02, const CIEC_BYTE &paBYTE_03, const CIEC_BYTE &paBYTE_04, const CIEC_BYTE &paBYTE_05, const CIEC_BYTE &paBYTE_06, const CIEC_BYTE &paBYTE_07) {
        var_BYTE_00 = paBYTE_00;
        var_BYTE_01 = paBYTE_01;
        var_BYTE_02 = paBYTE_02;
        var_BYTE_03 = paBYTE_03;
        var_BYTE_04 = paBYTE_04;
        var_BYTE_05 = paBYTE_05;
        var_BYTE_06 = paBYTE_06;
        var_BYTE_07 = paBYTE_07;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_BYTE &paBYTE_00, const CIEC_BYTE &paBYTE_01, const CIEC_BYTE &paBYTE_02, const CIEC_BYTE &paBYTE_03, const CIEC_BYTE &paBYTE_04, const CIEC_BYTE &paBYTE_05, const CIEC_BYTE &paBYTE_06, const CIEC_BYTE &paBYTE_07) {
        evt_REQ(std::forward<const CIEC_BYTE &>(paBYTE_00), std::forward<const CIEC_BYTE &>(paBYTE_01), std::forward<const CIEC_BYTE &>(paBYTE_02), std::forward<const CIEC_BYTE &>(paBYTE_03), std::forward<const CIEC_BYTE &>(paBYTE_04), std::forward<const CIEC_BYTE &>(paBYTE_05), std::forward<const CIEC_BYTE &>(paBYTE_06), std::forward<const CIEC_BYTE &>(paBYTE_07));
      }
  };

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_BYTES(const CIEC_BYTE &st_lv_BYTE_00, const CIEC_BYTE &st_lv_BYTE_01, const CIEC_BYTE &st_lv_BYTE_02, const CIEC_BYTE &st_lv_BYTE_03, const CIEC_BYTE &st_lv_BYTE_04, const CIEC_BYTE &st_lv_BYTE_05, const CIEC_BYTE &st_lv_BYTE_06, const CIEC_BYTE &st_lv_BYTE_07);
}

