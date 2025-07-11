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
#include "core/util/extevhandlerhelper.h"

#include "EXECUTE_ACTION_SERVER.h"

USE_STRING_ID(ACTIONMSGNAME);
USE_STRING_ID(ACTIONNAMESPACE);
USE_STRING_ID(ACTIONSTATUS);
USE_STRING_ID(BOOL);
USE_STRING_ID(COMMAND);
USE_STRING_ID(DINT);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(EXECUTE_ACTION_SERVER);
USE_STRING_ID(FBSTATUS);
USE_STRING_ID(ID);
USE_STRING_ID(ID1);
USE_STRING_ID(ID2);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RESULT);
USE_STRING_ID(RSP);
USE_STRING_ID(STATE);
USE_STRING_ID(STRING);

DEFINE_FIRMWARE_FB(FORTE_EXECUTE_ACTION_SERVER, STRID(EXECUTE_ACTION_SERVER))

namespace {
  const auto cDataInputNames =
      std::array{STRID(QI), STRID(ACTIONNAMESPACE), STRID(ACTIONMSGNAME), STRID(RESULT), STRID(ID), STRID(STATE)};
  const auto cDataOutputNames =
      std::array{STRID(QO), STRID(FBSTATUS), STRID(ACTIONSTATUS), STRID(COMMAND), STRID(ID1), STRID(ID2)};
  const auto cEventInputNames = std::array{STRID(INIT), STRID(RSP)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

void FORTE_EXECUTE_ACTION_SERVER::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:

      if (QI() && !m_Initiated) {

        m_RosNamespace = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONNAMESPACE());
        m_RosMsgName = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(ACTIONMSGNAME());
        DEVLOG_DEBUG("[EXEC_SERVER] Namespace: %s \n[EXEC_SERVER] Message name : %s \n", m_RosNamespace.c_str(),
                     m_RosMsgName.c_str());

        m_nh = new ros::NodeHandle(m_RosNamespace);

        m_ActionServer = new actionServer(
            *m_nh, m_RosMsgName, boost::bind(&FORTE_EXECUTE_ACTION_SERVER::ActionExecuteCB, this, paECET, _1), false);
        m_ActionServer->start(); // not needed if auto_start=true (last param in ctor of server)

        QO() = true;
        m_Initiated = true;
        FBSTATUS() = "Server started";
      }

      // server already initialized
      else if (QI() && m_Initiated) {
        FBSTATUS() = "Server already initialized";
        QO() = true;
      }
      // terminate server and function block
      else {
        m_Initiated = false;
        m_ResultAvailable = false;
        if (m_ActionServer->isActive()) {
          m_ActionServer->setAborted();
        }
        m_ActionServer->shutdown(); // TODO test this!
        // ros::shutdown();
        FBSTATUS() = "Action Server Shutdown";
        // DEVLOG_DEBUG("[EXEC_SERVER] Action server shutdown\n");
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;

    case scmEventRSPID:
      // send a result (see executeCB)
      if (QI()) {
        m_Result.result = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(RESULT());
        m_Result.id = ID();
        FBSTATUS() = "Server sends result";
        m_ResultAvailable = true;
      }
      // send feedback
      else {
        // get feedback data from STATE() input
        m_Feedback.state = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(STATE());
        m_ActionServer->publishFeedback(m_Feedback);
      }
      break;

      // The server received a new goal
    case cgExternalEventID:
      if (m_ActionServer->isPreemptRequested() || !ros::ok()) {
        m_ActionServer->setPreempted();
        ACTIONSTATUS() = "PREEMPTED";
        QO() = false;
        // DEVLOG_DEBUG("[EXEC_SERVER] ExternalEvent: preempted or !ok\n");
      } else {
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
      sendOutputEvent(scmEventINDID);
      break;
  }
}

void FORTE_EXECUTE_ACTION_SERVER::ActionExecuteCB(const ExecuteGoalConstPtr &pa_goal,
                                                  CEventChainExecutionThread *const paECET) {

  m_GoalConstPtr = pa_goal;
  ros::Rate r(2); // 2Hz

  setEventChainExecutor(paECET);
  // DEVLOG_DEBUG("[EXEC_SERVER] Received goal is: %s \n", mGoalConstPtr->command.c_str());
  getExtEvHandler<CROSManager>(*this).startChain(this);

  // FIXME better idea as flag polling?
  // wait for an available result(mResultAvailable set in case of RSP+)
  while (!m_ResultAvailable && m_Initiated) {
    // send cyclic feedback here if needed
    // mActionServer->publishFeedback(mFeedback);

    r.sleep();
  }
  m_ResultAvailable = false;
  m_ActionServer->setSucceeded(m_Result);
  // DEVLOG_DEBUG("[EXEC_SERVER] result is: %s \n", mResult.result.c_str());
  FBSTATUS() = "Server has sent result for current goal";
  ACTIONSTATUS() = "SUCCEEDED";
}
