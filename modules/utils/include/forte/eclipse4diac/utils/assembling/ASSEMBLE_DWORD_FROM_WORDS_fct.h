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
 *** Name: ASSEMBLE_DWORD_FROM_WORDS
 *** Description: this Function combines the 2 WORDS to a dword
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"

namespace forte::eclipse4diac::utils::assembling {
  class FORTE_ASSEMBLE_DWORD_FROM_WORDS final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_WORDS)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_ASSEMBLE_DWORD_FROM_WORDS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WORD var_WORD_00;
      CIEC_WORD var_WORD_01;

      CIEC_DWORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_WORD_00;
      CDataConnection *conn_WORD_01;

      COutDataConnection<CIEC_DWORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  CIEC_DWORD func_ASSEMBLE_DWORD_FROM_WORDS(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01);
} // namespace forte::eclipse4diac::utils::assembling
