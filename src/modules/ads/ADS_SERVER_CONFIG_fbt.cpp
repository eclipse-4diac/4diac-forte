/************************************************************************* 
 *** Copyright (c) 2018 TU Vienna/ACIN  
 ***  
 *** This program and the accompanying materials are made  
 *** available under the terms of the Eclipse Public License 2.0  
 *** which is available at https://www.eclipse.org/legal/epl-2.0/  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 *** 
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ADS_SERVER_CONFIG
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-08-28/Martin Melik Merkumians - TU Vienna/ACIN - initial contribution
 *************************************************************************/

#include "ADS_SERVER_CONFIG_fbt.h"

USE_STRING_ID(ADS_PORT);
USE_STRING_ID(ADS_SERVER_CONFIG);
USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(FRIENDLY_NAME);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(SERVER_ADS_ADDRESS);
USE_STRING_ID(SERVER_IPV4_OR_HOSTNAME);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(WSTRING);


#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "CAdsConnectionManager.h"

DEFINE_FIRMWARE_FB(FORTE_ADS_SERVER_CONFIG, STRID(ADS_SERVER_CONFIG))

const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmDataInputNames[] = {STRID(QI), STRID(FRIENDLY_NAME), STRID(SERVER_ADS_ADDRESS), STRID(ADS_PORT), STRID(SERVER_IPV4_OR_HOSTNAME)};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmDataInputTypeIds[] = {STRID(BOOL), STRID(WSTRING), STRID(WSTRING), STRID(UINT), STRID(WSTRING)};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_ADS_SERVER_CONFIG::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ADS_SERVER_CONFIG::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_ADS_SERVER_CONFIG::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_ADS_SERVER_CONFIG::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmEventOutputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_ADS_SERVER_CONFIG::scmEventOutputTypeIds[] = {STRID(EInit)};
const SFBInterfaceSpec FORTE_ADS_SERVER_CONFIG::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  5, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ADS_SERVER_CONFIG::FORTE_ADS_SERVER_CONFIG(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_FRIENDLY_NAME(u""_WSTRING),
    var_SERVER_ADS_ADDRESS(u""_WSTRING),
    var_ADS_PORT(0_UINT),
    var_SERVER_IPV4_OR_HOSTNAME(u""_WSTRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_FRIENDLY_NAME(nullptr),
    conn_SERVER_ADS_ADDRESS(nullptr),
    conn_ADS_PORT(nullptr),
    conn_SERVER_IPV4_OR_HOSTNAME(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {
};

void FORTE_ADS_SERVER_CONFIG::setInitialValues() {
  var_QI = 0_BOOL;
  var_FRIENDLY_NAME = u""_WSTRING;
  var_SERVER_ADS_ADDRESS = u""_WSTRING;
  var_ADS_PORT = 0_UINT;
  var_SERVER_IPV4_OR_HOSTNAME = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_ADS_SERVER_CONFIG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      if(QI()) {
        QO() = CIEC_BOOL(forte::ads::CAdsConnectionManager::getInstance().addConnection(FRIENDLY_NAME().getStorage().c_str(), SERVER_ADS_ADDRESS().getStorage().c_str(), static_cast<CIEC_UDINT::TValueType>(ADS_PORT()), SERVER_IPV4_OR_HOSTNAME().getStorage().c_str()));
      } else {
        forte::ads::CAdsConnectionManager::getInstance().removeConnection(FRIENDLY_NAME().getStorage().c_str());
        QO() = CIEC_BOOL(false);
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_ADS_SERVER_CONFIG::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_ADS_SERVER_CONFIG::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_FRIENDLY_NAME;
    case 2: return &var_SERVER_ADS_ADDRESS;
    case 3: return &var_ADS_PORT;
    case 4: return &var_SERVER_IPV4_OR_HOSTNAME;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ADS_SERVER_CONFIG::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_ADS_SERVER_CONFIG::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_ADS_SERVER_CONFIG::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_FRIENDLY_NAME;
    case 2: return &conn_SERVER_ADS_ADDRESS;
    case 3: return &conn_ADS_PORT;
    case 4: return &conn_SERVER_IPV4_OR_HOSTNAME;
  }
  return nullptr;
}

CDataConnection *FORTE_ADS_SERVER_CONFIG::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
