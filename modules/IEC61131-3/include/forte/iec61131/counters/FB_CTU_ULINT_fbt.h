/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::counters {
  class FORTE_FB_CTU_ULINT final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_CTU_ULINT)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FB_CTU_ULINT(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_CU;
      CIEC_BOOL var_R;
      CIEC_ULINT var_PV;

      CIEC_BOOL var_Q;
      CIEC_ULINT var_CV;

      CEventConnection conn_CNF;

      CDataConnection *conn_CU;
      CDataConnection *conn_R;
      CDataConnection *conn_PV;

      COutDataConnection<CIEC_BOOL> conn_Q;
      COutDataConnection<CIEC_ULINT> conn_CV;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paCU,
                   const CIEC_BOOL &paR,
                   const CIEC_ULINT &paPV,
                   CAnyBitOutputParameter<CIEC_BOOL> paQ,
                   COutputParameter<CIEC_ULINT> paCV) {
        COutputGuard guard_paQ(paQ);
        COutputGuard guard_paCV(paCV);
        var_CU = paCU;
        var_R = paR;
        var_PV = paPV;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
        *paCV = var_CV;
      }

      void operator()(const CIEC_BOOL &paCU,
                      const CIEC_BOOL &paR,
                      const CIEC_ULINT &paPV,
                      CAnyBitOutputParameter<CIEC_BOOL> paQ,
                      COutputParameter<CIEC_ULINT> paCV) {
        evt_REQ(paCU, paR, paPV, paQ, paCV);
      }
  };
} // namespace forte::iec61131::counters
