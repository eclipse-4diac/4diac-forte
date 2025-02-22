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
 *** Name: logiBUS_8_AI
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
#include "forte_dword.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_logiBUS_8_AI final : public logiBUSIOFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_logiBUS_8_AI)

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
  
  std::unique_ptr<forte::core::io::IOHandle> handle_AI1;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI2;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI3;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI4;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI5;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI6;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI7;
  std::unique_ptr<forte::core::io::IOHandle> handle_AI8;
  

  static bool isInit;
  static bool init();
  EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_logiBUS_8_AI(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_AnalogInput_I1;
    CIEC_DWORD var_AnalogInput_I1_hysteresis;
    CIEC_STRING var_AnalogInput_I2;
    CIEC_DWORD var_AnalogInput_I2_hysteresis;
    CIEC_STRING var_AnalogInput_I3;
    CIEC_DWORD var_AnalogInput_I3_hysteresis;
    CIEC_STRING var_AnalogInput_I4;
    CIEC_DWORD var_AnalogInput_I4_hysteresis;
    CIEC_STRING var_AnalogInput_I5;
    CIEC_DWORD var_AnalogInput_I5_hysteresis;
    CIEC_STRING var_AnalogInput_I6;
    CIEC_DWORD var_AnalogInput_I6_hysteresis;
    CIEC_STRING var_AnalogInput_I7;
    CIEC_DWORD var_AnalogInput_I7_hysteresis;
    CIEC_STRING var_AnalogInput_I8;
    CIEC_DWORD var_AnalogInput_I8_hysteresis;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_AnalogInput_I1;
    CDataConnection *conn_AnalogInput_I1_hysteresis;
    CDataConnection *conn_AnalogInput_I2;
    CDataConnection *conn_AnalogInput_I2_hysteresis;
    CDataConnection *conn_AnalogInput_I3;
    CDataConnection *conn_AnalogInput_I3_hysteresis;
    CDataConnection *conn_AnalogInput_I4;
    CDataConnection *conn_AnalogInput_I4_hysteresis;
    CDataConnection *conn_AnalogInput_I5;
    CDataConnection *conn_AnalogInput_I5_hysteresis;
    CDataConnection *conn_AnalogInput_I6;
    CDataConnection *conn_AnalogInput_I6_hysteresis;
    CDataConnection *conn_AnalogInput_I7;
    CDataConnection *conn_AnalogInput_I7_hysteresis;
    CDataConnection *conn_AnalogInput_I8;
    CDataConnection *conn_AnalogInput_I8_hysteresis;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paAnalogInput_I1, const CIEC_DWORD &paAnalogInput_I1_hysteresis, const CIEC_STRING &paAnalogInput_I2, const CIEC_DWORD &paAnalogInput_I2_hysteresis, const CIEC_STRING &paAnalogInput_I3, const CIEC_DWORD &paAnalogInput_I3_hysteresis, const CIEC_STRING &paAnalogInput_I4, const CIEC_DWORD &paAnalogInput_I4_hysteresis, const CIEC_STRING &paAnalogInput_I5, const CIEC_DWORD &paAnalogInput_I5_hysteresis, const CIEC_STRING &paAnalogInput_I6, const CIEC_DWORD &paAnalogInput_I6_hysteresis, const CIEC_STRING &paAnalogInput_I7, const CIEC_DWORD &paAnalogInput_I7_hysteresis, const CIEC_STRING &paAnalogInput_I8, const CIEC_DWORD &paAnalogInput_I8_hysteresis, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_AnalogInput_I1 = paAnalogInput_I1;
      var_AnalogInput_I1_hysteresis = paAnalogInput_I1_hysteresis;
      var_AnalogInput_I2 = paAnalogInput_I2;
      var_AnalogInput_I2_hysteresis = paAnalogInput_I2_hysteresis;
      var_AnalogInput_I3 = paAnalogInput_I3;
      var_AnalogInput_I3_hysteresis = paAnalogInput_I3_hysteresis;
      var_AnalogInput_I4 = paAnalogInput_I4;
      var_AnalogInput_I4_hysteresis = paAnalogInput_I4_hysteresis;
      var_AnalogInput_I5 = paAnalogInput_I5;
      var_AnalogInput_I5_hysteresis = paAnalogInput_I5_hysteresis;
      var_AnalogInput_I6 = paAnalogInput_I6;
      var_AnalogInput_I6_hysteresis = paAnalogInput_I6_hysteresis;
      var_AnalogInput_I7 = paAnalogInput_I7;
      var_AnalogInput_I7_hysteresis = paAnalogInput_I7_hysteresis;
      var_AnalogInput_I8 = paAnalogInput_I8;
      var_AnalogInput_I8_hysteresis = paAnalogInput_I8_hysteresis;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paAnalogInput_I1, const CIEC_DWORD &paAnalogInput_I1_hysteresis, const CIEC_STRING &paAnalogInput_I2, const CIEC_DWORD &paAnalogInput_I2_hysteresis, const CIEC_STRING &paAnalogInput_I3, const CIEC_DWORD &paAnalogInput_I3_hysteresis, const CIEC_STRING &paAnalogInput_I4, const CIEC_DWORD &paAnalogInput_I4_hysteresis, const CIEC_STRING &paAnalogInput_I5, const CIEC_DWORD &paAnalogInput_I5_hysteresis, const CIEC_STRING &paAnalogInput_I6, const CIEC_DWORD &paAnalogInput_I6_hysteresis, const CIEC_STRING &paAnalogInput_I7, const CIEC_DWORD &paAnalogInput_I7_hysteresis, const CIEC_STRING &paAnalogInput_I8, const CIEC_DWORD &paAnalogInput_I8_hysteresis, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paAnalogInput_I1, paAnalogInput_I1_hysteresis, paAnalogInput_I2, paAnalogInput_I2_hysteresis, paAnalogInput_I3, paAnalogInput_I3_hysteresis, paAnalogInput_I4, paAnalogInput_I4_hysteresis, paAnalogInput_I5, paAnalogInput_I5_hysteresis, paAnalogInput_I6, paAnalogInput_I6_hysteresis, paAnalogInput_I7, paAnalogInput_I7_hysteresis, paAnalogInput_I8, paAnalogInput_I8_hysteresis, paQO, paSTATUS);
    }
};

