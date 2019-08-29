/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#ifndef _TRIGGER_SERVICE_SERVER_H_
#define _TRIGGER_SERVICE_SERVER_H_

#include <esfb.h>
#include <forte_bool.h>
#include <forte_string.h>

#include <std_srvs/Trigger.h>
#include <string>

// cppcheck-suppress noConstructor
class FORTE_TRIGGER_SERVICE_SERVER : public CEventSourceFB{
  DECLARE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_SERVER)

  private:

    bool m_Initiated;
    ros::NodeHandle* m_nh;
    ros::ServiceServer m_triggerServer;
    std_srvs::Trigger m_srv;
    std::string m_RosNamespace;
    std::string m_RosMsgName;
    bool m_ResponseAvailable;

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_STRING &NAMESPACE(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }
    ;

    CIEC_STRING &SRVNAME(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }
    ;

    CIEC_BOOL &SUCCESS(){
      return *static_cast<CIEC_BOOL*>(getDI(3));
    }
    ;

    CIEC_STRING &MESSAGE(){
      return *static_cast<CIEC_STRING*>(getDI(4));
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

    FORTE_FB_DATA_ARRAY(2, 5, 2, 0)
    ;

    void executeEvent(int pa_nEIID);

    bool triggerCallback(std_srvs::Trigger::Request &pa_req, std_srvs::Trigger::Response &pa_resp);

  public:
    // cppcheck-suppress noConstructor
    EVENT_SOURCE_FUNCTION_BLOCK_CTOR(FORTE_TRIGGER_SERVICE_SERVER), m_Initiated(false), m_RosNamespace(""), m_RosMsgName(""), m_ResponseAvailable(false){
    };

    virtual ~FORTE_TRIGGER_SERVICE_SERVER(){};
  };

#endif //close the ifdef sequence from the beginning of the file
