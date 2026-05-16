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
 *** Name: ASSEMBLE_LWORD_FROM_WORDS
 *** Description: this Function combines the 4 WORDS to a LWORD
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_word.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_LWORD_FROM_WORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_WORDS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_LWORD_FROM_WORDS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WORD var_WORD_00;
      CIEC_WORD var_WORD_01;
      CIEC_WORD var_WORD_02;
      CIEC_WORD var_WORD_03;

      CIEC_LWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_WORD_00;
      CDataConnection *conn_WORD_01;
      CDataConnection *conn_WORD_02;
      CDataConnection *conn_WORD_03;

      COutDataConnection<CIEC_LWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_WORD &paWORD_00, const CIEC_WORD &paWORD_01, const CIEC_WORD &paWORD_02, const CIEC_WORD &paWORD_03) {
        var_WORD_00 = paWORD_00;
        var_WORD_01 = paWORD_01;
        var_WORD_02 = paWORD_02;
        var_WORD_03 = paWORD_03;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_WORD &paWORD_00, const CIEC_WORD &paWORD_01, const CIEC_WORD &paWORD_02, const CIEC_WORD &paWORD_03) {
        evt_REQ(std::forward<const CIEC_WORD &>(paWORD_00), std::forward<const CIEC_WORD &>(paWORD_01), std::forward<const CIEC_WORD &>(paWORD_02), std::forward<const CIEC_WORD &>(paWORD_03));
      }
  };

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_WORDS(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01, const CIEC_WORD &st_lv_WORD_02, const CIEC_WORD &st_lv_WORD_03);
}

