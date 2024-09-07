/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "simplefb.h"
#include "forte_bool.h"
#include "forte_lint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_FB_CTUD_LINT: public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_FB_CTUD_LINT)

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
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_REQ(void);

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;

public:
  FORTE_FB_CTUD_LINT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);


  CIEC_BOOL var_CU;
  CIEC_BOOL var_CD;
  CIEC_BOOL var_R;
  CIEC_BOOL var_LD;
  CIEC_LINT var_PV;
  CIEC_BOOL var_QU;
  CIEC_BOOL var_QD;
  CIEC_LINT var_CV;
  
  CIEC_BOOL var_conn_QU;
  CIEC_BOOL var_conn_QD;
  CIEC_LINT var_conn_CV;
  CEventConnection conn_CNF;
  CDataConnection *conn_CU;
  CDataConnection *conn_CD;
  CDataConnection *conn_R;
  CDataConnection *conn_LD;
  CDataConnection *conn_PV;
  CDataConnection conn_QU;
  CDataConnection conn_QD;
  CDataConnection conn_CV;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_BOOL &pa_CU, const CIEC_BOOL &pa_CD, const CIEC_BOOL &pa_R, const CIEC_BOOL &pa_LD, const CIEC_LINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_LINT &pa_CV) {
    var_CU = pa_CU;
    var_CD = pa_CD;
    var_R = pa_R;
    var_LD = pa_LD;
    var_PV = pa_PV;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_QU = var_QU;
    pa_QD = var_QD;
    pa_CV = var_CV;
  }
  
  void operator()(const CIEC_BOOL &pa_CU, const CIEC_BOOL &pa_CD, const CIEC_BOOL &pa_R, const CIEC_BOOL &pa_LD, const CIEC_LINT &pa_PV, CIEC_BOOL &pa_QU, CIEC_BOOL &pa_QD, CIEC_LINT &pa_CV) {
    evt_REQ(pa_CU, pa_CD, pa_R, pa_LD, pa_PV, pa_QU, pa_QD, pa_CV);
  }
  
};



