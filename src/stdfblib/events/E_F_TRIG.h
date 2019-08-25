/*******************************************************************************
 * Copyright (c) 2007 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_F_TRIG_H_
#define _E_F_TRIG_H_

#include <funcbloc.h>

// cppcheck-suppress noConstructor
class E_F_TRIG: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_F_TRIG)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];

  static const TEventID scm_nEventEIID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventEOID = 0;
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,1,0, 0);

  bool m_bOldVal;


  virtual void executeEvent(int pa_nEIID);
  
  CIEC_BOOL& QI() {
     return *static_cast<CIEC_BOOL*>(getDI(0));
  }

public:
  FUNCTION_BLOCK_CTOR(E_F_TRIG), m_bOldVal(false) {
  };

  virtual ~E_F_TRIG(){};

};

#endif //_E_F_TRIG_H_
