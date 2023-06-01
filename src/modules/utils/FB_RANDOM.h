/*******************************************************************************
 * Copyright (c) 2012 Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_uint.h"
#include "forte_real.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_FB_RANDOM: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_FB_RANDOM)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  
  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventREQID = 1;
  
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  
  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventCNFID = 1;
  
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];
  

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_INIT(void);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;
  static const TForteInt16 scm_nStateState = 2;
  
  void enterStateSTART(void);
  void enterStateREQ(void);
  void enterStateState(void);

  void executeEvent(TEventID pa_nEIID) override;

  void readInputData(TEventID pa_nEIID) override;
  void writeOutputData(TEventID pa_nEIID) override;

  unsigned int mSeedValue;

public:
  FORTE_FB_RANDOM(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);


  CIEC_UINT var_SEED;
  CIEC_REAL var_VAL;
  
  CIEC_REAL var_conn_VAL;
  CEventConnection conn_INITO;
  CEventConnection conn_CNF;
  CDataConnection *conn_SEED;
  CDataConnection conn_VAL;
  
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  
  void evt_INIT(const CIEC_UINT &pa_SEED, CIEC_REAL &pa_VAL) {
    var_SEED = pa_SEED;
    receiveInputEvent(scm_nEventINITID, nullptr);
    pa_VAL = var_VAL;
  }
  void evt_REQ(const CIEC_UINT &pa_SEED, CIEC_REAL &pa_VAL) {
    var_SEED = pa_SEED;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_VAL = var_VAL;
  }
  
  void operator()(const CIEC_UINT &pa_SEED, CIEC_REAL &pa_VAL) {
    evt_INIT(pa_SEED, pa_VAL);
  }
  
};



