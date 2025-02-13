/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
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
 *** Name: E_DEMUX8
 *** Description: Event demultiplexor 8 Events
 *** Version:
 ***     1.0: 2024-07-15/Franz Höpfinger - HR Agrartechnik GmbH - copy from E_DEMUX and make a E_DEMUX8 in the same way
 *************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_E_DEMUX8 final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_DEMUX8)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const TEventID scmEventEIID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventEO0ID = 0;
    static const TEventID scmEventEO1ID = 1;
    static const TEventID scmEventEO2ID = 2;
    static const TEventID scmEventEO3ID = 3;
    static const TEventID scmEventEO4ID = 4;
    static const TEventID scmEventEO5ID = 5;
    static const TEventID scmEventEO6ID = 6;
    static const TEventID scmEventEO7ID = 7;
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    CIEC_ANY *getVarInternal(size_t) override;

    static const TForteInt16 scmStateSTART = 0;
    static const TForteInt16 scmStateState = 1;
    static const TForteInt16 scmStateState_1 = 2;
    static const TForteInt16 scmStateState_2 = 3;
    static const TForteInt16 scmStateState_3 = 4;
    static const TForteInt16 scmStateState_4 = 5;
    static const TForteInt16 scmStateState_5 = 6;
    static const TForteInt16 scmStateState_6 = 7;
    static const TForteInt16 scmStateState_7 = 8;
    static const TForteInt16 scmStateState_8 = 9;

    void enterStateSTART(CEventChainExecutionThread *const paECET);
    void enterStateState(CEventChainExecutionThread *const paECET);
    void enterStateState_1(CEventChainExecutionThread *const paECET);
    void enterStateState_2(CEventChainExecutionThread *const paECET);
    void enterStateState_3(CEventChainExecutionThread *const paECET);
    void enterStateState_4(CEventChainExecutionThread *const paECET);
    void enterStateState_5(CEventChainExecutionThread *const paECET);
    void enterStateState_6(CEventChainExecutionThread *const paECET);
    void enterStateState_7(CEventChainExecutionThread *const paECET);
    void enterStateState_8(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_E_DEMUX8(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_UINT var_K;

    CEventConnection conn_EO0;
    CEventConnection conn_EO1;
    CEventConnection conn_EO2;
    CEventConnection conn_EO3;
    CEventConnection conn_EO4;
    CEventConnection conn_EO5;
    CEventConnection conn_EO6;
    CEventConnection conn_EO7;

    CDataConnection *conn_K;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_EI(const CIEC_UINT &paK) {
      var_K = paK;
      executeEvent(scmEventEIID, nullptr);
    }

    void operator()(const CIEC_UINT &paK) {
      evt_EI(paK);
    }
};

