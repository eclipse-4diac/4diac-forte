/*******************************************************************************
 * Copyright (c) 2013, 2019 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Jose Cabral:
 *   - Fix calculateValueString to use template variable
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


class FORTE_F_NE: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_F_NE)

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

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;

public:
  FORTE_F_NE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

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
  
  void evt_REQ(const CIEC_ANY_ELEMENTARY &pa_IN1, const CIEC_ANY_ELEMENTARY &pa_IN2, CIEC_BOOL &pa_OUT) {
    var_IN1 = pa_IN1;
    var_IN2 = pa_IN2;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_OUT = var_OUT;
  }
  
  void operator()(const CIEC_ANY_ELEMENTARY &pa_IN1, const CIEC_ANY_ELEMENTARY &pa_IN2, CIEC_BOOL &pa_OUT) {
    evt_REQ(pa_IN1, pa_IN2, pa_OUT);
  }
  
};



