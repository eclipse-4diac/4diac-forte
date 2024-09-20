/*************************************************************************  
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SCALE_ESC
 *** Description: Scaling Function Block with escape
 *** Version:
 ***     1.0: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_real.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_signalprocessing__SCALE_ESC final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_signalprocessing__SCALE_ESC)

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
    FORTE_signalprocessing__SCALE_ESC(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_REAL var_IN;
    CIEC_REAL var_MAX1;
    CIEC_REAL var_MIN1;
    CIEC_REAL var_MAX1_ESC;
    CIEC_REAL var_MIN1_ESC;
    CIEC_REAL var_MAX2;
    CIEC_REAL var_MIN2;
    CIEC_REAL var_MAX2_ESC;
    CIEC_REAL var_MIN2_ESC;

    CIEC_REAL var_;

    CIEC_REAL var_conn_;

    CEventConnection conn_CNF;

    CDataConnection *conn_IN;
    CDataConnection *conn_MAX1;
    CDataConnection *conn_MIN1;
    CDataConnection *conn_MAX1_ESC;
    CDataConnection *conn_MIN1_ESC;
    CDataConnection *conn_MAX2;
    CDataConnection *conn_MIN2;
    CDataConnection *conn_MAX2_ESC;
    CDataConnection *conn_MIN2_ESC;

    CDataConnection conn_;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_REAL &paIN, const CIEC_REAL &paMAX1, const CIEC_REAL &paMIN1, const CIEC_REAL &paMAX1_ESC, const CIEC_REAL &paMIN1_ESC, const CIEC_REAL &paMAX2, const CIEC_REAL &paMIN2, const CIEC_REAL &paMAX2_ESC, const CIEC_REAL &paMIN2_ESC, CIEC_REAL &pa) {
      var_IN = paIN;
      var_MAX1 = paMAX1;
      var_MIN1 = paMIN1;
      var_MAX1_ESC = paMAX1_ESC;
      var_MIN1_ESC = paMIN1_ESC;
      var_MAX2 = paMAX2;
      var_MIN2 = paMIN2;
      var_MAX2_ESC = paMAX2_ESC;
      var_MIN2_ESC = paMIN2_ESC;
      executeEvent(scmEventREQID, nullptr);
      pa = var_;
    }

    void operator()(const CIEC_REAL &paIN, const CIEC_REAL &paMAX1, const CIEC_REAL &paMIN1, const CIEC_REAL &paMAX1_ESC, const CIEC_REAL &paMIN1_ESC, const CIEC_REAL &paMAX2, const CIEC_REAL &paMIN2, const CIEC_REAL &paMAX2_ESC, const CIEC_REAL &paMIN2_ESC, CIEC_REAL &pa) {
      evt_REQ(paIN, paMAX1, paMIN1, paMAX1_ESC, paMIN1_ESC, paMAX2, paMIN2, paMAX2_ESC, paMIN2_ESC, pa);
    }
};

CIEC_REAL func_SCALE_ESC(CIEC_REAL st_lv_IN, CIEC_REAL st_lv_MAX1, CIEC_REAL st_lv_MIN1, CIEC_REAL st_lv_MAX1_ESC, CIEC_REAL st_lv_MIN1_ESC, CIEC_REAL st_lv_MAX2, CIEC_REAL st_lv_MIN2, CIEC_REAL st_lv_MAX2_ESC, CIEC_REAL st_lv_MIN2_ESC);

