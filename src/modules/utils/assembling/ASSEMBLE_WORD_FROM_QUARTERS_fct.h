/*******************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ASSEMBLE_WORD_FROM_QUARTERS
 *** Description: this Function combines the 8 QUARTER BYTES to a WORD
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_byte.h"
#include "forte_word.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_ASSEMBLE_WORD_FROM_QUARTERS final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_ASSEMBLE_WORD_FROM_QUARTERS)

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
    FORTE_ASSEMBLE_WORD_FROM_QUARTERS(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BYTE var_QUARTER_BYTE_00;
    CIEC_BYTE var_QUARTER_BYTE_01;
    CIEC_BYTE var_QUARTER_BYTE_02;
    CIEC_BYTE var_QUARTER_BYTE_03;
    CIEC_BYTE var_QUARTER_BYTE_04;
    CIEC_BYTE var_QUARTER_BYTE_05;
    CIEC_BYTE var_QUARTER_BYTE_06;
    CIEC_BYTE var_QUARTER_BYTE_07;

    CIEC_WORD var_;

    CIEC_WORD var_conn_;

    CEventConnection conn_CNF;

    CDataConnection *conn_QUARTER_BYTE_00;
    CDataConnection *conn_QUARTER_BYTE_01;
    CDataConnection *conn_QUARTER_BYTE_02;
    CDataConnection *conn_QUARTER_BYTE_03;
    CDataConnection *conn_QUARTER_BYTE_04;
    CDataConnection *conn_QUARTER_BYTE_05;
    CDataConnection *conn_QUARTER_BYTE_06;
    CDataConnection *conn_QUARTER_BYTE_07;

    CDataConnection conn_;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BYTE &paQUARTER_BYTE_00, const CIEC_BYTE &paQUARTER_BYTE_01, const CIEC_BYTE &paQUARTER_BYTE_02, const CIEC_BYTE &paQUARTER_BYTE_03, const CIEC_BYTE &paQUARTER_BYTE_04, const CIEC_BYTE &paQUARTER_BYTE_05, const CIEC_BYTE &paQUARTER_BYTE_06, const CIEC_BYTE &paQUARTER_BYTE_07, CIEC_WORD &pa) {
      var_QUARTER_BYTE_00 = paQUARTER_BYTE_00;
      var_QUARTER_BYTE_01 = paQUARTER_BYTE_01;
      var_QUARTER_BYTE_02 = paQUARTER_BYTE_02;
      var_QUARTER_BYTE_03 = paQUARTER_BYTE_03;
      var_QUARTER_BYTE_04 = paQUARTER_BYTE_04;
      var_QUARTER_BYTE_05 = paQUARTER_BYTE_05;
      var_QUARTER_BYTE_06 = paQUARTER_BYTE_06;
      var_QUARTER_BYTE_07 = paQUARTER_BYTE_07;
      executeEvent(scmEventREQID, nullptr);
      pa = var_;
    }

    void operator()(const CIEC_BYTE &paQUARTER_BYTE_00, const CIEC_BYTE &paQUARTER_BYTE_01, const CIEC_BYTE &paQUARTER_BYTE_02, const CIEC_BYTE &paQUARTER_BYTE_03, const CIEC_BYTE &paQUARTER_BYTE_04, const CIEC_BYTE &paQUARTER_BYTE_05, const CIEC_BYTE &paQUARTER_BYTE_06, const CIEC_BYTE &paQUARTER_BYTE_07, CIEC_WORD &pa) {
      evt_REQ(paQUARTER_BYTE_00, paQUARTER_BYTE_01, paQUARTER_BYTE_02, paQUARTER_BYTE_03, paQUARTER_BYTE_04, paQUARTER_BYTE_05, paQUARTER_BYTE_06, paQUARTER_BYTE_07, pa);
    }
};

CIEC_WORD func_ASSEMBLE_WORD_FROM_QUARTERS(CIEC_BYTE st_lv_QUARTER_BYTE_00, CIEC_BYTE st_lv_QUARTER_BYTE_01, CIEC_BYTE st_lv_QUARTER_BYTE_02, CIEC_BYTE st_lv_QUARTER_BYTE_03, CIEC_BYTE st_lv_QUARTER_BYTE_04, CIEC_BYTE st_lv_QUARTER_BYTE_05, CIEC_BYTE st_lv_QUARTER_BYTE_06, CIEC_BYTE st_lv_QUARTER_BYTE_07);


