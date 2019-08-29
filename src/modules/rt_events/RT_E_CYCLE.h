/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RT_E_CYCLE_H_
#define _RT_E_CYCLE_H_

#include <esfb.h>
#include <ecet.h>
#include "../arch/timerha.h"

class FORTE_RT_E_CYCLE: public CEventSourceFB{
  DECLARE_FIRMWARE_FB(FORTE_RT_E_CYCLE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_TIME &DT() {
    return *static_cast<CIEC_TIME*>(getDI(0));
  };

  CIEC_TIME &Deadline() {
    return *static_cast<CIEC_TIME*>(getDI(1));
  };

  CIEC_TIME &WCET() {
    return *static_cast<CIEC_TIME*>(getDI(2));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  static const TEventID scm_nEventSTARTID = 0;
  static const TEventID scm_nEventSTOPID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 3, 1, 0);

  bool mActive; //!> flag to indicate that the timed fb is currently active
  STimedFBListEntry mTimeListEntry;
  CEventChainExecutionThread mECEO;

  void executeEvent(int paEIID);

public:
  FORTE_RT_E_CYCLE(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

  virtual ~FORTE_RT_E_CYCLE(){};

};

#endif //close the ifdef sequence from the beginning of the file

