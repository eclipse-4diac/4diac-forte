/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - initial contribution
 *******************************************************************************/

#ifndef _SET_LOCAL_ADS_ADDRESS_H_
#define _SET_LOCAL_ADS_ADDRESS_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <forte_wstring.h>

class FORTE_SET_LOCAL_ADS_ADDRESS : public CFunctionBlock{
    DECLARE_FIRMWARE_FB (FORTE_SET_LOCAL_ADS_ADDRESS)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_STRING &PARAMS(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_WSTRING &STATUS(){
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    }
    ;

    CIEC_STRING &LOCAL_ADS_ADDRESS(){
      return *static_cast<CIEC_STRING*>(getDO(2));
    }
    ;

    static const TEventID scm_nEventINITID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(1, 2, 3, 0);

    void executeEvent(int pa_nEIID);

  public:
    FUNCTION_BLOCK_CTOR (FORTE_SET_LOCAL_ADS_ADDRESS) { };

    virtual ~FORTE_SET_LOCAL_ADS_ADDRESS(){
    }
    ;

};

#endif //close the ifdef sequence from the beginning of the file

