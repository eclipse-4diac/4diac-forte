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
 *** Name: ASSEMBLE_LWORD_FROM_DWORDS
 *** Description: this Function combines the 2 DWORDS to a LWORD
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_LWORD_FROM_DWORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_DWORDS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_LWORD_FROM_DWORDS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DWORD var_DWORD_00;
      CIEC_DWORD var_DWORD_01;

      CIEC_LWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_DWORD_00;
      CDataConnection *conn_DWORD_01;

      COutDataConnection<CIEC_LWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_DWORD &paDWORD_00, const CIEC_DWORD &paDWORD_01) {
        var_DWORD_00 = paDWORD_00;
        var_DWORD_01 = paDWORD_01;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_DWORD &paDWORD_00, const CIEC_DWORD &paDWORD_01) {
        evt_REQ(std::forward<const CIEC_DWORD &>(paDWORD_00), std::forward<const CIEC_DWORD &>(paDWORD_01));
      }
  };

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_DWORDS(const CIEC_DWORD &st_lv_DWORD_00, const CIEC_DWORD &st_lv_DWORD_01);
}

