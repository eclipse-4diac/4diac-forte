/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ben Schneider
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _E_STOPWATCH_H_
#define _E_STOPWATCH_H_

#include <basicfb.h>
#include <forte_time.h>

#include <chrono>
using namespace std;

class FORTE_E_STOPWATCH: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_E_STOPWATCH)

private:
  chrono::time_point <chrono::high_resolution_clock> m_begin, m_end;
  int mCurrentTime = 0;

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_TIME &TD() {
    return *static_cast<CIEC_TIME*>(getDO(0));
  };

  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_BASIC_FB_DATA_ARRAY(1, 0, 1, 0, 0);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateMeasure = 1;
  static const TForteInt16 scm_nStateStopped = 2;

  void enterStateSTART(void);
  void enterStateMeasure(void);
  void enterStateStopped(void);

  virtual void executeEvent(int pa_nEIID);

  void writeElapsedTimeToTD(void);

public:
  FORTE_E_STOPWATCH(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
        0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_E_STOPWATCH(){};

};

#endif //close the ifdef sequence from the beginning of the file
