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

#ifndef _E_STOPWATCH_H_
#define _E_STOPWATCH_H_

#include "basicfb.h"
#include "forte_time.h"
#include "forte_array_at.h"


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

  CIEC_TIME &st_TD() {
    return *static_cast<CIEC_TIME*>(getDO(0));
  }
  
  CIEC_TIME &st_startTime() {
    return *static_cast<CIEC_TIME*>(getVarInternal(0));
  }
  

  void alg_captureStartTime(void);
  void alg_calcDiff(void);

  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateMeasure = 1;
  static const TForteInt16 scm_nStateSTOP = 2;
  
  void enterStateSTART(void);
  void enterStateMeasure(void);
  void enterStateSTOP(void);

  virtual void executeEvent(int pa_nEIID);

  FORTE_BASIC_FB_DATA_ARRAY(1, 0, 1, 1, 0);

public:
  FORTE_E_STOPWATCH(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stInternalVars, m_anFBConnData, m_anFBVarsData) {
  };

  virtual ~FORTE_E_STOPWATCH() {};
};

#endif // _E_STOPWATCH_H_


