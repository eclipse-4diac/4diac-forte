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
#ifndef _E_REND_H_
#define _E_REND_H_

#include <funcbloc.h>

// cppcheck-suppress noConstructor
class E_REND: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_REND)

private:

  static const TEventID scm_nEventEI1ID = 0;
  static const TEventID scm_nEventEI2ID = 1;
  static const TEventID scm_nEventRID = 2;
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const int scm_nStateSTART = 0;
  static const int scm_nStateR = 1;
  static const int scm_nStateR1 = 2;
  static const int scm_nStateEI1 = 3;
  static const int scm_nStateEO = 4;
  static const int scm_nStateEI2 = 5;
  static const int scm_nStateR2 = 6;

  bool m_bE1Occ;
  bool m_bE2Occ;


  virtual void executeEvent(int pa_nEIID);

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,0,0, 0);

public:
  FUNCTION_BLOCK_CTOR(E_REND),
    m_bE1Occ(false),
    m_bE2Occ(false){
  };
  virtual ~E_REND() {};

};

#endif //_E_REND_H_
