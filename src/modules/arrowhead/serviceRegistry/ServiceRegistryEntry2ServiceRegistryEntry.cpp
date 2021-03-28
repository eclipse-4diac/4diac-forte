/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "ServiceRegistryEntry2ServiceRegistryEntry.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ServiceRegistryEntry2ServiceRegistryEntry_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_ServiceRegistryEntry2ServiceRegistryEntry, g_nStringIdServiceRegistryEntry2ServiceRegistryEntry)

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anDataInputTypeIds[] = {g_nStringIdServiceRegistryEntry};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anDataOutputTypeIds[] = {g_nStringIdServiceRegistryEntry};

const TForteInt16 FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_ServiceRegistryEntry2ServiceRegistryEntry::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}



