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
#include <random>


class FORTE_FB_RANDOM: public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_FB_RANDOM)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  
  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  
  static const TEventID scmEventINITID = 0;
  static const TEventID scmEventREQID = 1;
  
  static const TDataIOID scmEIWith[];
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];
  
  static const TEventID scmEventINITOID = 0;
  static const TEventID scmEventCNFID = 1;
  
  static const TDataIOID scmEOWith[]; 
  static const TForteInt16 scmEOWithIndexes[];
  static const CStringDictionary::TStringId scmEventOutputNames[];
  

  static const SFBInterfaceSpec scmFBInterfaceSpec;
  CIEC_ANY *getVarInternal(size_t) override;
  void alg_INIT(void);
  void alg_REQ(void);
  static const TForteInt16 scmStateSTART = 0;
  static const TForteInt16 scmStateREQ = 1;
  static const TForteInt16 scmStateState = 2;
  
  void enterStateSTART(void);
  void enterStateREQ(void);
  void enterStateState(void);

  void executeEvent(TEventID paEIID) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;

  std::random_device mRandomDevice;  // Will be used to obtain a seed for the random number engine
  std::mt19937 mRandomGenerator; // Standard mersenne_twister_engine 
  std::uniform_real_distribution<float> mDistribution;

public:
  FORTE_FB_RANDOM(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);


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
    receiveInputEvent(scmEventINITID, nullptr);
    pa_VAL = var_VAL;
  }
  void evt_REQ(const CIEC_UINT &pa_SEED, CIEC_REAL &pa_VAL) {
    var_SEED = pa_SEED;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_VAL = var_VAL;
  }
  
  void operator()(const CIEC_UINT &pa_SEED, CIEC_REAL &pa_VAL) {
    evt_INIT(pa_SEED, pa_VAL);
  }
  
};



