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

#include "ROSManager.h"
#include <ros/ros.h>
#include <extevhandlerhelper.h>

#include "EXECUTE_ACTION_SERVER.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EXECUTE_ACTION_SERVER_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_EXECUTE_ACTION_SERVER, g_nStringIdEXECUTE_ACTION_SERVER)

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdACTIONNAMESPACE, g_nStringIdACTIONMSGNAME, g_nStringIdRESULT, g_nStringIdID, g_nStringIdSTATE };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdDINT, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdFBSTATUS, g_nStringIdACTIONSTATUS, g_nStringIdCOMMAND, g_nStringIdID1, g_nStringIdID2 };

const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdDINT, g_nStringIdDINT };

const TForteInt16 FORTE_EXECUTE_ACTION_SERVER::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_EXECUTE_ACTION_SERVER::scm_anEIWith[] = { 0, 1, 2, 255, 0, 5, 4, 3, 255 };
const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdRSP };

const TDataIOID FORTE_EXECUTE_ACTION_SERVER::scm_anEOWith[] = { 0, 1, 255, 0, 1, 3, 2, 4, 5, 255 };
const TForteInt16 FORTE_EXECUTE_ACTION_SERVER::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId FORTE_EXECUTE_ACTION_SERVER::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdIND };

const SFBInterfaceSpec FORTE_EXECUTE_ACTION_SERVER::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 6, scm_anDataInputNames, scm_anDataInputTypeIds, 6, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_EXECUTE_ACTION_SERVER::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:

      if(QI() && !m_Initiated){
        
        m_RosNamespace = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONNAMESPACE());
        m_RosMsgName = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONMSGNAME());
        DEVLOG_DEBUG("[EXEC_SERVER] Namespace: %s \n[EXEC_SERVER] Message name : %s \n", m_RosNamespace.c_str(), m_RosMsgName.c_str());

        m_nh = new ros::NodeHandle(m_RosNamespace);

        m_ActionServer = new actionServer(*m_nh, m_RosMsgName, boost::bind(&FORTE_EXECUTE_ACTION_SERVER::ActionExecuteCB, this, _1), false);
        m_ActionServer->start(); //not needed if auto_start=true (last param in ctor of server)

        QO() = true;
        m_Initiated = true;
        FBSTATUS() = "Server started";
      }

      //server already initialized
      else if(QI() && m_Initiated){
        FBSTATUS() = "Server already initialized";
        QO() = true;
      }
      //terminate server and function block
      else{
        m_Initiated = false;
        m_ResultAvailable = false;
        if(m_ActionServer->isActive()){
          m_ActionServer->setAborted();
        }
        m_ActionServer->shutdown(); //TODO test this!
        //ros::shutdown();
        FBSTATUS() = "Action Server Shutdown";
        //DEVLOG_DEBUG("[EXEC_SERVER] Action server shutdown\n");
      }
      sendOutputEvent(scm_nEventINITOID);
      break;

    case scm_nEventRSPID:
      //send a result (see executeCB)
      if(QI()){
        m_Result.result = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(RESULT());
        m_Result.id = ID();
        FBSTATUS() = "Server sends result";
        m_ResultAvailable = true;
      }
      //send feedback
      else{
        // get feedback data from STATE() input
        m_Feedback.state = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(STATE());
        m_ActionServer->publishFeedback(m_Feedback);
      }
      break;

      //The server received a new goal
    case cg_nExternalEventID:
      if(m_ActionServer->isPreemptRequested() || !ros::ok()){
        m_ActionServer->setPreempted();
        ACTIONSTATUS() = "PREEMPTED";
        QO() = false;
        //DEVLOG_DEBUG("[EXEC_SERVER] ExternalEvent: preempted or !ok\n");
      }
      else{
        int stringLength = std::strlen(m_GoalConstPtr->command.c_str());
        CIEC_STRING tmpResult;
        tmpResult.assign(m_GoalConstPtr->command.c_str(), static_cast<TForteUInt16>(stringLength));
        COMMAND().saveAssign(tmpResult);
        ID1() = m_GoalConstPtr->id1;
        ID2() = m_GoalConstPtr->id2;

        FBSTATUS() = "Server received new goal";
        ACTIONSTATUS() = "ACTIVE";
        QO() = true;
      }
      sendOutputEvent(scm_nEventINDID);
      break;
  }
}

void FORTE_EXECUTE_ACTION_SERVER::ActionExecuteCB(const ExecuteGoalConstPtr &pa_goal){

  m_GoalConstPtr = pa_goal;
  ros::Rate r(2); //2Hz

  setEventChainExecutor(m_poInvokingExecEnv);
  //DEVLOG_DEBUG("[EXEC_SERVER] Received goal is: %s \n", mGoalConstPtr->command.c_str());
  getExtEvHandler<CROSManager>(*this).startChain(this);

  // FIXME better idea as flag polling?
  //wait for an available result(mResultAvailable set in case of RSP+)
  while(!m_ResultAvailable && m_Initiated){
    // send cyclic feedback here if needed
    // mActionServer->publishFeedback(mFeedback);

    r.sleep();
  }
  m_ResultAvailable = false;
  m_ActionServer->setSucceeded(m_Result);
  //DEVLOG_DEBUG("[EXEC_SERVER] result is: %s \n", mResult.result.c_str());
  FBSTATUS() = "Server has sent result for current goal";
  ACTIONSTATUS() = "SUCCEEDED";
}
