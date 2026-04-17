/*************************************************************************
 *** Copyright (c) 2026 HR Agartechnik GmbH
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604152003!
 ***
 *** Name: E_D_FF_ANY
 *** Description: Data latch (d) flip flop
 *** Version:
 ***     1.0: 2026-04-17/Franz Höpfinger - HR Agrartechnik GmbH - E_D_FF copied and replace BOOL by ANY
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/forte_st_util.h"

namespace forte::iec61499::events {
  class FORTE_E_D_FF_ANY final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_D_FF_ANY)

    private:
      static const TEventID scmEventEOID = 0;
      static const TEventID scmEventCLKID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_D_FF_ANY(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_VARIANT var_D;

      CIEC_ANY_VARIANT var_Q;

      CEventConnection conn_EO;

      CDataConnection *conn_D;

      COutDataConnection<CIEC_ANY_VARIANT> conn_Q;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_CLK(const CIEC_ANY_VARIANT &paD, COutputParameter<CIEC_ANY_VARIANT> paQ) {
        COutputGuard guard_Q(paQ);
        var_D = paD;
        executeEvent(scmEventCLKID, nullptr);
        paQ->setValue(var_Q.unwrap());
      }

      void operator()(const CIEC_ANY_VARIANT &paD, COutputParameter<CIEC_ANY_VARIANT> paQ) {
        evt_CLK(std::forward<const CIEC_ANY_VARIANT &>(paD), std::forward<COutputParameter<CIEC_ANY_VARIANT>>(paQ));
      }
  };
} // namespace forte::iec61499::events
