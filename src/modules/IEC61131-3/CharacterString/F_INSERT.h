/*******************************************************************************
 * Copyright (c) 2013 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _F_INSERT_H_
#define _F_INSERT_H_

#include <funcbloc.h>

class FORTE_F_INSERT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_F_INSERT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ANY_STRING &IN1() {
    return *static_cast<CIEC_ANY_STRING*>(getDI(0));
  };

  CIEC_ANY_STRING &IN2() {
    return *static_cast<CIEC_ANY_STRING*>(getDI(1));
  };

  CIEC_ANY_INT &P() {
    return *static_cast<CIEC_ANY_INT*>(getDI(2));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY_STRING &st_OUT() {
    return *static_cast<CIEC_ANY_STRING*>(getDO(0));
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

   FORTE_FB_DATA_ARRAY(1, 3, 1, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_F_INSERT){
  };

  template<typename T> void calculateValueString(){
     T oIn1;
     T oIn2;
     oIn1.saveAssign(IN1());
     oIn2.saveAssign(IN2());
     st_OUT().saveAssign(INSERT(oIn1,oIn2,P()));
   }

  virtual ~FORTE_F_INSERT(){};

};

#endif //close the ifdef sequence from the beginning of the file

