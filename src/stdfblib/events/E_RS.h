/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_RS_H_
#define _E_RS_H_

#ifndef FMU

#include <funcbloc.h>

class E_RS: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_RS)

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];

  static const TEventID scm_nEventRID = 0;
  static const TEventID scm_nEventSID = 1;
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

 
  virtual void executeEvent(int pa_nEIID);

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,0,1, 0);

  CIEC_BOOL& Q() {
   	return *static_cast<CIEC_BOOL*>(getDO(0));
  }

public:
  FUNCTION_BLOCK_CTOR(E_RS){
  };
  virtual ~E_RS() {};

};

#else

#include <basicfb.h>
#include <forte_bool.h>

class E_RS: public CBasicFB{
  DECLARE_FIRMWARE_FB(E_RS)

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &Q() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  static const TEventID scm_nEventRID = 0;
  static const TEventID scm_nEventSID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 0, 1, 0, 0);
  void alg_SET(void);
  void alg_RESET(void);
  static const TForteInt16 scm_nStateQ0 = 0;
  static const TForteInt16 scm_nStateRESET = 1;
  static const TForteInt16 scm_nStateSET = 2;

  void enterStateQ0(void);
  void enterStateRESET(void);
  void enterStateSET(void);

  virtual void executeEvent(int pa_nEIID);

public:
  E_RS(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~E_RS(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

