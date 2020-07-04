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

#ifndef _EXECUTE_ACTION_SERVER_H_
#define _EXECUTE_ACTION_SERVER_H_

#include <esfb.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <forte_dint.h>

#include <actionlib/server/simple_action_server.h>
#include <reapp_msgs/ExecuteAction.h>
#include <reapp_msgs/ExecuteResult.h>

typedef boost::shared_ptr<reapp_msgs::ExecuteGoal const> ExecuteGoalConstPtr; //Warum funktioniert boost::bind mit typedef, aber nicht ohne??
typedef actionlib::SimpleActionServer<reapp_msgs::ExecuteAction> actionServer;

// cppcheck-suppress noConstructor
class FORTE_EXECUTE_ACTION_SERVER : public CEventSourceFB{
  DECLARE_FIRMWARE_FB(FORTE_EXECUTE_ACTION_SERVER)

  private:

    ros::NodeHandle* m_nh;
    std::string m_RosNamespace;
    std::string m_RosMsgName;

    bool m_Initiated;
    bool m_ResultAvailable;
    actionServer* m_ActionServer;
    reapp_msgs::ExecuteFeedback m_Feedback;
    reapp_msgs::ExecuteResult m_Result;
    ExecuteGoalConstPtr m_GoalConstPtr;

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_STRING &ACTIONNAMESPACE(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }
    ;

    CIEC_STRING &ACTIONMSGNAME(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }
    ;

    CIEC_STRING &RESULT(){
      return *static_cast<CIEC_STRING*>(getDI(3));
    }
    ;

    CIEC_DINT &ID(){
      return *static_cast<CIEC_DINT*>(getDI(4));
    }
    ;

    CIEC_STRING &STATE(){
      return *static_cast<CIEC_STRING*>(getDI(5));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_STRING &FBSTATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }
    ;

    CIEC_STRING &ACTIONSTATUS(){
      return *static_cast<CIEC_STRING*>(getDO(2));
    }
    ;

    CIEC_STRING &COMMAND(){
      return *static_cast<CIEC_STRING*>(getDO(3));
    }
    ;

    CIEC_DINT &ID1(){
      return *static_cast<CIEC_DINT*>(getDO(4));
    }
    ;

    CIEC_DINT &ID2(){
      return *static_cast<CIEC_DINT*>(getDO(5));
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

    FORTE_FB_DATA_ARRAY(2, 6, 6, 0)
    ;

    void executeEvent(int pa_nEIID);

    void ActionExecuteCB(const ExecuteGoalConstPtr &pa_goal);

  public:
    // cppcheck-suppress noConstructor
    EVENT_SOURCE_FUNCTION_BLOCK_CTOR(FORTE_EXECUTE_ACTION_SERVER), m_nh(0) ,m_RosNamespace(""), m_RosMsgName(""), m_Initiated(false), m_ResultAvailable(false), m_ActionServer(0) {
    };

    virtual ~FORTE_EXECUTE_ACTION_SERVER(){};

  };

#endif //close the ifdef sequence from the beginning of the file
