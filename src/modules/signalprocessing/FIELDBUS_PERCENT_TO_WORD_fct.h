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
 *** Name: FIELDBUS_PERCENT_TO_WORD
 *** Description:
 *** Version:
 ***     1.0: 2023-10-21/franz -  -
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_real.h"
#include "forte_word.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD)

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
    FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_REAL var_RI;

    CIEC_BOOL var_QO;
    CIEC_WORD var_WO;

    CIEC_BOOL var_conn_QO;
    CIEC_WORD var_conn_WO;

    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_RI;

    CDataConnection conn_QO;
    CDataConnection conn_WO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_REAL &paRI, CIEC_BOOL &paQO, CIEC_WORD &paWO) {
      var_QI = paQI;
      var_RI = paRI;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paWO = var_WO;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_REAL &paRI, CIEC_BOOL &paQO, CIEC_WORD &paWO) {
      evt_REQ(paQI, paRI, paQO, paWO);
    }
};

void func_FIELDBUS_PERCENT_TO_WORD(CIEC_BOOL st_lv_QI, CIEC_REAL st_lv_RI, CIEC_BOOL &st_lv_QO, CIEC_WORD &st_lv_WO);


