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
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: FB_TOF_LTIME
 *** Description: standard timer function block (off-delay timing) (LTIME)
 *** Version:
 ***     1.0: 2026-02-01/Franz Höpfinger - HR Agrartechnik GmbH - Copy over from FB_TOF and made this Block
 *************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/forte_st_util.h"

namespace forte::iec61131::timers {
  class FORTE_FB_TOF_LTIME final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_TOF_LTIME)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_BOOL var_MEM;
      CIEC_LTIME var_StartTime;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_FB_TOF_LTIME(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_IN;
      CIEC_LTIME var_PT;

      CIEC_BOOL var_Q;
      CIEC_LTIME var_ET;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;
      CDataConnection *conn_PT;

      COutDataConnection<CIEC_BOOL> conn_Q;
      COutDataConnection<CIEC_LTIME> conn_ET;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_BOOL &paIN,
                   const CIEC_LTIME &paPT,
                   CAnyBitOutputParameter<CIEC_BOOL> paQ,
                   COutputParameter<CIEC_LTIME> paET) {
        COutputGuard guard_Q(paQ);
        COutputGuard guard_ET(paET);
        var_IN = paIN;
        var_PT = paPT;
        executeEvent(scmEventREQID, nullptr);
        *paQ = var_Q;
        *paET = var_ET;
      }

      void operator()(const CIEC_BOOL &paIN,
                      const CIEC_LTIME &paPT,
                      CAnyBitOutputParameter<CIEC_BOOL> paQ,
                      COutputParameter<CIEC_LTIME> paET) {
        evt_REQ(std::forward<const CIEC_BOOL &>(paIN), std::forward<const CIEC_LTIME &>(paPT),
                std::forward<CAnyBitOutputParameter<CIEC_BOOL>>(paQ), std::forward<COutputParameter<CIEC_LTIME>>(paET));
      }
  };
} // namespace forte::iec61131::timers
