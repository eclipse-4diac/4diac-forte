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

#ifndef _ADS_SERVER_CONFIG_H_
#define _ADS_SERVER_CONFIG_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <forte_uint.h>
#include <forte_wstring.h>

class FORTE_ADS_SERVER_CONFIG : public CFunctionBlock{
    DECLARE_FIRMWARE_FB (FORTE_ADS_SERVER_CONFIG)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_STRING &FRIENDLY_NAME(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    };

    CIEC_STRING &SERVER_ADS_ADDRESS(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    };

    CIEC_UINT &ADS_PORT(){
      return *static_cast<CIEC_UINT*>(getDI(3));
    };

    CIEC_STRING &SERVER_IPV4_OR_HOSTNAME(){
      return *static_cast<CIEC_STRING*>(getDI(4));
    };

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS(){
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    static const TEventID scm_nEventINITID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(1, 5, 2, 0);

    void executeEvent(int pa_nEIID);

  public:
    FUNCTION_BLOCK_CTOR (FORTE_ADS_SERVER_CONFIG) { };

    virtual ~FORTE_ADS_SERVER_CONFIG(){
    };

};

#endif //close the ifdef sequence from the beginning of the file

