/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_PERMIT_H_
#define _E_PERMIT_H_

#ifndef FMU

#include <funcbloc.h>

class E_PERMIT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_PERMIT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];

  static const TEventID scm_nEventEIID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const CStringDictionary::TStringId scm_anEventOutputNames[];


  virtual void executeEvent(int pa_nEIID);

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,1,0, 0);
  
  CIEC_BOOL& PERMIT() {
   	return *static_cast<CIEC_BOOL*>(getDI(0));
  }

public:
  FUNCTION_BLOCK_CTOR(E_PERMIT) {
  };
  virtual ~E_PERMIT(){};

};

#else

#include <basicfb.h>
#include <forte_bool.h>

class E_PERMIT: public CBasicFB{
  DECLARE_FIRMWARE_FB(E_PERMIT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &PERMIT() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  static const TEventID scm_nEventEIID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 1, 0, 0, 0);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateEO = 1;
  static const TForteInt16 scm_nStateNULL = 2;

  void enterStateSTART(void);
  void enterStateEO(void);
  void enterStateNULL(void);

  virtual void executeEvent(int pa_nEIID);

public:
  E_PERMIT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~E_PERMIT(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

