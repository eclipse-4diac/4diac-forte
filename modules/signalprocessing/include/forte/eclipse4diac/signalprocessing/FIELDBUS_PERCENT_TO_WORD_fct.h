/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_PERCENT_TO_WORD
 *** Description: Convert a WORD Value Range 0-FAFF to a REAL in the Range 0.0 to 100.0
 *** Version:
 ***     1.0: 2023-10-21/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Update to a more Function like Interface
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_FIELDBUS_PERCENT_TO_WORD final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_FIELDBUS_PERCENT_TO_WORD)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FIELDBUS_PERCENT_TO_WORD(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_REAL var_RI;

      CIEC_WORD var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_RI;

      COutDataConnection<CIEC_WORD> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_REAL &paRI, CAnyBitOutputParameter<CIEC_WORD> pa) {
        COutputGuard guard_pa(pa);
        var_RI = paRI;
        executeEvent(scmEventREQID, nullptr);
        *pa = var_;
      }

      void operator()(const CIEC_REAL &paRI, CAnyBitOutputParameter<CIEC_WORD> &pa) {
        evt_REQ(paRI, pa);
      }
  };

  CIEC_WORD func_FIELDBUS_PERCENT_TO_WORD(CIEC_REAL st_lv_RI);
} // namespace forte::eclipse4diac::signalprocessing
