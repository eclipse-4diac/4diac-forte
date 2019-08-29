/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "FieldsToServiceRegistryEntry.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToServiceRegistryEntry_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceRegistryEntry, g_nStringIdFieldsToServiceRegistryEntry)

const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anDataInputNames[] = {g_nStringIdprovidedService, g_nStringIdprovider, g_nStringIdserviceURI, g_nStringIdversion, g_nStringIdudp, g_nStringIdttl, g_nStringIdmetadata};

const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anDataInputTypeIds[] = {g_nStringIdArrowheadService, g_nStringIdArrowheadSystem, g_nStringIdWSTRING, g_nStringIdDINT, g_nStringIdBOOL, g_nStringIdDINT, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anDataOutputNames[] = {g_nStringIdserviceRegistryEntry};

const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anDataOutputTypeIds[] = {g_nStringIdServiceRegistryEntry};

const TForteInt16 FORTE_FieldsToServiceRegistryEntry::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToServiceRegistryEntry::scm_anEIWith[] = {6, 5, 4, 3, 2, 1, 0, 255};
const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToServiceRegistryEntry::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToServiceRegistryEntry::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToServiceRegistryEntry::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToServiceRegistryEntry::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  7,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToServiceRegistryEntry::alg_REQ(void){
serviceRegistryEntry().providedService() = providedService();
serviceRegistryEntry().provider() = provider();
serviceRegistryEntry().serviceURI() = serviceURI();
serviceRegistryEntry().version() = version();
serviceRegistryEntry().udp() = udp();
serviceRegistryEntry().ttl() = ttl();
serviceRegistryEntry().metadata() = metadata();

}


void FORTE_FieldsToServiceRegistryEntry::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToServiceRegistryEntry::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToServiceRegistryEntry::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventREQID == pa_nEIID)
          enterStateREQ();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateREQ:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


