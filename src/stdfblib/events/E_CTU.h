/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
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


class FORTE_E_CTU: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_CTU)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventCUID = 0;
  static const TEventID scm_nEventRID = 1;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventCUOID = 0;
  static const TEventID scm_nEventROID = 1;
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_R(void);
  void alg_CU(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateCU = 1;
  static const TForteInt16 scm_nStateR = 2;
  
  void enterStateSTART(void);
  void enterStateCU(void);
  void enterStateR(void);

  void executeEvent(int pa_nEIID) override;

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_E_CTU(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_UINT var_PV;
  CIEC_BOOL var_Q;
  CIEC_UINT var_CV;
  CIEC_BOOL var_conn_Q;
  CIEC_UINT var_conn_CV;
  CEventConnection conn_CUO;
  CEventConnection conn_RO;
  CDataConnection *conn_PV;
  CDataConnection conn_Q;
  CDataConnection conn_CV;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_CU(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventCUID, nullptr);
    pa_Q = var_Q;
    pa_CV = var_CV;
  }
  void evt_R(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    var_PV = pa_PV;
    receiveInputEvent(scm_nEventRID, nullptr);
    pa_Q = var_Q;
    pa_CV = var_CV;
  }
  void operator()(const CIEC_UINT &pa_PV, CIEC_BOOL &pa_Q, CIEC_UINT &pa_CV) {
    evt_CU(pa_PV, pa_Q, pa_CV);
  }
};



