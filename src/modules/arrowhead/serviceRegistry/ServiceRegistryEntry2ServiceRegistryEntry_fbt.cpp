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

#include "ServiceRegistryEntry2ServiceRegistryEntry_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry2ServiceRegistryEntry);

DEFINE_FIRMWARE_FB(FORTE_ServiceRegistryEntry2ServiceRegistryEntry, STRID(ServiceRegistryEntry2ServiceRegistryEntry))

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmDataInputTypeIds[] = {
    STRID(ServiceRegistryEntry)};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmDataOutputTypeIds[] = {
    STRID(ServiceRegistryEntry)};

const TForteInt16 FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEIWith[] = {0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEventInputTypeIds[] = {
    STRID(Event)};

const TDataIOID FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEventOutputNames[] = {
    STRID(CNF)};
const CStringDictionary::TStringId FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmEventOutputTypeIds[] = {
    STRID(Event)};

const SFBInterfaceSpec FORTE_ServiceRegistryEntry2ServiceRegistryEntry::scmFBInterfaceSpec = {1,
                                                                                              scmEventInputNames,
                                                                                              scmEventInputTypeIds,
                                                                                              scmEIWith,
                                                                                              scmEIWithIndexes,
                                                                                              1,
                                                                                              scmEventOutputNames,
                                                                                              scmEventOutputTypeIds,
                                                                                              scmEOWith,
                                                                                              scmEOWithIndexes,
                                                                                              1,
                                                                                              scmDataInputNames,
                                                                                              scmDataInputTypeIds,
                                                                                              1,
                                                                                              scmDataOutputNames,
                                                                                              scmDataOutputTypeIds,
                                                                                              0,
                                                                                              0};

void FORTE_ServiceRegistryEntry2ServiceRegistryEntry::executeEvent(TEventID paEIID,
                                                                   CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
