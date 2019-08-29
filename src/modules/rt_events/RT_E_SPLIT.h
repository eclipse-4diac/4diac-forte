/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RT_E_SPLIT_H_
#define _RT_E_SPLIT_H_

#include <funcbloc.h>
#include <ecet.h>

// cppcheck-suppress noConstructor
class FORTE_RT_E_SPLIT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_RT_E_SPLIT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_TIME &Tmin() {
    return *static_cast<CIEC_TIME*>(getDI(1));
  };

  CIEC_TIME &Deadline_EO1() {
    return *static_cast<CIEC_TIME*>(getDI(2));
  };

  CIEC_TIME &WCET_EO1() {
    return *static_cast<CIEC_TIME*>(getDI(3));
  };

  CIEC_TIME &Deadline_EO2() {
    return *static_cast<CIEC_TIME*>(getDI(4));
  };

  CIEC_TIME &WCET_EO2() {
    return *static_cast<CIEC_TIME*>(getDI(5));
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
  static const TEventID scm_nEventEO1ID = 1;
  static const TEventID scm_nEventEO2ID = 2;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(3, 6, 1, 0);

   CEventChainExecutionThread m_oECEO1, m_oECEO2;
   bool m_bInitialized;


  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_RT_E_SPLIT){
    m_bInitialized = false;
  };

  virtual ~FORTE_RT_E_SPLIT(){};

};

#endif //close the ifdef sequence from the beginning of the file

