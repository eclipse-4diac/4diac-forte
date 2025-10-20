/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: DualHysteresis
 *** Description: 2-way conversion of Analog to Digital with Hysteresis
 *** Version:
 ***     1.0: 2023-06-06/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_real.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_DualHysteresis final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_DualHysteresis)

    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_initialize(void);
      void alg_deInitialize(void);
      void alg_alNeutral(void);
      void alg_alUp(void);
      void alg_alDown(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateInit = 1;
      static const TForteInt16 scmStateUP = 2;
      static const TForteInt16 scmStateNeutral = 3;
      static const TForteInt16 scmStateDeInit = 4;
      static const TForteInt16 scmStateDOWN = 5;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateInit(CEventChainExecutionThread *const paECET);
      void enterStateUP(CEventChainExecutionThread *const paECET);
      void enterStateNeutral(CEventChainExecutionThread *const paECET);
      void enterStateDeInit(CEventChainExecutionThread *const paECET);
      void enterStateDOWN(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_DualHysteresis(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_REAL var_MI;
      CIEC_REAL var_DEAD;
      CIEC_REAL var_HYSTERESIS;
      CIEC_REAL var_INPUT;

      CIEC_BOOL var_QO;
      CIEC_BOOL var_DO_UP;
      CIEC_BOOL var_DO_DOWN;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_MI;
      CDataConnection *conn_DEAD;
      CDataConnection *conn_HYSTERESIS;
      CDataConnection *conn_INPUT;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_BOOL> conn_DO_UP;
      COutDataConnection<CIEC_BOOL> conn_DO_DOWN;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI,
                    const CIEC_REAL &paMI,
                    const CIEC_REAL &paDEAD,
                    const CIEC_REAL &paHYSTERESIS,
                    const CIEC_REAL &paINPUT,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    CAnyBitOutputParameter<CIEC_BOOL> paDO_UP,
                    CAnyBitOutputParameter<CIEC_BOOL> paDO_DOWN) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paDO_UP(paDO_UP);
        COutputGuard guard_paDO_DOWN(paDO_DOWN);
        var_QI = paQI;
        var_MI = paMI;
        var_DEAD = paDEAD;
        var_HYSTERESIS = paHYSTERESIS;
        var_INPUT = paINPUT;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paDO_UP = var_DO_UP;
        *paDO_DOWN = var_DO_DOWN;
      }

      void evt_REQ(const CIEC_BOOL &paQI,
                   const CIEC_REAL &paMI,
                   const CIEC_REAL &paDEAD,
                   const CIEC_REAL &paHYSTERESIS,
                   const CIEC_REAL &paINPUT,
                   CAnyBitOutputParameter<CIEC_BOOL> paQO,
                   CAnyBitOutputParameter<CIEC_BOOL> paDO_UP,
                   CAnyBitOutputParameter<CIEC_BOOL> paDO_DOWN) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paDO_UP(paDO_UP);
        COutputGuard guard_paDO_DOWN(paDO_DOWN);
        var_QI = paQI;
        var_MI = paMI;
        var_DEAD = paDEAD;
        var_HYSTERESIS = paHYSTERESIS;
        var_INPUT = paINPUT;
        executeEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paDO_UP = var_DO_UP;
        *paDO_DOWN = var_DO_DOWN;
      }

      void operator()(const CIEC_BOOL &paQI,
                      const CIEC_REAL &paMI,
                      const CIEC_REAL &paDEAD,
                      const CIEC_REAL &paHYSTERESIS,
                      const CIEC_REAL &paINPUT,
                      CAnyBitOutputParameter<CIEC_BOOL> paQO,
                      CAnyBitOutputParameter<CIEC_BOOL> paDO_UP,
                      CAnyBitOutputParameter<CIEC_BOOL> paDO_DOWN) {
        evt_INIT(paQI, paMI, paDEAD, paHYSTERESIS, paINPUT, paQO, paDO_UP, paDO_DOWN);
      }
  };
} // namespace forte::eclipse4diac::signalprocessing
