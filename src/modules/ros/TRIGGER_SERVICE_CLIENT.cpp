/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#include "TRIGGER_SERVICE_CLIENT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TRIGGER_SERVICE_CLIENT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_CLIENT, g_nStringIdTRIGGER_SERVICE_CLIENT)

const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdNAMESPACE, g_nStringIdSRVNAME };

const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdSUCCESS, g_nStringIdMESSAGE };

const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdSTRING };

const TForteInt16 FORTE_TRIGGER_SERVICE_CLIENT::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_TRIGGER_SERVICE_CLIENT::scm_anEIWith[] = { 0, 1, 2, 255, 0, 255 };
const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_TRIGGER_SERVICE_CLIENT::scm_anEOWith[] = { 0, 1, 255, 0, 1, 2, 3, 255 };
const TForteInt16 FORTE_TRIGGER_SERVICE_CLIENT::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId FORTE_TRIGGER_SERVICE_CLIENT::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_TRIGGER_SERVICE_CLIENT::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 3, scm_anDataInputNames, scm_anDataInputTypeIds, 4, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_TRIGGER_SERVICE_CLIENT::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      //initiate
      if(!m_Initiated && QI()){
        m_RosNamespace = CROSManager::getInstance().ciecStringToStdString(NAMESPACE());
        m_RosMsgName = CROSManager::getInstance().ciecStringToStdString(SRVNAME());
        m_nh = new ros::NodeHandle(m_RosNamespace);
        m_triggerClient = m_nh->serviceClient < std_srvs::Trigger > (m_RosMsgName);
        m_Initiated = true;
        STATUS() = "Client waits for server";
        bool connect_success = m_triggerClient.waitForExistence();

        STATUS() = "Client connected to server";
        QO() = true;
      }
      //terminate
      else if(m_Initiated && !QI()){
        m_nh->shutdown();
        STATUS() = "Client terminated";
        QO() = false;
      }
      //silently ignore other cases
      else{
        STATUS() = "Unknown init command sequence";
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      //call service
      if(m_Initiated && QI()){
        STATUS() = "Request sent";

        // TODO implement a non-blocking version of the client (worker-thread in handler)
        bool srv_success = m_triggerClient.call(m_srv);

        SUCCESS() = m_srv.response.success;
        MESSAGE() = CROSManager::getInstance().stdStringToCiecString(m_srv.response.message);

        if(srv_success){
          QO() = true;
          STATUS() = "Response received";
        }
        else{
          QO() = false;
          STATUS() = "Receiving response failed";
        }
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}
