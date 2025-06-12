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

#include "ServiceCallManager.h"
#include "ROSManager.h"
#include <extevhandlerhelper.h>

#include "TRIGGER_SERVICE_CLIENT.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MESSAGE);
USE_STRING_ID(NAMESPACE);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(SRVNAME);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(SUCCESS);
USE_STRING_ID(TRIGGER_SERVICE_CLIENT);

DEFINE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_CLIENT, STRID(TRIGGER_SERVICE_CLIENT))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(NAMESPACE), STRID(SRVNAME)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS), STRID(SUCCESS), STRID(MESSAGE)};
  const auto cEventInputNames = std::array{STRID(INIT), STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(CNF)};
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

void FORTE_TRIGGER_SERVICE_CLIENT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      // initiate
      if (!m_Initiated && QI()) {
        setEventChainExecutor(paECET);
        m_RosNamespace = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(NAMESPACE());
        m_RosMsgName = getExtEvHandler<CROSManager>(*this).ciecStringToStdString(SRVNAME());
        m_nh = new ros::NodeHandle(m_RosNamespace);
        m_triggerClient = m_nh->serviceClient<std_srvs::Trigger>(m_RosMsgName);
        STATUS() = "Client waits for server";

        getExtEvHandler<CServiceCallManager>(*this).queueConnectWait(this);
      }
      // terminate
      else if (m_Initiated && !QI()) {
        m_nh->shutdown();
        STATUS() = "Client terminated";
        QO() = false;
        m_Initiated = false;
        sendOutputEvent(scmEventINITOID, paECET);
      }
      // silently ignore other cases
      else {
        STATUS() = "Unknown init command sequence";
        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
    case scmEventREQID:
      // call service
      if (m_Initiated && QI()) {
        STATUS() = "Request sent";
        // add to queue
        getExtEvHandler<CServiceCallManager>(*this).queueServiceCall(this);
      }
      // uninitialized or REQ-
      else {
        STATUS() = "Sending request not possible";
        QO() = false;
        sendOutputEvent(scmEventCNFID, paECET);
      }
      break;
    case cgExternalEventID:
      // waitForExistence returned
      if (!m_Initiated) {
        m_Initiated = true;
        STATUS() = "Client connected to server";
        QO() = true;
        sendOutputEvent(scmEventINITOID, paECET);
      }
      // call returned
      else {
        sendOutputEvent(scmEventCNFID, paECET);
      }
      break;
  }
}

void FORTE_TRIGGER_SERVICE_CLIENT::callService() {
  bool srv_success = m_triggerClient.call(m_srv);

  SUCCESS() = m_srv.response.success;
  MESSAGE() = getExtEvHandler<CROSManager>(*this).stdStringToCiecString(m_srv.response.message);

  if (srv_success) {
    QO() = true;
    STATUS() = "Response received";
  } else {
    QO() = false;
    STATUS() = "Receiving response failed";
    SUCCESS() = false;
    MESSAGE() = "failed";
  }
  getExtEvHandler<CServiceCallManager>(*this).startChain(this);
}

void FORTE_TRIGGER_SERVICE_CLIENT::waitForServer() {

  if (!m_triggerClient.waitForExistence()) {
    DEVLOG_ERROR("[FORTE_TRIGGER_SERVICE_CLIENT] connection failed. Server not existing. \n");
  } else {
    getExtEvHandler<CServiceCallManager>(*this).startChain(this);
  }
}
