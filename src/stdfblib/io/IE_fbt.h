/*******************************************************************************
 * Copyright (c) 2014, 2023 fortiss GmbH, HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger - initial API and implementation and/or initial documentation
 *   Franz Hoepfinger - copied over IX to IE, and removed the IN
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "processinterface.h"

class FORTE_IE final : public CProcessInterface {
  DECLARE_FIRMWARE_FB(FORTE_IE)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  static const TEventID scmEventINITID = 0;
  static const TEventID scmEventREQID = 1;
  static const TDataIOID scmEIWith[];
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];
  static const TEventID scmEventINITOID = 0;
  static const TEventID scmEventCNFID = 1;
  static const TEventID scmEventINDID = 2;
  static const TDataIOID scmEOWith[];
  static const TForteInt16 scmEOWithIndexes[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_IE(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

  CIEC_BOOL var_QI;
  CIEC_STRING var_PARAMS;
  CIEC_BOOL var_QO;
  CIEC_STRING var_STATUS;
  CIEC_BOOL var_conn_QO;
  CIEC_STRING var_conn_STATUS;
  CEventConnection conn_INITO;
  CEventConnection conn_CNF;
  CEventConnection conn_IND;
  CDataConnection *conn_QI;
  CDataConnection *conn_PARAMS;
  CDataConnection conn_QO;
  CDataConnection conn_STATUS;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    receiveInputEvent(scmEventINITID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void evt_REQ(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    evt_INIT(pa_QI, pa_PARAMS, pa_QO, pa_STATUS);
  }
};



