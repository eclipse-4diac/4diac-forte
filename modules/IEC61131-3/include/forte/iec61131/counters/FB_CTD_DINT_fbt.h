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
#include "forte/datatypes/forte_dint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::counters {
  class FORTE_FB_CTD_DINT final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_CTD_DINT)

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
      FORTE_FB_CTD_DINT(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_CD;
      CIEC_BOOL var_LD;
      CIEC_DINT var_PV;

      CIEC_BOOL var_Q;
      CIEC_DINT var_CV;

      CEventConnection conn_CNF;

      CDataConnection *conn_CD;
      CDataConnection *conn_LD;
      CDataConnection *conn_PV;

      COutDataConnection<CIEC_BOOL> conn_Q;
      COutDataConnection<CIEC_DINT> conn_CV;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paCD,
                   const CIEC_BOOL &paLD,
                   const CIEC_DINT &paPV,
                   CAnyBitOutputParameter<CIEC_BOOL> paQ,
                   COutputParameter<CIEC_DINT> paCV) {
        COutputGuard guard_paQ(paQ);
        COutputGuard guard_paCV(paCV);
        var_CD = paCD;
        var_LD = paLD;
        var_PV = paPV;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
        *paCV = var_CV;
      }

      void operator()(const CIEC_BOOL &paCD,
                      const CIEC_BOOL &paLD,
                      const CIEC_DINT &paPV,
                      CAnyBitOutputParameter<CIEC_BOOL> paQ,
                      COutputParameter<CIEC_DINT> paCV) {
        evt_REQ(paCD, paLD, paPV, paQ, paCV);
      }
  };
} // namespace forte::iec61131::counters
