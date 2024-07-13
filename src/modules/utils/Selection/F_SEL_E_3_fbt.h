/*************************************************************************  
 *** Copyright (c) 2012, 2022 TU Wien ACIN, HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_SEL_E_3
 *** Description: selection event-based selection)
 *** Version:
 ***     1.0: 2012-03-25/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2024-07-13/Franz Höpfinger - HR Agrartechnik GmbH - changed to Basic FB
 *************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_any_variant.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_F_SEL_E_3 final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_F_SEL_E_3)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventREQ0ID = 0;
    static const TEventID scmEventREQ1ID = 1;
    static const TEventID scmEventREQ2ID = 2;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCNFID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    CIEC_ANY *getVarInternal(size_t) override;

    void alg_COPY0(void);
    void alg_COPY1(void);
    void alg_COPY2(void);

    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateCOPY0 = 1;
    static const TForteInt16 scmStateCOPY1 = 2;
    static const TForteInt16 scmStateCOPY2 = 3;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateCOPY0(CEventChainExecutionThread *const paECET);
    void enterStateCOPY1(CEventChainExecutionThread *const paECET);
    void enterStateCOPY2(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_F_SEL_E_3(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_ANY_VARIANT var_IN0;
    CIEC_ANY_VARIANT var_IN1;
    CIEC_ANY_VARIANT var_IN2;

    CIEC_ANY_VARIANT var_OUT;

    CIEC_ANY_VARIANT var_conn_OUT;

    CEventConnection conn_CNF;

    CDataConnection *conn_IN0;
    CDataConnection *conn_IN1;
    CDataConnection *conn_IN2;

    CDataConnection conn_OUT;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ0(const CIEC_ANY &paIN0, const CIEC_ANY &paIN1, const CIEC_ANY &paIN2, CIEC_ANY &paOUT) {
      var_IN0 = paIN0;
      var_IN1 = paIN1;
      var_IN2 = paIN2;
      executeEvent(scmEventREQ0ID, nullptr);
      paOUT.setValue(var_OUT.unwrap());
    }

    void evt_REQ1(const CIEC_ANY &paIN0, const CIEC_ANY &paIN1, const CIEC_ANY &paIN2, CIEC_ANY &paOUT) {
      var_IN0 = paIN0;
      var_IN1 = paIN1;
      var_IN2 = paIN2;
      executeEvent(scmEventREQ1ID, nullptr);
      paOUT.setValue(var_OUT.unwrap());
    }

    void evt_REQ2(const CIEC_ANY &paIN0, const CIEC_ANY &paIN1, const CIEC_ANY &paIN2, CIEC_ANY &paOUT) {
      var_IN0 = paIN0;
      var_IN1 = paIN1;
      var_IN2 = paIN2;
      executeEvent(scmEventREQ2ID, nullptr);
      paOUT.setValue(var_OUT.unwrap());
    }

    void operator()(const CIEC_ANY &paIN0, const CIEC_ANY &paIN1, const CIEC_ANY &paIN2, CIEC_ANY &paOUT) {
      evt_REQ0(paIN0, paIN1, paIN2, paOUT);
    }
};


