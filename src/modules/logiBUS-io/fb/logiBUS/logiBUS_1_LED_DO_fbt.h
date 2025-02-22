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
 *** Name: logiBUS_1_LED_DO
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
#include "forte_usint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_logiBUS_1_LED_DO final : public logiBUSIOFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_logiBUS_1_LED_DO)

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
  
  std::unique_ptr<forte::core::io::IOHandle> handle_LED_5HZ;
  std::unique_ptr<forte::core::io::IOHandle> handle_LED_2HZ;
  std::unique_ptr<forte::core::io::IOHandle> handle_LED_1HZ;
  std::unique_ptr<forte::core::io::IOHandle> handle_LED_05HZ;
  std::unique_ptr<forte::core::io::IOHandle> handle_LED_ON;
  
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_logiBUS_1_LED_DO(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_Output;
    CIEC_STRING var_LED_5HZ;
    CIEC_STRING var_LED_2HZ;
    CIEC_STRING var_LED_1HZ;
    CIEC_STRING var_LED_05HZ;
    CIEC_STRING var_LED_ON;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_Output;
    CDataConnection *conn_LED_5HZ;
    CDataConnection *conn_LED_2HZ;
    CDataConnection *conn_LED_1HZ;
    CDataConnection *conn_LED_05HZ;
    CDataConnection *conn_LED_ON;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_USINT &paOutput, const CIEC_STRING &paLED_5HZ, const CIEC_STRING &paLED_2HZ, const CIEC_STRING &paLED_1HZ, const CIEC_STRING &paLED_05HZ, const CIEC_STRING &paLED_ON, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_Output = paOutput;
      var_LED_5HZ = paLED_5HZ;
      var_LED_2HZ = paLED_2HZ;
      var_LED_1HZ = paLED_1HZ;
      var_LED_05HZ = paLED_05HZ;
      var_LED_ON = paLED_ON;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_USINT &paOutput, const CIEC_STRING &paLED_5HZ, const CIEC_STRING &paLED_2HZ, const CIEC_STRING &paLED_1HZ, const CIEC_STRING &paLED_05HZ, const CIEC_STRING &paLED_ON, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paOutput, paLED_5HZ, paLED_2HZ, paLED_1HZ, paLED_05HZ, paLED_ON, paQO, paSTATUS);
    }
};

