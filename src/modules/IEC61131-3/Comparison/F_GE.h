/*******************************************************************************
 * Copyright (c) 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_any_elementary_variant.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_F_GE: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_F_GE)

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

  void executeEvent(int pa_nEIID);

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_F_GE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_ANY_ELEMENTARY_VARIANT var_IN1;
  CIEC_ANY_ELEMENTARY_VARIANT var_IN2;
  CIEC_BOOL var_OUT;
  
  CIEC_BOOL var_conn_OUT;
  CEventConnection conn_CNF;
  CDataConnection *conn_IN1;
  CDataConnection *conn_IN2;
  CDataConnection conn_OUT;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_ANY_ELEMENTARY_VARIANT &pa_IN1, const CIEC_ANY_ELEMENTARY_VARIANT &pa_IN2, CIEC_BOOL &pa_OUT) {
    var_IN1 = pa_IN1;
    var_IN2 = pa_IN2;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_OUT = var_OUT;
  }
  
  void operator()(const CIEC_ANY_ELEMENTARY_VARIANT &pa_IN1, const CIEC_ANY_ELEMENTARY_VARIANT &pa_IN2, CIEC_BOOL &pa_OUT) {
    evt_REQ(pa_IN1, pa_IN2, pa_OUT);
  }
  
};



