/*******************************************************************************
 * Copyright (c) 2011 - 2019 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Jose Cabral:
 *   - Fix calculateValueString to use template variable
 *******************************************************************************/
#ifndef _F_SEL_H_
#define _F_SEL_H_

#include <funcbloc.h>

class FORTE_F_SEL: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_F_SEL)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &G() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_ANY &IN0() {
    return *static_cast<CIEC_ANY*>(getDI(1));
  };

  CIEC_ANY &IN1() {
    return *static_cast<CIEC_ANY*>(getDI(2));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY &st_OUT() {
    return *static_cast<CIEC_ANY*>(getDO(0));
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
  FUNCTION_BLOCK_CTOR(FORTE_F_SEL){
  };

  virtual ~FORTE_F_SEL(){};

  template<typename T> void calculateValue(){
    T oIn0;
    T oIn1;
    oIn0.saveAssign(IN0());
    oIn1.saveAssign(IN1());

    st_OUT().saveAssign(SEL<T> (G(), oIn0, oIn1));
  }

  template<typename T> void calculateValueString(){
    calculateValue<T>();
  }

};

#endif //close the ifdef sequence from the beginning of the file

