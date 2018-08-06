/*******************************************************************************
 * Copyright (c) 2007 - 2011 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_R_TRIG_H_
#define _E_R_TRIG_H_

#ifndef FMU

#include <funcbloc.h>

// cppcheck-suppress noConstructor
class E_R_TRIG: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(E_R_TRIG)

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

  void executeEvent(int pa_nEIID);

  CIEC_BOOL& QI() {
     return *static_cast<CIEC_BOOL*>(getDI(0));
  }

public:
  FUNCTION_BLOCK_CTOR(E_R_TRIG), m_bOldVal(false){
  };
  virtual ~E_R_TRIG(){};

};

#else

#include <cfb.h>
#include <typelib.h>
#include <forte_bool.h>

class E_R_TRIG: public CCompositeFB{
  DECLARE_FIRMWARE_FB(E_R_TRIG)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
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

   FORTE_FB_DATA_ARRAY(1, 1, 0, 0);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(E_R_TRIG){
  };

  virtual ~E_R_TRIG(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

