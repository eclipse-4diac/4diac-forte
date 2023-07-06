/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 *               2020 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ben Schneider - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - Changed to a full basic FB implementation utilizing the new
 *                   NOW_MONOTONIC function
 *******************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_STOPWATCH: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_STOPWATCH)

private:
  
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  
  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  
  static const TEventID scm_nEventEOID = 0;
  
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];
  

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  static const CStringDictionary::TStringId scm_anInternalsNames[];
  static const CStringDictionary::TStringId scm_anInternalsTypeIds[];
  static const SInternalVarsInformation scm_stInternalVars;
  
  CIEC_TIME var_startTime;
  
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_captureStartTime(void);
  void alg_calcDiff(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateMeasure = 1;
  static const TForteInt16 scm_nStateSTOP = 2;
  
  void enterStateSTART(void);
  void enterStateMeasure(void);
  void enterStateSTOP(void);

  void executeEvent(TEventID pa_nEIID) override;

  void readInputData(TEventID) override;
  void writeOutputData(TEventID pa_nEIID) override;

public:
  FORTE_E_STOPWATCH(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);


  CIEC_TIME var_TD;
  
  CIEC_TIME var_conn_TD;
  CEventConnection conn_EO;
  CDataConnection conn_TD;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_START(CIEC_TIME &pa_TD) {
    receiveInputEvent(scm_nEventSTARTID, nullptr);
    pa_TD = var_TD;
  }
  void evt_STOP(CIEC_TIME &pa_TD) {
    receiveInputEvent(scm_nEventSTOPID, nullptr);
    pa_TD = var_TD;
  }
  
  void operator()(CIEC_TIME &pa_TD) {
    evt_START(pa_TD);
  }
  
};



