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
#include "forte/util/extevhandlerhelper.h"

#include "TRIGGER_SERVICE_SERVER.h"

using namespace forte::core::literals;

DEFINE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_SERVER, "TRIGGER_SERVICE_SERVER"_STRID)

namespace {
  const auto cDataInputNames =
      std::array{"QI"_STRID, "NAMESPACE"_STRID, "SRVNAME"_STRID, "SUCCESS"_STRID, "MESSAGE"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
  const auto cEventInputNames = std::array{"INIT"_STRID, "RSP"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
  const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

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
