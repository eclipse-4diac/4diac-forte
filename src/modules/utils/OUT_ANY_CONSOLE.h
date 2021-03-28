/*******************************************************************************
 * Copyright (c) 2011 - 2014 AIT, ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Monika Wenger, Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _OUT_ANY_CONSOLE_H_
#define _OUT_ANY_CONSOLE_H_

#include <funcbloc.h>

class FORTE_OUT_ANY_CONSOLE: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_OUT_ANY_CONSOLE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }
  ;

  CIEC_STRING &LABEL() {
    return *static_cast<CIEC_STRING*>(getDI(1));
  }
  ;

  CIEC_ANY &st_IN() {
    return *static_cast<CIEC_ANY*>(getDI(2));
  }
  ;

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }
  ;

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  //maximum string buffer size for conversions into string values
  static const TForteInt16 scm_maxStringBufSize;

  FORTE_FB_DATA_ARRAY(1, 3, 1, 0)
    ;

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_OUT_ANY_CONSOLE){
  };

  virtual ~FORTE_OUT_ANY_CONSOLE() {};

};

#endif //close the ifdef sequence from the beginning of the file
