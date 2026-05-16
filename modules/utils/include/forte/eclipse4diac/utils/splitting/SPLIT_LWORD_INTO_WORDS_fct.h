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
 *** Name: SPLIT_LWORD_INTO_WORDS
 *** Description: this Function extracts the 4 WORDS from a lword
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_LWORD_INTO_WORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_WORDS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_LWORD_INTO_WORDS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_LWORD var_IN;

      CIEC_WORD var_WORD_00;
      CIEC_WORD var_WORD_01;
      CIEC_WORD var_WORD_02;
      CIEC_WORD var_WORD_03;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_WORD> conn_WORD_00;
      COutDataConnection<CIEC_WORD> conn_WORD_01;
      COutDataConnection<CIEC_WORD> conn_WORD_02;
      COutDataConnection<CIEC_WORD> conn_WORD_03;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_LWORD &paIN, CAnyBitOutputParameter<CIEC_WORD> paWORD_00, CAnyBitOutputParameter<CIEC_WORD> paWORD_01, CAnyBitOutputParameter<CIEC_WORD> paWORD_02, CAnyBitOutputParameter<CIEC_WORD> paWORD_03) {
        COutputGuard guard_WORD_00(paWORD_00);
        COutputGuard guard_WORD_01(paWORD_01);
        COutputGuard guard_WORD_02(paWORD_02);
        COutputGuard guard_WORD_03(paWORD_03);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paWORD_00 = var_WORD_00;
        *paWORD_01 = var_WORD_01;
        *paWORD_02 = var_WORD_02;
        *paWORD_03 = var_WORD_03;
      }

      void operator()(const CIEC_LWORD &paIN, CAnyBitOutputParameter<CIEC_WORD> paWORD_00, CAnyBitOutputParameter<CIEC_WORD> paWORD_01, CAnyBitOutputParameter<CIEC_WORD> paWORD_02, CAnyBitOutputParameter<CIEC_WORD> paWORD_03) {
        evt_REQ(std::forward<const CIEC_LWORD &>(paIN), std::forward<CAnyBitOutputParameter<CIEC_WORD>>(paWORD_00), std::forward<CAnyBitOutputParameter<CIEC_WORD>>(paWORD_01), std::forward<CAnyBitOutputParameter<CIEC_WORD>>(paWORD_02), std::forward<CAnyBitOutputParameter<CIEC_WORD>>(paWORD_03));
      }
  };

  void func_SPLIT_LWORD_INTO_WORDS(const CIEC_LWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_00, CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_01, CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_02, CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_03);
}

