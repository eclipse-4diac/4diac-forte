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

#ifndef _F_LWORD_BCD_TO_ULINT_H_
#define _F_LWORD_BCD_TO_ULINT_H_

#include <funcbloc.h>
#include <forte_lword.h>
#include <forte_ulint.h>

class FORTE_F_LWORD_BCD_TO_ULINT: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_F_LWORD_BCD_TO_ULINT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_LWORD &IN() {
    return *static_cast<CIEC_LWORD*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ULINT &OUT() {
    return *static_cast<CIEC_ULINT*>(getDO(0));
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

   FORTE_FB_DATA_ARRAY(1, 1, 1, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_F_LWORD_BCD_TO_ULINT){
  };

  virtual ~FORTE_F_LWORD_BCD_TO_ULINT(){};

};

#endif //close the ifdef sequence from the beginning of the file

