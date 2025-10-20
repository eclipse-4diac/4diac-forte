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

#include "PowerlinkFunctionBlockDI.h"
#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_X20DI9372 final : public PowerlinkFunctionBlockDI {
    DECLARE_FIRMWARE_FB(FORTE_X20DI9372)

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
    FORTE_X20DI9372(forte::StringId paInstanceNameId, CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_CNID;
    CIEC_UINT var_MODID;

    CIEC_BOOL var_QO;
    CIEC_USINT var_CNIDO;
    CIEC_STRING var_STATUS;
    CIEC_BOOL var_DI01;
    CIEC_BOOL var_DI02;
    CIEC_BOOL var_DI03;
    CIEC_BOOL var_DI04;
    CIEC_BOOL var_DI05;
    CIEC_BOOL var_DI06;
    CIEC_BOOL var_DI07;
    CIEC_BOOL var_DI08;
    CIEC_BOOL var_DI09;
    CIEC_BOOL var_DI10;
    CIEC_BOOL var_DI11;
    CIEC_BOOL var_DI12;

    CEventConnection conn_INITO;
    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_CNID;
    CDataConnection *conn_MODID;

    COutDataConnection<CIEC_BOOL> conn_QO;
    COutDataConnection<CIEC_USINT> conn_CNIDO;
    COutDataConnection<CIEC_STRING> conn_STATUS;
    COutDataConnection<CIEC_BOOL> conn_DI01;
    COutDataConnection<CIEC_BOOL> conn_DI02;
    COutDataConnection<CIEC_BOOL> conn_DI03;
    COutDataConnection<CIEC_BOOL> conn_DI04;
    COutDataConnection<CIEC_BOOL> conn_DI05;
    COutDataConnection<CIEC_BOOL> conn_DI06;
    COutDataConnection<CIEC_BOOL> conn_DI07;
    COutDataConnection<CIEC_BOOL> conn_DI08;
    COutDataConnection<CIEC_BOOL> conn_DI09;
    COutDataConnection<CIEC_BOOL> conn_DI10;
    COutDataConnection<CIEC_BOOL> conn_DI11;
    COutDataConnection<CIEC_BOOL> conn_DI12;

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
                  CAnyBitOutputParameter<CIEC_BOOL> paDI01,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI02,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI03,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI04,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI05,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI06,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI07,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI08,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI09,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI10,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI11,
                  CAnyBitOutputParameter<CIEC_BOOL> paDI12) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paCNIDO(paCNIDO);
      COutputGuard guard_paSTATUS(paSTATUS);
      COutputGuard guard_paDI01(paDI01);
      COutputGuard guard_paDI02(paDI02);
      COutputGuard guard_paDI03(paDI03);
      COutputGuard guard_paDI04(paDI04);
      COutputGuard guard_paDI05(paDI05);
      COutputGuard guard_paDI06(paDI06);
      COutputGuard guard_paDI07(paDI07);
      COutputGuard guard_paDI08(paDI08);
      COutputGuard guard_paDI09(paDI09);
      COutputGuard guard_paDI10(paDI10);
      COutputGuard guard_paDI11(paDI11);
      COutputGuard guard_paDI12(paDI12);
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventINITID, nullptr);
      *paQO = var_QO;
      *paCNIDO = var_CNIDO;
      *paSTATUS = var_STATUS;
      *paDI01 = var_DI01;
      *paDI02 = var_DI02;
      *paDI03 = var_DI03;
      *paDI04 = var_DI04;
      *paDI05 = var_DI05;
      *paDI06 = var_DI06;
      *paDI07 = var_DI07;
      *paDI08 = var_DI08;
      *paDI09 = var_DI09;
      *paDI10 = var_DI10;
      *paDI11 = var_DI11;
      *paDI12 = var_DI12;
    }

    void evt_REQ(const CIEC_BOOL &paQI,
                 const CIEC_USINT &paCNID,
                 const CIEC_UINT &paMODID,
                 CAnyBitOutputParameter<CIEC_BOOL> paQO,
                 COutputParameter<CIEC_USINT> paCNIDO,
                 COutputParameter<CIEC_STRING> paSTATUS,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI01,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI02,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI03,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI04,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI05,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI06,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI07,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI08,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI09,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI10,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI11,
                 CAnyBitOutputParameter<CIEC_BOOL> paDI12) {
      COutputGuard guard_paQO(paQO);
      COutputGuard guard_paCNIDO(paCNIDO);
      COutputGuard guard_paSTATUS(paSTATUS);
      COutputGuard guard_paDI01(paDI01);
      COutputGuard guard_paDI02(paDI02);
      COutputGuard guard_paDI03(paDI03);
      COutputGuard guard_paDI04(paDI04);
      COutputGuard guard_paDI05(paDI05);
      COutputGuard guard_paDI06(paDI06);
      COutputGuard guard_paDI07(paDI07);
      COutputGuard guard_paDI08(paDI08);
      COutputGuard guard_paDI09(paDI09);
      COutputGuard guard_paDI10(paDI10);
      COutputGuard guard_paDI11(paDI11);
      COutputGuard guard_paDI12(paDI12);
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventREQID, nullptr);
      *paQO = var_QO;
      *paCNIDO = var_CNIDO;
      *paSTATUS = var_STATUS;
      *paDI01 = var_DI01;
      *paDI02 = var_DI02;
      *paDI03 = var_DI03;
      *paDI04 = var_DI04;
      *paDI05 = var_DI05;
      *paDI06 = var_DI06;
      *paDI07 = var_DI07;
      *paDI08 = var_DI08;
      *paDI09 = var_DI09;
      *paDI10 = var_DI10;
      *paDI11 = var_DI11;
      *paDI12 = var_DI12;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_USINT &paCNID,
                    const CIEC_UINT &paMODID,
                    CAnyBitOutputParameter<CIEC_BOOL> paQO,
                    COutputParameter<CIEC_USINT> paCNIDO,
                    COutputParameter<CIEC_STRING> paSTATUS,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI01,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI02,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI03,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI04,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI05,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI06,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI07,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI08,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI09,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI10,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI11,
                    CAnyBitOutputParameter<CIEC_BOOL> paDI12) {
      evt_INIT(paQI, paCNID, paMODID, paQO, paCNIDO, paSTATUS, paDI01, paDI02, paDI03, paDI04, paDI05, paDI06, paDI07,
               paDI08, paDI09, paDI10, paDI11, paDI12);
    }
};
