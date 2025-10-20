/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Gerhard Ebenhofer,
 *   Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_SR final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_SR)

    private:
      static const TEventID scmEventSID = 0;
      static const TEventID scmEventRID = 1;
      static const TEventID scmEventEOID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_SET(void);
      void alg_RESET(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateSET = 1;
      static const TForteInt16 scmStateRESET = 2;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateSET(CEventChainExecutionThread *const paECET);
      void enterStateRESET(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_SR(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_Q;

      CEventConnection conn_EO;

      COutDataConnection<CIEC_BOOL> conn_Q;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_S(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        receiveInputEvent(scmEventSID, nullptr);
        *paQ = var_Q;
      }

      void evt_R(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        receiveInputEvent(scmEventRID, nullptr);
        *paQ = var_Q;
      }

      void operator()(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        evt_S(paQ);
      }
  };
} // namespace forte::iec61499::events
