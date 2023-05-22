/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_any_variant.h"
#include "forte_uint.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_GET_AT_INDEX: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_GET_AT_INDEX)

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

  void executeEvent(TEventID pa_nEIID);

  void readInputData(TEventID pa_nEIID) override;
  void writeOutputData(TEventID pa_nEIID) override;

public:
  FORTE_GET_AT_INDEX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_ANY_VARIANT var_IN_ARRAY;
  CIEC_UINT var_INDEX;
  CIEC_BOOL var_QO;
  CIEC_ANY_VARIANT var_OUT;
  
  CIEC_BOOL var_conn_QO;
  CIEC_ANY_VARIANT var_conn_OUT;
  CEventConnection conn_CNF;
  CDataConnection *conn_IN_ARRAY;
  CDataConnection *conn_INDEX;
  CDataConnection conn_QO;
  CDataConnection conn_OUT;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_ANY_VARIANT &pa_IN_ARRAY, const CIEC_UINT &pa_INDEX, CIEC_BOOL &pa_QO, CIEC_ANY_VARIANT &pa_OUT) {
    var_IN_ARRAY = pa_IN_ARRAY;
    var_INDEX = pa_INDEX;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_QO = var_QO;
    pa_OUT = var_OUT;
  }
  
  void operator()(const CIEC_ANY_VARIANT &pa_IN_ARRAY, const CIEC_UINT &pa_INDEX, CIEC_BOOL &pa_QO, CIEC_ANY_VARIANT &pa_OUT) {
    evt_REQ(pa_IN_ARRAY, pa_INDEX, pa_QO, pa_OUT);
  }
  
};



