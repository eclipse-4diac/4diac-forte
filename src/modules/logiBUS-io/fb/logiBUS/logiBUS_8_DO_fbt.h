/*************************************************************************
 *** Copyright (c) 2018, 2025 HR Agrartechnik GmbH, Jose Cabral
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *** 
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: logiBUS_8_DO
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "logiBUSIOFunctionBlock.h"
#include "forte_bool.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_logiBUS_8_DO final : public logiBUSIOFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_logiBUS_8_DO)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const CStringDictionary::TStringId scmEventInputTypeIds[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
  
  std::unique_ptr<forte::core::io::IOHandle> handle_Q1;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q2;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q3;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q4;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q5;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q6;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q7;
  std::unique_ptr<forte::core::io::IOHandle> handle_Q8;
  
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_logiBUS_8_DO(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_DigitalOutput_Q1;
    CIEC_STRING var_DigitalOutput_Q2;
    CIEC_STRING var_DigitalOutput_Q3;
    CIEC_STRING var_DigitalOutput_Q4;
    CIEC_STRING var_DigitalOutput_Q5;
    CIEC_STRING var_DigitalOutput_Q6;
    CIEC_STRING var_DigitalOutput_Q7;
    CIEC_STRING var_DigitalOutput_Q8;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_DigitalOutput_Q1;
    CDataConnection *conn_DigitalOutput_Q2;
    CDataConnection *conn_DigitalOutput_Q3;
    CDataConnection *conn_DigitalOutput_Q4;
    CDataConnection *conn_DigitalOutput_Q5;
    CDataConnection *conn_DigitalOutput_Q6;
    CDataConnection *conn_DigitalOutput_Q7;
    CDataConnection *conn_DigitalOutput_Q8;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalOutput_Q1, const CIEC_STRING &paDigitalOutput_Q2, const CIEC_STRING &paDigitalOutput_Q3, const CIEC_STRING &paDigitalOutput_Q4, const CIEC_STRING &paDigitalOutput_Q5, const CIEC_STRING &paDigitalOutput_Q6, const CIEC_STRING &paDigitalOutput_Q7, const CIEC_STRING &paDigitalOutput_Q8, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_DigitalOutput_Q1 = paDigitalOutput_Q1;
      var_DigitalOutput_Q2 = paDigitalOutput_Q2;
      var_DigitalOutput_Q3 = paDigitalOutput_Q3;
      var_DigitalOutput_Q4 = paDigitalOutput_Q4;
      var_DigitalOutput_Q5 = paDigitalOutput_Q5;
      var_DigitalOutput_Q6 = paDigitalOutput_Q6;
      var_DigitalOutput_Q7 = paDigitalOutput_Q7;
      var_DigitalOutput_Q8 = paDigitalOutput_Q8;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalOutput_Q1, const CIEC_STRING &paDigitalOutput_Q2, const CIEC_STRING &paDigitalOutput_Q3, const CIEC_STRING &paDigitalOutput_Q4, const CIEC_STRING &paDigitalOutput_Q5, const CIEC_STRING &paDigitalOutput_Q6, const CIEC_STRING &paDigitalOutput_Q7, const CIEC_STRING &paDigitalOutput_Q8, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paDigitalOutput_Q1, paDigitalOutput_Q2, paDigitalOutput_Q3, paDigitalOutput_Q4, paDigitalOutput_Q5, paDigitalOutput_Q6, paDigitalOutput_Q7, paDigitalOutput_Q8, paQO, paSTATUS);
    }
};

