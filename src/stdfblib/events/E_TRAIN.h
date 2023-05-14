/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_time.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_TRAIN: public CCompositeFB {
  DECLARE_FIRMWARE_FB(FORTE_E_TRAIN)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventEOID = 0;
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  static const SCFB_FBInstanceData scm_astInternalFBs[];
  static const SCFB_FBParameter scm_astParamters[];
  static const SCFB_FBConnectionData scm_astEventConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];
  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

  void readInputData(TEventID pa_nEIID) override;
  void writeOutputData(TEventID pa_nEIID) override;

public:
  FORTE_E_TRAIN(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_TIME var_DT;
  CIEC_UINT var_N;
  CIEC_UINT var_CV;
  CIEC_UINT var_conn_CV;
  CEventConnection conn_EO;
  CDataConnection *conn_DT;
  CDataConnection *conn_N;
  CDataConnection conn_CV;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_START(const CIEC_TIME &pa_DT, const CIEC_UINT &pa_N, CIEC_UINT &pa_CV) {
    var_DT = pa_DT;
    var_N = pa_N;
    receiveInputEvent(scm_nEventSTARTID, nullptr);
    pa_CV = var_CV;
  }
  void evt_STOP(const CIEC_TIME &pa_DT, const CIEC_UINT &pa_N, CIEC_UINT &pa_CV) {
    var_DT = pa_DT;
    var_N = pa_N;
    receiveInputEvent(scm_nEventSTOPID, nullptr);
    pa_CV = var_CV;
  }
  void operator()(const CIEC_TIME &pa_DT, const CIEC_UINT &pa_N, CIEC_UINT &pa_CV) {
    evt_START(pa_DT, pa_N, pa_CV);
  }
};



