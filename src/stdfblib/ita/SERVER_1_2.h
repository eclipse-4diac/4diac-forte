/*******************************************************************************
 * Copyright (c) 2012, 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SERVER_1_2_H_
#define SERVER_1_2_H_

#include "commfb.h"

/*!\brief implementation of server_1_2 block with static interface which is more
 * memory efficient on small platforms like LMS
 */

class SERVER_1_2 : public forte::com_infra::CCommFB{
  DECLARE_FIRMWARE_FB(SERVER_1_2)

  public:
    SERVER_1_2(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~SERVER_1_2();

    CIEC_BOOL& QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }

    CIEC_STRING& ID(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }

    CIEC_STRING& SD_1(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }

    CIEC_BOOL& QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }


    CIEC_STRING& RD_1(){
      return *static_cast<CIEC_STRING*>(getDO(2));
    }
    CIEC_STRING& RD_2(){
      return *static_cast<CIEC_STRING*>(getDO(3));
    }

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];
    static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];
    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventRSPID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventINDID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 3, 4, 0)
    ;

};

#endif /* SERVER_1_2_H_ */
