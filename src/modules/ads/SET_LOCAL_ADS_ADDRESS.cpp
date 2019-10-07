/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - initial contribution
 *******************************************************************************/

#include "SET_LOCAL_ADS_ADDRESS.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SET_LOCAL_ADS_ADDRESS_gen.cpp"
#endif

#include <sstream>

#include <AdsLib.h>

DEFINE_FIRMWARE_FB(FORTE_SET_LOCAL_ADS_ADDRESS, g_nStringIdSET_LOCAL_ADS_ADDRESS)

const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anDataInputNames[] ={g_nStringIdQI, g_nStringIdPARAMS};

const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdLOCAL_ADS_ADDRESS };

const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdSTRING };

const TForteInt16 FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEventInputNames[] = { g_nStringIdINIT };

const TDataIOID FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEOWith[] = { 0, 1, 2, 255 };
const TForteInt16 FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_SET_LOCAL_ADS_ADDRESS::scm_anEventOutputNames[] = { g_nStringIdINITO };

const SFBInterfaceSpec FORTE_SET_LOCAL_ADS_ADDRESS::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 2, scm_anDataInputNames, scm_anDataInputTypeIds, 3, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_SET_LOCAL_ADS_ADDRESS::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      AmsNetId desiredAddress(std::string(PARAMS().getValue()));
      if(QI()){
        AdsSetLocalAddress(desiredAddress);
        AmsAddr actualAddress = { 0 };
        long port = AdsPortOpenEx();
        AdsGetLocalAddressEx(port, &actualAddress);
        if(actualAddress.netId == desiredAddress){
          STATUS() = "OK";
          QO() = true;
          std::stringstream amsNetId;
          amsNetId << std::to_string(actualAddress.netId.b[0]) << "." + std::to_string(actualAddress.netId.b[1]) << "." << std::to_string(actualAddress.netId.b[2]) << "." << std::to_string(actualAddress.netId.b[3]) << "." << std::to_string(actualAddress.netId.b[4]) << "." << std::to_string(actualAddress.netId.b[5]);
          LOCAL_ADS_ADDRESS() = amsNetId.str().c_str();
        }
        else{
          STATUS() = "NOT OK";
          QO() = false;
        }
      }
      else{
        QO() = false;
      }

      sendOutputEvent(scm_nEventINITOID);
      break;
  }
}

