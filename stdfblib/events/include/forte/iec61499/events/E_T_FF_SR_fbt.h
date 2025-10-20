/*************************************************************************
 *** Copyright (c) 2014, 2024 fortiss GmbH, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_T_FF_SR
 *** Description: Event-driven bistable and Toggle
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2024-08-27/Franz Höpfinger - HR Agrartechnik GmbH - Copy E_SR and make this FB
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_T_FF_SR final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_E_T_FF_SR)

    private:
      static const TEventID scmEventSID = 0;
      static const TEventID scmEventRID = 1;
      static const TEventID scmEventCLKID = 2;
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
      FORTE_E_T_FF_SR(StringId paInstanceNameId, CFBContainer &paContainer);

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
        executeEvent(scmEventSID, nullptr);
        *paQ = var_Q;
      }

      void evt_R(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        executeEvent(scmEventRID, nullptr);
        *paQ = var_Q;
      }

      void evt_CLK(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        COutputGuard guard_paQ(paQ);
        executeEvent(scmEventCLKID, nullptr);
        *paQ = var_Q;
      }

      void operator()(CAnyBitOutputParameter<CIEC_BOOL> paQ) {
        evt_S(paQ);
      }
  };
} // namespace forte::iec61499::events
