/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::timers {
  class FORTE_FB_TOF final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_FB_TOF)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      bool fallingEdge;
      bool notFirstRisingEdge;
      CIEC_TIME start;
      CIEC_TIME count;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FB_TOF(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_IN;
      CIEC_TIME var_PT;

      CIEC_BOOL var_Q;
      CIEC_TIME var_ET;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;
      CDataConnection *conn_PT;

      COutDataConnection<CIEC_BOOL> conn_Q;
      COutDataConnection<CIEC_TIME> conn_ET;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paIN,
                   const CIEC_TIME &paPT,
                   CAnyBitOutputParameter<CIEC_BOOL> paQ,
                   COutputParameter<CIEC_TIME> paET) {
        COutputGuard guard_paQ(paQ);
        COutputGuard guard_paET(paET);
        var_IN = paIN;
        var_PT = paPT;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
        *paET = var_ET;
      }

      void operator()(const CIEC_BOOL &paIN,
                      const CIEC_TIME &paPT,
                      CAnyBitOutputParameter<CIEC_BOOL> paQ,
                      COutputParameter<CIEC_TIME> paET) {
        evt_REQ(paIN, paPT, paQ, paET);
      }
  };
} // namespace forte::iec61131::timers
