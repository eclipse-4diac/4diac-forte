/*************************************************************************
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
 *** Name: SPLIT_BYTE_INTO_QUARTERS
 *** Description: this Function extracts the 4 QUARTER BYTE from a byte
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_byte.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_SPLIT_BYTE_INTO_QUARTERS final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_SPLIT_BYTE_INTO_QUARTERS)

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
    FORTE_SPLIT_BYTE_INTO_QUARTERS(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BYTE var_IN;

    CIEC_BYTE var_QUARTER_BYTE_00;
    CIEC_BYTE var_QUARTER_BYTE_01;
    CIEC_BYTE var_QUARTER_BYTE_02;
    CIEC_BYTE var_QUARTER_BYTE_03;

    CIEC_BYTE var_conn_QUARTER_BYTE_00;
    CIEC_BYTE var_conn_QUARTER_BYTE_01;
    CIEC_BYTE var_conn_QUARTER_BYTE_02;
    CIEC_BYTE var_conn_QUARTER_BYTE_03;

    CEventConnection conn_CNF;

    CDataConnection *conn_IN;

    CDataConnection conn_QUARTER_BYTE_00;
    CDataConnection conn_QUARTER_BYTE_01;
    CDataConnection conn_QUARTER_BYTE_02;
    CDataConnection conn_QUARTER_BYTE_03;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BYTE &paIN, CIEC_BYTE &paQUARTER_BYTE_00, CIEC_BYTE &paQUARTER_BYTE_01, CIEC_BYTE &paQUARTER_BYTE_02, CIEC_BYTE &paQUARTER_BYTE_03) {
      var_IN = paIN;
      executeEvent(scmEventREQID, nullptr);
      paQUARTER_BYTE_00 = var_QUARTER_BYTE_00;
      paQUARTER_BYTE_01 = var_QUARTER_BYTE_01;
      paQUARTER_BYTE_02 = var_QUARTER_BYTE_02;
      paQUARTER_BYTE_03 = var_QUARTER_BYTE_03;
    }

    void operator()(const CIEC_BYTE &paIN, CIEC_BYTE &paQUARTER_BYTE_00, CIEC_BYTE &paQUARTER_BYTE_01, CIEC_BYTE &paQUARTER_BYTE_02, CIEC_BYTE &paQUARTER_BYTE_03) {
      evt_REQ(paIN, paQUARTER_BYTE_00, paQUARTER_BYTE_01, paQUARTER_BYTE_02, paQUARTER_BYTE_03);
    }
};

void func_SPLIT_BYTE_INTO_QUARTERS(CIEC_BYTE st_lv_IN, CIEC_BYTE &st_lv_QUARTER_BYTE_00, CIEC_BYTE &st_lv_QUARTER_BYTE_01, CIEC_BYTE &st_lv_QUARTER_BYTE_02, CIEC_BYTE &st_lv_QUARTER_BYTE_03);


