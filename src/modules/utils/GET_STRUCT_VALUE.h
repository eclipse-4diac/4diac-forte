/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_any_variant.h"
#include "forte_string.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_GET_STRUCT_VALUE: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_GET_STRUCT_VALUE)

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

  CIEC_ANY *lookForMember(CIEC_STRUCT &paWhereToLook, char *paMemberName);

  void executeEvent(TEventID pa_nEIID);

  void readInputData(TEventID pa_nEIID) override;
  void writeOutputData(TEventID pa_nEIID) override;

public:
  FORTE_GET_STRUCT_VALUE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_ANY_VARIANT var_in_struct;
  CIEC_STRING var_member;
  CIEC_BOOL var_QO;
  CIEC_ANY_VARIANT var_output;
  
  CIEC_BOOL var_conn_QO;
  CIEC_ANY_VARIANT var_conn_output;
  CEventConnection conn_CNF;
  CDataConnection *conn_in_struct;
  CDataConnection *conn_member;
  CDataConnection conn_QO;
  CDataConnection conn_output;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_ANY_VARIANT &pa_in_struct, const CIEC_STRING &pa_member, CIEC_BOOL &pa_QO, CIEC_ANY_VARIANT &pa_output) {
    var_in_struct = pa_in_struct;
    var_member = pa_member;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_QO = var_QO;
    pa_output = var_output;
  }
  
  void operator()(const CIEC_ANY_VARIANT &pa_in_struct, const CIEC_STRING &pa_member, CIEC_BOOL &pa_QO, CIEC_ANY_VARIANT &pa_output) {
    evt_REQ(pa_in_struct, pa_member, pa_QO, pa_output);
  }
  
};



