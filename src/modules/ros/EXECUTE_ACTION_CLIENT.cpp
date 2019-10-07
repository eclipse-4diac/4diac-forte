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

#include <ros/ros.h>
#include "ROSManager.h"
#include <extevhandlerhelper.h>

#include "EXECUTE_ACTION_CLIENT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EXECUTE_ACTION_CLIENT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_EXECUTE_ACTION_CLIENT, g_nStringIdEXECUTE_ACTION_CLIENT)

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdACTIONNAMESPACE, g_nStringIdACTIONMSGNAME, g_nStringIdCOMMAND, g_nStringIdID1, g_nStringIdID2 };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdDINT, g_nStringIdDINT };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdFBSTATUS, g_nStringIdACTIONSTATUS, g_nStringIdRESULT, g_nStringIdID, g_nStringIdSTATE };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdDINT, g_nStringIdSTRING };

const TForteInt16 FORTE_EXECUTE_ACTION_CLIENT::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_EXECUTE_ACTION_CLIENT::scm_anEIWith[] = { 0, 1, 2, 255, 0, 3, 4, 5, 255 };
const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_EXECUTE_ACTION_CLIENT::scm_anEOWith[] = { 0, 1, 255, 0, 1, 2, 4, 3, 255, 5, 255 };
const TForteInt16 FORTE_EXECUTE_ACTION_CLIENT::scm_anEOWithIndexes[] = { 0, 3, 9 };
const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_CLIENT::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_EXECUTE_ACTION_CLIENT::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 6, scm_anDataInputNames, scm_anDataInputTypeIds, 6, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_EXECUTE_ACTION_CLIENT::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      //initialize FB
      if(!m_Initiated){

        connectToActionServer();
        QO() = true;
      }
      //already initialized
      else if(QI() && m_Initiated){
        FBSTATUS() = "Already initiated";
        QO() = true;
      }
      //termintate FB
      else if(!QI() && m_Initiated){
        m_ActionClient->cancelAllGoals();
        ros::shutdown();
        m_Initiated = false;
        FBSTATUS() = "Terminated - All goals canceled";
        QO() = false;
      }
      sendOutputEvent(scm_nEventINITOID);
      break;

    case scm_nEventREQID:
      //send new goal to the action server
      if(m_Initiated){
        //send new goal
        if(QI() && !m_GoalActive){
          if(m_ActionClient->isServerConnected()){
            //register the external eventchain on the thread of the Request event
            setEventChainExecutor(m_poInvokingExecEnv);
            reapp_msgs::ExecuteGoal goal;
            goal.id1 = ID1();
            goal.id2 = ID2();
            goal.command = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(COMMAND());

            m_ActionClient->sendGoal(goal, boost::bind(&FORTE_EXECUTE_ACTION_CLIENT::doneCallback, this, _1, _2), boost::bind(&FORTE_EXECUTE_ACTION_CLIENT::activeCallback, this), boost::bind(&FORTE_EXECUTE_ACTION_CLIENT::feedbackCallback, this, _1));

            FBSTATUS() = "New action goal sent";
            QO() = true;
          }
          else{ //not connected
            FBSTATUS() = "Sending goal not possible. Server not Connected";
            QO() = false;
          }
        }
        //ignore goals that are sent while another goal is active
        else if(!QI() && !m_GoalActive){
          FBSTATUS() = "Goal not sent, server busy";
        }
        //cancel the goal at the action server that was sent by the client
        else if(!QI() && m_GoalActive){
          FBSTATUS() = "Canceling goals";
          m_ActionClient->cancelAllGoals();
          FBSTATUS() = "Goals canceled";
          QO() = false;
          sendOutputEvent(scm_nEventCNFID);
        }
      }

      //FB is not initiated at all/correctly
      else{
        FBSTATUS() = "FB not initiated";
        QO() = false;
      }
      break;

    case cg_nExternalEventID:
      //we received feedback (QO=false)
      if(false == QO()){

        const std::string callbackFeedback = m_ExecuteFeedbackConstPtr->state;

        STATE() = callbackFeedback.c_str();
        FBSTATUS() = "Feedback received";
      }
      //we received a result (QO=true)
      else{
        const std::string callbackResultResult = m_ExecuteResultConstPtr->result;
        const int callbackResultID = m_ExecuteResultConstPtr->id;
        ID() = callbackResultID;
        RESULT() = callbackResultResult.c_str();

        ACTIONSTATUS() = getCurrentActionState().c_str();
        FBSTATUS() = "Result received";
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_EXECUTE_ACTION_CLIENT::doneCallback(const actionlib::SimpleClientGoalState&, const ExecuteResultConstPtr& pa_result){
  m_ExecuteResultConstPtr = pa_result;

  ACTIONSTATUS() = getCurrentActionState().c_str();
  QO() = true;
  m_GoalActive = false;
  getExtEvHandler<CROSManager>(*this).startChain(this);
}

void FORTE_EXECUTE_ACTION_CLIENT::activeCallback(){
  ACTIONSTATUS() = getCurrentActionState().c_str();
  m_GoalActive = true;
}

void FORTE_EXECUTE_ACTION_CLIENT::feedbackCallback(const ExecuteFeedbackConstPtr &pa_feedback){
  m_ExecuteFeedbackConstPtr = pa_feedback;

  ACTIONSTATUS() = getCurrentActionState().c_str();
  QO() = false;
  getExtEvHandler<CROSManager>(*this).startChain(this);
}

std::string FORTE_EXECUTE_ACTION_CLIENT::getCurrentActionState(){
  std::string retVal = "UNKNOWN";

  int i = m_ActionClient->getState().state_;
  switch (i){
    case actionlib::SimpleClientGoalState::PENDING:
      retVal = "PENDING";
      break;
    case actionlib::SimpleClientGoalState::ACTIVE:
      retVal = "ACTIVE";
      break;
    case actionlib::SimpleClientGoalState::PREEMPTED:
      retVal = "PREEMPTED";
      break;
    case actionlib::SimpleClientGoalState::SUCCEEDED:
      retVal = "SUCCEEDED";
      break;
    case actionlib::SimpleClientGoalState::ABORTED:
    case actionlib::SimpleClientGoalState::REJECTED:
      retVal = "ABORTED";
      break;
    case actionlib::SimpleClientGoalState::RECALLED:
      retVal = "RECALLED";
      break;
    case actionlib::SimpleClientGoalState::LOST:
      retVal = "LOST";
      break;
  }
  return retVal;
}

void FORTE_EXECUTE_ACTION_CLIENT::connectToActionServer(){
  m_RosNamespace = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONNAMESPACE());
  m_RosMsgName = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONMSGNAME());
  DEVLOG_DEBUG("[EXEC_CLIENT] Namespace: %s \nMessage name : %s \n", m_RosNamespace.c_str(), m_RosMsgName.c_str());

  nh = new ros::NodeHandle(m_RosNamespace);

  m_ActionClient = new actionClient(*nh, m_RosMsgName, true);

  FBSTATUS() = "Client waits for server";
  m_ActionClient->waitForServer();
  FBSTATUS() = "Client connected to server";
  m_Initiated = true;
}
