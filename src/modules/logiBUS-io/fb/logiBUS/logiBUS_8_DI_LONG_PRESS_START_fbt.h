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
 *** Name: logiBUS_8_DI_LONG_PRESS_START
 *** Description: Button long press start --> IE
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

class FORTE_logiBUS_8_DI_LONG_PRESS_START final : public logiBUSIOFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_logiBUS_8_DI_LONG_PRESS_START)

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
  
  std::unique_ptr<forte::core::io::IOHandle> handle_I1;
  std::unique_ptr<forte::core::io::IOHandle> handle_I2;
  std::unique_ptr<forte::core::io::IOHandle> handle_I3;
  std::unique_ptr<forte::core::io::IOHandle> handle_I4;
  std::unique_ptr<forte::core::io::IOHandle> handle_I5;
  std::unique_ptr<forte::core::io::IOHandle> handle_I6;
  std::unique_ptr<forte::core::io::IOHandle> handle_I7;
  std::unique_ptr<forte::core::io::IOHandle> handle_I8;
  
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_logiBUS_8_DI_LONG_PRESS_START(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_DigitalInput_I1;
    CIEC_STRING var_DigitalInput_I2;
    CIEC_STRING var_DigitalInput_I3;
    CIEC_STRING var_DigitalInput_I4;
    CIEC_STRING var_DigitalInput_I5;
    CIEC_STRING var_DigitalInput_I6;
    CIEC_STRING var_DigitalInput_I7;
    CIEC_STRING var_DigitalInput_I8;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_DigitalInput_I1;
    CDataConnection *conn_DigitalInput_I2;
    CDataConnection *conn_DigitalInput_I3;
    CDataConnection *conn_DigitalInput_I4;
    CDataConnection *conn_DigitalInput_I5;
    CDataConnection *conn_DigitalInput_I6;
    CDataConnection *conn_DigitalInput_I7;
    CDataConnection *conn_DigitalInput_I8;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalInput_I1, const CIEC_STRING &paDigitalInput_I2, const CIEC_STRING &paDigitalInput_I3, const CIEC_STRING &paDigitalInput_I4, const CIEC_STRING &paDigitalInput_I5, const CIEC_STRING &paDigitalInput_I6, const CIEC_STRING &paDigitalInput_I7, const CIEC_STRING &paDigitalInput_I8, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_DigitalInput_I1 = paDigitalInput_I1;
      var_DigitalInput_I2 = paDigitalInput_I2;
      var_DigitalInput_I3 = paDigitalInput_I3;
      var_DigitalInput_I4 = paDigitalInput_I4;
      var_DigitalInput_I5 = paDigitalInput_I5;
      var_DigitalInput_I6 = paDigitalInput_I6;
      var_DigitalInput_I7 = paDigitalInput_I7;
      var_DigitalInput_I8 = paDigitalInput_I8;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalInput_I1, const CIEC_STRING &paDigitalInput_I2, const CIEC_STRING &paDigitalInput_I3, const CIEC_STRING &paDigitalInput_I4, const CIEC_STRING &paDigitalInput_I5, const CIEC_STRING &paDigitalInput_I6, const CIEC_STRING &paDigitalInput_I7, const CIEC_STRING &paDigitalInput_I8, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paDigitalInput_I1, paDigitalInput_I2, paDigitalInput_I3, paDigitalInput_I4, paDigitalInput_I5, paDigitalInput_I6, paDigitalInput_I7, paDigitalInput_I8, paQO, paSTATUS);
    }
};

