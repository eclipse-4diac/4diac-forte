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
#ifndef _RT_E_DEMUX_H_
#define _RT_E_DEMUX_H_

#include <funcbloc.h>
#include <ecet.h>

class FORTE_RT_E_DEMUX: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_RT_E_DEMUX)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_UINT &K() {
    return *static_cast<CIEC_UINT*>(getDI(1));
  };

  CIEC_TIME &Tmin() {
    return *static_cast<CIEC_TIME*>(getDI(2));
  };

  CIEC_TIME &Deadline0() {
    return *static_cast<CIEC_TIME*>(getDI(3));
  };

  CIEC_TIME &WCET0() {
    return *static_cast<CIEC_TIME*>(getDI(4));
  };

  CIEC_TIME &Deadline1() {
    return *static_cast<CIEC_TIME*>(getDI(5));
  };

  CIEC_TIME &WCET1() {
    return *static_cast<CIEC_TIME*>(getDI(6));
  };

  CIEC_TIME &Deadline2() {
    return *static_cast<CIEC_TIME*>(getDI(7));
  };

  CIEC_TIME &WCET2() {
    return *static_cast<CIEC_TIME*>(getDI(8));
  };

  CIEC_TIME &Deadline3() {
    return *static_cast<CIEC_TIME*>(getDI(9));
  };

  CIEC_TIME &WCET3() {
    return *static_cast<CIEC_TIME*>(getDI(10));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventEIID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventEO0ID = 1;
  static const TEventID scm_nEventEO1ID = 2;
  static const TEventID scm_nEventEO2ID = 3;
  static const TEventID scm_nEventEO3ID = 4;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(5, 11, 1, 0);

   
  CEventChainExecutionThread m_oECEO0;
  CEventChainExecutionThread m_oECEO1;
  CEventChainExecutionThread m_oECEO2;
  CEventChainExecutionThread m_oECEO3;
  bool m_bInitialized;

  void executeEvent(int pa_nEIID);

public:
  FORTE_RT_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  virtual ~FORTE_RT_E_DEMUX(){};

};

#endif //close the ifdef sequence from the beginning of the file

