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
#include "forte_string.h"
#include "forte_real.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_F_STRING_AS_REAL: public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_F_STRING_AS_REAL)

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
  FORTE_F_STRING_AS_REAL(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);


  CIEC_STRING var_IN;
  CIEC_REAL var_OUT;
  
  CIEC_REAL var_conn_OUT;
  CEventConnection conn_CNF;
  CDataConnection *conn_IN;
  CDataConnection conn_OUT;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_REQ(const CIEC_STRING &pa_IN, CIEC_REAL &pa_OUT) {
    var_IN = pa_IN;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_OUT = var_OUT;
  }
  
  void operator()(const CIEC_STRING &pa_IN, CIEC_REAL &pa_OUT) {
    evt_REQ(pa_IN, pa_OUT);
  }
  
};



