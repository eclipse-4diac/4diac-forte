/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::reconfiguration {
  class FORTE_EC_SET_EVT final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_EC_SET_EVT)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
      void executeRQST();

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_EC_SET_EVT(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_FB_NAME;
      CIEC_WSTRING var_FB_EVENT_IO;
      CIEC_WSTRING var_DST;
      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;
      CEventConnection conn_CNF;
      CDataConnection *conn_QI;
      CDataConnection *conn_FB_NAME;
      CDataConnection *conn_FB_EVENT_IO;
      CDataConnection *conn_DST;
      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      void evt_REQ(const CIEC_BOOL &pa_QI,
                   const CIEC_WSTRING &pa_FB_NAME,
                   const CIEC_WSTRING &pa_FB_EVENT_IO,
                   const CIEC_WSTRING &pa_DST,
                   CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                   COutputParameter<CIEC_WSTRING> pa_STATUS) {
        COutputGuard guard_pa_QO(pa_QO);
        COutputGuard guard_pa_STATUS(pa_STATUS);
        var_QI = pa_QI;
        var_FB_NAME = pa_FB_NAME;
        var_FB_EVENT_IO = pa_FB_EVENT_IO;
        var_DST = pa_DST;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_QO = var_QO;
        *pa_STATUS = var_STATUS;
      }
      void operator()(const CIEC_BOOL &pa_QI,
                      const CIEC_WSTRING &pa_FB_NAME,
                      const CIEC_WSTRING &pa_FB_EVENT_IO,
                      const CIEC_WSTRING &pa_DST,
                      CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                      COutputParameter<CIEC_WSTRING> pa_STATUS) {
        evt_REQ(pa_QI, pa_FB_NAME, pa_FB_EVENT_IO, pa_DST, pa_QO, pa_STATUS);
      }
  };
} // namespace forte::eclipse4diac::reconfiguration
