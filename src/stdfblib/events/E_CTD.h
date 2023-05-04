/*******************************************************************************
 * Copyright (c) 2014 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
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


class FORTE_E_CTD: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_CTD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventCDID = 0;
  static const TEventID scm_nEventLDID = 1;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventCDOID = 0;
  static const TEventID scm_nEventLDOID = 1;
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_CD(void);
  void alg_LOAD(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateCD = 1;
  static const TForteInt16 scm_nStateLD = 2;
  
  void enterStateSTART(void);
  void enterStateCD(void);
  void enterStateLD(void);

  void executeEvent(int pa_nEIID) override;

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_E_CTD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_UINT var_PV;
  CIEC_BOOL var_Q;
  CIEC_UINT var_CV;
  CIEC_BOOL var_conn_Q;
  CIEC_UINT var_conn_CV;
  CEventConnection conn_CDO;
  CEventConnection conn_LDO;
  CDataConnection *conn_PV;
  CDataConnection conn_Q;
  CDataConnection conn_CV;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_CD(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventCDID, nullptr);
    pa_Q = var_Q;
    pa_CV = var_CV;
  }
  void evt_LD(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventLDID, nullptr);
    pa_Q = var_Q;
    pa_CV = var_CV;
  }
  void operator()(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    evt_CD(pa_PV, pa_Q, pa_CV);
  }
};



