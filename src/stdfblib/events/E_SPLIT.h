/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_SPLIT_H_
#define _E_SPLIT_H_

#ifndef FMU

#include <funcbloc.h>

class E_SPLIT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_SPLIT)

private:
  static const TEventID scm_nEventEIID = 0;
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEO1ID = 0;
  static const TEventID scm_nEventEO2ID = 1;
  static const CStringDictionary::TStringId scm_anEventOutputNames[];


  virtual void executeEvent(int pa_nEIID){
    if(scm_nEventEIID == pa_nEIID){
      sendOutputEvent( scm_nEventEO1ID);
      sendOutputEvent( scm_nEventEO2ID);
    }
  };

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(2,0,0, 0);

public:
  FUNCTION_BLOCK_CTOR(E_SPLIT){
  };
  virtual ~E_SPLIT(){};

};

#else

#include <basicfb.h>

class E_SPLIT: public CBasicFB{
  DECLARE_FIRMWARE_FB(E_SPLIT)

private:
  static const TEventID scm_nEventEIID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEO1ID = 0;
  static const TEventID scm_nEventEO2ID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(2, 0, 0, 0, 0);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateEO = 1;

  void enterStateSTART(void);
  void enterStateEO(void);

  virtual void executeEvent(int pa_nEIID);

public:
  E_SPLIT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~E_SPLIT(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

