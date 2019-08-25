/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - initial contribution
 *******************************************************************************/

#include "ADS_SERVER_CONFIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ADS_SERVER_CONFIG_gen.cpp"
#endif

#include "CAdsConnectionManager.h"

DEFINE_FIRMWARE_FB(FORTE_ADS_SERVER_CONFIG, g_nStringIdADS_SERVER_CONFIG)

const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdFRIENDLY_NAME, g_nStringIdSERVER_ADS_ADDRESS, g_nStringIdADS_PORT, g_nStringIdSERVER_IPV4_OR_HOSTNAME };

const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdUINT, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 FORTE_ADS_SERVER_CONFIG::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_ADS_SERVER_CONFIG::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anEventInputNames[] = { g_nStringIdINIT };

const TDataIOID FORTE_ADS_SERVER_CONFIG::scm_anEOWith[] = { 0, 1, 255 };
const TForteInt16 FORTE_ADS_SERVER_CONFIG::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scm_anEventOutputNames[] = { g_nStringIdINITO };

const SFBInterfaceSpec FORTE_ADS_SERVER_CONFIG::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 5, scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_ADS_SERVER_CONFIG::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(QI()) {
        QO() = forte::ads::CAdsConnectionManager::getInstance().addConnection(FRIENDLY_NAME().getValue(), SERVER_ADS_ADDRESS().getValue(), static_cast<CIEC_UDINT::TValueType>(ADS_PORT()), SERVER_IPV4_OR_HOSTNAME().getValue());
      } else {
        forte::ads::CAdsConnectionManager::getInstance().removeConnection(FRIENDLY_NAME().getValue());
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
  }
}

