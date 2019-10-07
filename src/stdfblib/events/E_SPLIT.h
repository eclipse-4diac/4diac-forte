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
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_SPLIT_H_
#define _E_SPLIT_H_

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

#endif //_E_SPLIT_H_
