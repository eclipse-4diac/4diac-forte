/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::controlengineering {
  class FORTE_PID final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_PID)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;

      CIEC_REAL var_ERR;
      CIEC_REAL var_ERR_OLD;
      CIEC_REAL var_I_SUM;
      CIEC_REAL var_DT_SEC;
      CIEC_REAL var_P_TERM;
      CIEC_REAL var_I_TERM;
      CIEC_REAL var_D_TERM;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_ALG_INIT(void);
      void alg_ALG_REQ(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateINIT = 1;
      static const TForteInt16 scmStateREQ = 2;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateINIT(CEventChainExecutionThread *const paECET);
      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_PID(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_REAL var_ACTUAL;
      CIEC_REAL var_SET_POINT;
      CIEC_REAL var_KP;
      CIEC_REAL var_TN;
      CIEC_REAL var_TV;
      CIEC_TIME var_Timestep;

      CIEC_BOOL var_QO;
      CIEC_REAL var_OUT;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_ACTUAL;
      CDataConnection *conn_SET_POINT;
      CDataConnection *conn_KP;
      CDataConnection *conn_TN;
      CDataConnection *conn_TV;
      CDataConnection *conn_Timestep;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_REAL> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI, const CIEC_REAL &paACTUAL, const CIEC_REAL &paSET_POINT, const CIEC_REAL &paKP, const CIEC_REAL &paTN, const CIEC_REAL &paTV, const CIEC_TIME &paTimestep, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_REAL> paOUT) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_OUT(paOUT);
        var_QI = paQI;
        var_ACTUAL = paACTUAL;
        var_SET_POINT = paSET_POINT;
        var_KP = paKP;
        var_TN = paTN;
        var_TV = paTV;
        var_Timestep = paTimestep;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paOUT = var_OUT;
      }

      void evt_REQ(const CIEC_BOOL &paQI, const CIEC_REAL &paACTUAL, const CIEC_REAL &paSET_POINT, const CIEC_REAL &paKP, const CIEC_REAL &paTN, const CIEC_REAL &paTV, const CIEC_TIME &paTimestep, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_REAL> paOUT) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_OUT(paOUT);
        var_QI = paQI;
        var_ACTUAL = paACTUAL;
        var_SET_POINT = paSET_POINT;
        var_KP = paKP;
        var_TN = paTN;
        var_TV = paTV;
        var_Timestep = paTimestep;
        executeEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paOUT = var_OUT;
      }
  };
}

