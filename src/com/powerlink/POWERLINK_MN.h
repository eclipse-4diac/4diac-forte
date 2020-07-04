/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _POWERLINK_MN_H_
#define _POWERLINK_MN_H_

#include <funcbloc.h>

class FORTE_POWERLINK_MN : public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_POWERLINK_MN)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_STRING &CDC_CFG(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }
    ;

    CIEC_STRING &APP_CFG(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }
    ;

    CIEC_STRING &DEV_NAME(){
      return *static_cast<CIEC_STRING*>(getDI(3));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
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

    FORTE_FB_DATA_ARRAY(1, 4, 2, 0)
    ;

    void executeEvent(int pa_nEIID);

    void shutdownStack();

  public:
    FUNCTION_BLOCK_CTOR(FORTE_POWERLINK_MN){
  };

  virtual ~FORTE_POWERLINK_MN();

};

#endif //close the ifdef sequence from the beginning of the file
