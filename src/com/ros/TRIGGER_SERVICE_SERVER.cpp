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

#include "TRIGGER_SERVICE_SERVER.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MESSAGE);
USE_STRING_ID(NAMESPACE);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RSP);
USE_STRING_ID(SRVNAME);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(SUCCESS);
USE_STRING_ID(TRIGGER_SERVICE_SERVER);

DEFINE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_SERVER, STRID(TRIGGER_SERVICE_SERVER))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(NAMESPACE), STRID(SRVNAME), STRID(SUCCESS), STRID(MESSAGE)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
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

void FORTE_TRIGGER_SERVICE_SERVER::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      // initiate
      if (!m_Initiated && QI()) {

        m_RosNamespace = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(NAMESPACE());
        m_RosMsgName = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(SRVNAME());
        m_nh = new ros::NodeHandle(m_RosNamespace);
        m_triggerServer = m_nh->advertiseService<FORTE_TRIGGER_SERVICE_SERVER>(
            m_RosMsgName, &FORTE_TRIGGER_SERVICE_SERVER::triggerCallback,
            const_cast<FORTE_TRIGGER_SERVICE_SERVER *>(this));
        m_Initiated = true;
        STATUS() = "Server initiated";
        QO() = true;
      }
      // terminate
      else if (m_Initiated && !QI()) {
        m_nh->shutdown();
        STATUS() = "Server terminated";
        QO() = false;
      } else {
        STATUS() = "initiation or termination failed";
        QO() = false;
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;

    case scmEventRSPID:
      STATUS() = "Processing service request finished";
      m_ResponseAvailable = true;
      break;

    case cgExternalEventID:
      QO() = true;
      sendOutputEvent(scmEventINDID);
      break;
  }
}

// TODO use or delete first parameter
bool FORTE_TRIGGER_SERVICE_SERVER::triggerCallback(std_srvs::Trigger::Request &,
                                                   std_srvs::Trigger::Response &pa_resp,
                                                   CEventChainExecutionThread *const paECET) {
  setEventChainExecutor(paECET);
  getExtEvHandler<CROSManager>(*this).startChain(this);

  // is a response available
  ros::Rate r(2); // 1Hz
  while (!m_ResponseAvailable) {
    r.sleep();
  }

  // write response
  pa_resp.success = SUCCESS();
  pa_resp.message = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(MESSAGE());

  m_ResponseAvailable = false;

  return true;
}
