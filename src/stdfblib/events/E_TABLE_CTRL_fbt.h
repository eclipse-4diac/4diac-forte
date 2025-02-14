/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TABLE_CTRL
 *** Description: Support function block for E_TABLE
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_time.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_E_TABLE_CTRL final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_TABLE_CTRL)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventSTARTID = 0;
    static const TEventID scmEventCLKID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCLKOID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    CIEC_ANY *getVarInternal(size_t) override;

    void alg_INIT(void);
    void alg_NEXT_STEP(void);

    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateINIT = 1;
    static const TForteInt16 scmStateINIT1 = 2;
    static const TForteInt16 scmStateNEXT_STEP = 3;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateINIT(CEventChainExecutionThread *const paECET);
    void enterStateINIT1(CEventChainExecutionThread *const paECET);
    void enterStateNEXT_STEP(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_E_TABLE_CTRL(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3> var_DT;
    CIEC_UINT var_N;

    CIEC_TIME var_DTO;
    CIEC_UINT var_CV;

    CIEC_TIME var_conn_DTO;
    CIEC_UINT var_conn_CV;

    CEventConnection conn_CLKO;

    CDataConnection *conn_DT;
    CDataConnection *conn_N;

    CDataConnection conn_DTO;
    CDataConnection conn_CV;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_START(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_TIME &paDTO, CIEC_UINT &paCV) {
      var_DT = paDT;
      var_N = paN;
      executeEvent(scmEventSTARTID, nullptr);
      paDTO = var_DTO;
      paCV = var_CV;
    }

    void evt_CLK(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_TIME &paDTO, CIEC_UINT &paCV) {
      var_DT = paDT;
      var_N = paN;
      executeEvent(scmEventCLKID, nullptr);
      paDTO = var_DTO;
      paCV = var_CV;
    }

    void operator()(const CIEC_ARRAY_COMMON<CIEC_TIME> &paDT, const CIEC_UINT &paN, CIEC_TIME &paDTO, CIEC_UINT &paCV) {
      evt_START(paDT, paN, paDTO, paCV);
    }
};

