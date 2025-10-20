/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "../types/forte_localizedtext.h"

namespace forte::com_infra::opc_ua {
  class FORTE_LocalizedText2LocalizedText : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_LocalizedText2LocalizedText)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_LocalizedText2LocalizedText(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_LocalizedText var_IN;
      CIEC_LocalizedText var_OUT;

      CEventConnection conn_CNF;
      CDataConnection *conn_IN;
      COutDataConnection<CIEC_LocalizedText> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_LocalizedText &pa_IN, COutputParameter<CIEC_LocalizedText> pa_OUT) {
        COutputGuard guard_pa_OUT(pa_OUT);
        var_IN = pa_IN;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_OUT = var_OUT;
      }

      void operator()(const CIEC_LocalizedText &pa_IN, COutputParameter<CIEC_LocalizedText> pa_OUT) {
        evt_REQ(pa_IN, pa_OUT);
      }

    protected:
      void setInitialValues() override;
  };
} // namespace forte::com_infra::opc_ua
