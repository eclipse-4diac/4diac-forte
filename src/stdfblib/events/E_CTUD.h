/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_CTUD_H_
#define _E_CTUD_H_

#include <basicfb.h>

class FORTE_E_CTUD: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_E_CTUD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_UINT &PV() {
    return *static_cast<CIEC_UINT*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QU() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_BOOL &QD() {
    return *static_cast<CIEC_BOOL*>(getDO(1));
  };

  CIEC_UINT &CV() {
    return *static_cast<CIEC_UINT*>(getDO(2));
  };

  static const TEventID scm_nEventCUID = 0;
  static const TEventID scm_nEventCDID = 1;
  static const TEventID scm_nEventRID = 2;
  static const TEventID scm_nEventLDID = 3;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCOID = 0;
  static const TEventID scm_nEventROID = 1;
  static const TEventID scm_nEventLDOID = 2;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(3, 1, 3, 0, 0);
  void alg_CountUp(void);
  void alg_Reset(void);
  void alg_Load(void);
  void alg_UpdateQUQD(void);
  void alg_CountDown(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateCU = 1;
  static const TForteInt16 scm_nStateR = 2;
  static const TForteInt16 scm_nStateCD = 3;
  static const TForteInt16 scm_nStateLD = 4;

  void enterStateSTART(void);
  void enterStateCU(void);
  void enterStateR(void);
  void enterStateCD(void);
  void enterStateLD(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_E_CTUD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_E_CTUD(){};

};

#endif //close the ifdef sequence from the beginning of the file

