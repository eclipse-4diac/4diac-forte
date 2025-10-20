/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/io/eIO/eConfigFB.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/eIO/eGenAdapter_adp.h"

namespace forte::io {
  class FORTE_eIWconfig final : public CeConfigFB {
      DECLARE_FIRMWARE_FB(FORTE_eIWconfig)

    private:
      static const TEventID scmEventCONFID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_eIWconfig(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WORD var_ST;
      CIEC_WORD var_BT;
      CIEC_WORD var_GRAD;

      CIEC_WSTRING var_STATUS;

      CSocketPin<FORTE_eGenAdapter_Socket> var_eIW;

      CEventConnection conn_CNF;

      CDataConnection *conn_ST;
      CDataConnection *conn_BT;
      CDataConnection *conn_GRAD;

      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      ISocketPin *getSocketPinUnchecked(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_CONF(const CIEC_WORD &paST,
                    const CIEC_WORD &paBT,
                    const CIEC_WORD &paGRAD,
                    COutputParameter<CIEC_WSTRING> paSTATUS) {
        COutputGuard guard_paSTATUS(paSTATUS);
        var_ST = paST;
        var_BT = paBT;
        var_GRAD = paGRAD;
        executeEvent(scmEventCONFID, nullptr);
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_WORD &paST,
                      const CIEC_WORD &paBT,
                      const CIEC_WORD &paGRAD,
                      COutputParameter<CIEC_WSTRING> paSTATUS) {
        evt_CONF(paST, paBT, paGRAD, paSTATUS);
      }

      bool eventGen() override;
  };
} // namespace forte::io
