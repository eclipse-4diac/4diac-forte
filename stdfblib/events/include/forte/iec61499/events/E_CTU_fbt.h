/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_CTU final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_CTU)

    private:
      static const TEventID scmEventCUID = 0;
      static const TEventID scmEventRID = 1;
      static const TEventID scmEventCUOID = 0;
      static const TEventID scmEventROID = 1;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_R(void);
      void alg_CU(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateCU = 1;
      static const TForteInt16 scmStateR = 2;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateCU(CEventChainExecutionThread *const paECET);
      void enterStateR(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_CTU(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_UINT var_PV;

      CIEC_BOOL var_Q;
      CIEC_UINT var_CV;

      CEventConnection conn_CUO;
      CEventConnection conn_RO;

      CDataConnection *conn_PV;

      COutDataConnection<CIEC_BOOL> conn_Q;
      COutDataConnection<CIEC_UINT> conn_CV;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_CU(const CIEC_UINT &paPV, CAnyBitOutputParameter<CIEC_BOOL> paQ, COutputParameter<CIEC_UINT> paCV) {
        COutputGuard guard_paQ(paQ);
        COutputGuard guard_paCV(paCV);
        var_PV = paPV;
        receiveInputEvent(scmEventCUID, nullptr);
        *paQ = var_Q;
        *paCV = var_CV;
      }

      void evt_R(const CIEC_UINT &paPV, CAnyBitOutputParameter<CIEC_BOOL> paQ, COutputParameter<CIEC_UINT> paCV) {
        COutputGuard guard_paQ(paQ);
        COutputGuard guard_paCV(paCV);
        var_PV = paPV;
        receiveInputEvent(scmEventRID, nullptr);
        *paQ = var_Q;
        *paCV = var_CV;
      }

      void operator()(const CIEC_UINT &paPV, CAnyBitOutputParameter<CIEC_BOOL> paQ, COutputParameter<CIEC_UINT> paCV) {
        evt_CU(paPV, paQ, paCV);
      }
  };
} // namespace forte::iec61499::events
