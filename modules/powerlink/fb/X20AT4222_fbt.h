/*******************************************************************************
 * Copyright (c) 2012 - 2024 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Michael Gafert - moved common functions to super class, changed to new FB style
 *******************************************************************************/

#pragma once

#include "PowerlinkFunctionBlockAT.h"
#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_real.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_X20AT4222 final : public PowerlinkFunctionBlockAT {
    DECLARE_FIRMWARE_FB(FORTE_X20AT4222)

  private:
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_X20AT4222(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_CNID;
    CIEC_UINT var_MODID;

    CIEC_BOOL var_QO;
    CIEC_USINT var_CNIDO;
    CIEC_STRING var_STATUS;
    CIEC_REAL var_T01;
    CIEC_REAL var_T02;
    CIEC_REAL var_T03;
    CIEC_REAL var_T04;

    CEventConnection conn_INITO;
    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_CNID;
    CDataConnection *conn_MODID;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_USINT> conn_CNIDO;
    COutDataConnection<CIEC_STRING> conn_STATUS;
    COutDataConnection<CIEC_REAL> conn_T01;
    COutDataConnection<CIEC_REAL> conn_T02;
    COutDataConnection<CIEC_REAL> conn_T03;
    COutDataConnection<CIEC_REAL> conn_T04;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_USINT &paCNID,
                  const CIEC_UINT &paMODID,
                  CAnyBitOutputParameter<CIEC_BOOL> paQO,
                  COutputParameter<CIEC_USINT> paCNIDO,
                  COutputParameter<CIEC_STRING> paSTATUS,
                  COutputParameter<CIEC_REAL> paT01,
                  COutputParameter<CIEC_REAL> paT02,
                  COutputParameter<CIEC_REAL> paT03,
                  COutputParameter<CIEC_REAL> paT04) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paCNIDO(paCNIDO);
      COutputGuard guard_paSTATUS(paSTATUS);
      COutputGuard guard_paT01(paT01);
      COutputGuard guard_paT02(paT02);
      COutputGuard guard_paT03(paT03);
      COutputGuard guard_paT04(paT04);
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paCNIDO = var_CNIDO;
      *paSTATUS = var_STATUS;
      *paT01 = var_T01;
      *paT02 = var_T02;
      *paT03 = var_T03;
      *paT04 = var_T04;
    }

    void evt_REQ(const CIEC_BOOL &paQI,
                 const CIEC_USINT &paCNID,
                 const CIEC_UINT &paMODID,
                 CAnyBitOutputParameter<CIEC_BOOL> paQO,
                 COutputParameter<CIEC_USINT> paCNIDO,
                 COutputParameter<CIEC_STRING> paSTATUS,
                 COutputParameter<CIEC_REAL> paT01,
                 COutputParameter<CIEC_REAL> paT02,
                 COutputParameter<CIEC_REAL> paT03,
                 COutputParameter<CIEC_REAL> paT04) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paCNIDO(paCNIDO);
      COutputGuard guard_paSTATUS(paSTATUS);
      COutputGuard guard_paT01(paT01);
      COutputGuard guard_paT02(paT02);
      COutputGuard guard_paT03(paT03);
      COutputGuard guard_paT04(paT04);
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventREQID, nullptr);
      *paQO = var_QO;
      *paCNIDO = var_CNIDO;
      *paSTATUS = var_STATUS;
      *paT01 = var_T01;
      *paT02 = var_T02;
      *paT03 = var_T03;
      *paT04 = var_T04;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_USINT &paCNID,
                    const CIEC_UINT &paMODID,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_USINT> paCNIDO,
                    COutputParameter<CIEC_STRING> paSTATUS,
                    COutputParameter<CIEC_REAL> paT01,
                    COutputParameter<CIEC_REAL> paT02,
                    COutputParameter<CIEC_REAL> paT03,
                    COutputParameter<CIEC_REAL> paT04) {
      evt_INIT(paQI, paCNID, paMODID, paQO, paCNIDO, paSTATUS, paT01, paT02, paT03, paT04);
    }
};
