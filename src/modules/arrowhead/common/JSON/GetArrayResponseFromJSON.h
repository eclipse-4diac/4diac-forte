/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _GETARRAYRESPONSEFROMJSON_H_
#define _GETARRAYRESPONSEFROMJSON_H_

#include <funcbloc.h>
#include <forte_string.h>
#include <forte_any.h>

class FORTE_GetArrayResponseFromJSON: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_GetArrayResponseFromJSON)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_STRING &response() {
    return *static_cast<CIEC_STRING*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY &output() {
    return *static_cast<CIEC_ANY*>(getDO(0));
  };

  CIEC_ARRAY &output_Array() {
    return *static_cast<CIEC_ARRAY*>(getDO(0));
  }

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

  bool isResponseEmpty(char* paText);


public:
  FUNCTION_BLOCK_CTOR(FORTE_GetArrayResponseFromJSON){
  };

  virtual ~FORTE_GetArrayResponseFromJSON(){};

};

#endif //close the ifdef sequence from the beginning of the file

