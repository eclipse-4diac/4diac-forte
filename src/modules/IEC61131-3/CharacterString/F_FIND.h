/*******************************************************************************
 * Copyright (c) 2013 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _F_FIND_H_
#define _F_FIND_H_

#include <funcbloc.h>
#include <forte_any_int.h>
#include <forte_any_string.h>

class FORTE_F_FIND: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_F_FIND)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ANY_STRING &IN1() {
    return *static_cast<CIEC_ANY_STRING*>(getDI(0));
  };

  CIEC_ANY_STRING &IN2() {
    return *static_cast<CIEC_ANY_STRING*>(getDI(1));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY_INT &OUT() {
    return *static_cast<CIEC_ANY_INT*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 2, 1, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_F_FIND){
  };

  template<typename T> void calculateValueString(){
     T oIn1;
     oIn1.saveAssign(IN1());
     T oIn2;
     oIn2.saveAssign(IN2());
     OUT().saveAssign(FIND(oIn1,oIn2));
   }

  virtual ~FORTE_F_FIND(){};

};

#endif //close the ifdef sequence from the beginning of the file

