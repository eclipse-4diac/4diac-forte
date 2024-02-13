/*************************************************************************  
 *** Copyright (c) 2023 HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_WORD_TO_PERCENT
 *** Description:
 *** Version:
 ***     1.0: 2023-10-21/franz -  -
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_word.h"
#include "forte_real.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventREQID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCNFID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_WORD var_WI;
    CIEC_REAL var_RO_E;

    CIEC_BOOL var_QO;
    CIEC_REAL var_RO;
    CIEC_WORD var_WO;

    CIEC_BOOL var_conn_QO;
    CIEC_REAL var_conn_RO;
    CIEC_WORD var_conn_WO;

    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_WI;
    CDataConnection *conn_RO_E;

    CDataConnection conn_QO;
    CDataConnection conn_RO;
    CDataConnection conn_WO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_WORD &paWI, const CIEC_REAL &paRO_E, CIEC_BOOL &paQO, CIEC_REAL &paRO, CIEC_WORD &paWO) {
      var_QI = paQI;
      var_WI = paWI;
      var_RO_E = paRO_E;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paRO = var_RO;
      paWO = var_WO;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_WORD &paWI, const CIEC_REAL &paRO_E, CIEC_BOOL &paQO, CIEC_REAL &paRO, CIEC_WORD &paWO) {
      evt_REQ(paQI, paWI, paRO_E, paQO, paRO, paWO);
    }
};

void func_FIELDBUS_WORD_TO_PERCENT(CIEC_BOOL st_lv_QI, CIEC_WORD st_lv_WI, CIEC_REAL st_lv_RO_E, CIEC_BOOL &st_lv_QO, CIEC_REAL &st_lv_RO, CIEC_WORD &st_lv_WO);


