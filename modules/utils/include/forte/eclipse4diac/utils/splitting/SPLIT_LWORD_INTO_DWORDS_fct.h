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
 *** Name: SPLIT_LWORD_INTO_DWORDS
 *** Description: this Function extracts the 2 DWORDS from a lword
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_LWORD_INTO_DWORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_DWORDS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_LWORD_INTO_DWORDS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_LWORD var_IN;

      CIEC_DWORD var_DWORD_00;
      CIEC_DWORD var_DWORD_01;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_DWORD> conn_DWORD_00;
      COutDataConnection<CIEC_DWORD> conn_DWORD_01;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_LWORD &paIN, CAnyBitOutputParameter<CIEC_DWORD> paDWORD_00, CAnyBitOutputParameter<CIEC_DWORD> paDWORD_01) {
        COutputGuard guard_DWORD_00(paDWORD_00);
        COutputGuard guard_DWORD_01(paDWORD_01);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paDWORD_00 = var_DWORD_00;
        *paDWORD_01 = var_DWORD_01;
      }

      void operator()(const CIEC_LWORD &paIN, CAnyBitOutputParameter<CIEC_DWORD> paDWORD_00, CAnyBitOutputParameter<CIEC_DWORD> paDWORD_01) {
        evt_REQ(std::forward<const CIEC_LWORD &>(paIN), std::forward<CAnyBitOutputParameter<CIEC_DWORD>>(paDWORD_00), std::forward<CAnyBitOutputParameter<CIEC_DWORD>>(paDWORD_01));
      }
  };

  void func_SPLIT_LWORD_INTO_DWORDS(const CIEC_LWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_DWORD> st_lv_DWORD_00, CAnyBitOutputParameter<CIEC_DWORD> st_lv_DWORD_01);
}

