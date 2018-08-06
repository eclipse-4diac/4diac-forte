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
#ifndef _E_SELECT_H_
#define _E_SELECT_H_

#ifndef FMU

#include <funcbloc.h>

class E_SELECT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_SELECT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];

  static const TEventID scm_nEventEI0ID = 0;
  static const TEventID scm_nEventEI1ID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const CStringDictionary::TStringId scm_anEventOutputNames[];


  virtual void executeEvent(int pa_nEIID);

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,0,1, 0);
  
  CIEC_BOOL& G() {
     return *static_cast<CIEC_BOOL*>(getDI(0));
  }

public:
  FUNCTION_BLOCK_CTOR(E_SELECT){
  };
  virtual ~E_SELECT(){};

};

#else

#include <basicfb.h>
#include <forte_bool.h>

class E_SELECT: public CBasicFB{
  DECLARE_FIRMWARE_FB(E_SELECT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &G() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  static const TEventID scm_nEventEI0ID = 0;
  static const TEventID scm_nEventEI1ID = 1;
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
  static const TForteInt16 scm_nStateEI0 = 2;
  static const TForteInt16 scm_nStateEI1 = 3;

  void enterStateSTART(void);
  void enterStateEO(void);
  void enterStateEI0(void);
  void enterStateEI1(void);

  virtual void executeEvent(int pa_nEIID);

public:
  E_SELECT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~E_SELECT(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

