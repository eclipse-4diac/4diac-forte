/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_uint.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_CTUD: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_CTUD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventCUID = 0;
  static const TEventID scm_nEventCDID = 1;
  static const TEventID scm_nEventRID = 2;
  static const TEventID scm_nEventLDID = 3;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventCOID = 0;
  static const TEventID scm_nEventROID = 1;
  static const TEventID scm_nEventLDOID = 2;
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_CountUp(void);
  void alg_Reset(void);
  void alg_Load(void);
  void alg_UpdateQUQD(void);
  void alg_CountDown(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateCU = 1;
  static const TForteInt16 scm_nStateR = 2;
  static const TForteInt16 scm_nStateCD = 3;
  static const TForteInt16 scm_nStateLD = 4;
  
  void enterStateSTART(void);
  void enterStateCU(void);
  void enterStateR(void);
  void enterStateCD(void);
  void enterStateLD(void);

  void executeEvent(TEventID pa_nEIID) override;

  void readInputData(TEventID pa_nEIID) override;
  void writeOutputData(TEventID pa_nEIID) override;

public:
  FORTE_E_CTUD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_UINT var_PV;
  CIEC_BOOL var_QU;
  CIEC_BOOL var_QD;
  CIEC_UINT var_CV;
  CIEC_BOOL var_conn_QU;
  CIEC_BOOL var_conn_QD;
  CIEC_UINT var_conn_CV;
  CEventConnection conn_CO;
  CEventConnection conn_RO;
  CEventConnection conn_LDO;
  CDataConnection *conn_PV;
  CDataConnection conn_QU;
  CDataConnection conn_QD;
  CDataConnection conn_CV;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_CU(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventCUID, nullptr);
    pa_QU = var_QU;
    pa_QD = var_QD;
    pa_CV = var_CV;
  }
  void evt_CD(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventCDID, nullptr);
    pa_QU = var_QU;
    pa_QD = var_QD;
    pa_CV = var_CV;
  }
  void evt_R(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventRID, nullptr);
    pa_QU = var_QU;
    pa_QD = var_QD;
    pa_CV = var_CV;
  }
  void evt_LD(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventLDID, nullptr);
    pa_QU = var_QU;
    pa_QD = var_QD;
    pa_CV = var_CV;
  }
  void operator()(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_UINT &pa_CV) {
    evt_CU(pa_PV, pa_QU, pa_QD, pa_CV);
  }
};



