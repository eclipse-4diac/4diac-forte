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
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SPLIT_DWORD_INTO_WORDS
 *** Description: this Function extracts the 2 WORD from a dword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  class FORTE_SPLIT_DWORD_INTO_WORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_WORDS)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_SPLIT_DWORD_INTO_WORDS(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DWORD var_IN;

      CIEC_WORD var_WORD_00;
      CIEC_WORD var_WORD_01;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;

      COutDataConnection<CIEC_WORD> conn_WORD_00;
      COutDataConnection<CIEC_WORD> conn_WORD_01;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_DWORD &paIN,
                   CAnyBitOutputParameter<CIEC_WORD> paWORD_00,
                   CAnyBitOutputParameter<CIEC_WORD> paWORD_01) {
        COutputGuard guard_paWORD_00(paWORD_00);
        COutputGuard guard_paWORD_01(paWORD_01);
        var_IN = paIN;
        executeEvent(scmEventREQID, nullptr);
        *paWORD_00 = var_WORD_00;
        *paWORD_01 = var_WORD_01;
      }

      void operator()(const CIEC_DWORD &paIN,
                      CAnyBitOutputParameter<CIEC_WORD> paWORD_00,
                      CAnyBitOutputParameter<CIEC_WORD> paWORD_01) {
        evt_REQ(paIN, paWORD_00, paWORD_01);
      }
  };

  void func_SPLIT_DWORD_INTO_WORDS(CIEC_DWORD st_lv_IN, CIEC_WORD &st_lv_WORD_00, CIEC_WORD &st_lv_WORD_01);
} // namespace forte::eclipse4diac::utils::splitting
