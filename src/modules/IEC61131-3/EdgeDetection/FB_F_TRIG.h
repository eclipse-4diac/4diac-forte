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
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_FB_F_TRIG: public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_FB_F_TRIG)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  
  static const TEventID scm_nEventREQID = 0;
  
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  
  static const TEventID scm_nEventCNFID = 0;
  
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];
  

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  static const CStringDictionary::TStringId scm_anInternalsNames[];
  static const CStringDictionary::TStringId scm_anInternalsTypeIds[];
  static const SInternalVarsInformation scm_stInternalVars;
  
  CIEC_BOOL var_MEM;
  
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_REQ(void);

  void executeEvent(int pa_nEIID) override;

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_FB_F_TRIG(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);


  CIEC_BOOL var_CLK;
  CIEC_BOOL var_Q;
  
  CIEC_BOOL var_conn_Q;
  CEventConnection conn_CNF;
  CDataConnection *conn_CLK;
  CDataConnection conn_Q;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_BOOL &pa_CLK, CIEC_BOOL &pa_Q) {
    var_CLK = pa_CLK;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_Q = var_Q;
  }
  
  void operator()(const CIEC_BOOL &pa_CLK, CIEC_BOOL &pa_Q) {
    evt_REQ(pa_CLK, pa_Q);
  }
  
};



