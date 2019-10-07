/*******************************************************************************
 * Copyright (c) 2014 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_CTD_H_
#define _E_CTD_H_

#include <basicfb.h>

class FORTE_E_CTD: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_E_CTD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_UINT &PV() {
    return *static_cast<CIEC_UINT*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &Q() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_UINT &CV() {
    return *static_cast<CIEC_UINT*>(getDO(1));
  };

  static const TEventID scm_nEventCDID = 0;
  static const TEventID scm_nEventLDID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCDOID = 0;
  static const TEventID scm_nEventLDOID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(2, 1, 2, 0, 0);
  void alg_CD(void);
  void alg_LD(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateCU = 1;
  static const TForteInt16 scm_nStateLD = 2;

  void enterStateSTART(void);
  void enterStateCU(void);
  void enterStateLD(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_E_CTD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_E_CTD(){};

};

#endif //close the ifdef sequence from the beginning of the file

