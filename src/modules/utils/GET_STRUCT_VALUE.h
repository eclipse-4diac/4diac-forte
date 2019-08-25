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

#ifndef _GET_STRUCT_VALUE_H_
#define _GET_STRUCT_VALUE_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <forte_any.h>

class FORTE_GET_STRUCT_VALUE : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_GET_STRUCT_VALUE)

  public:
    FUNCTION_BLOCK_CTOR(FORTE_GET_STRUCT_VALUE){};

    virtual ~FORTE_GET_STRUCT_VALUE() {};

  private:

    CIEC_ANY &in_struct() {
      return *static_cast<CIEC_ANY*>(getDI(0));
    };

    CIEC_STRING &member() {
      return *static_cast<CIEC_STRING*>(getDI(1));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_ANY &output() {
      return *static_cast<CIEC_ANY*>(getDO(1));
    };

    void executeEvent(int pa_nEIID);

    CIEC_ANY* getMemberFromName(CIEC_STRUCT* paWhereToLook, char* paMemberName);

    CIEC_ANY* lookForMember(CIEC_STRUCT* paWhereToLook, char* paMemberName);

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    static const TEventID scm_nEventREQID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];
    static const TEventID scm_nEventCNFID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(1, 2, 2, 0);
};

#endif //_GET_STRUCT_VALUE_H_

