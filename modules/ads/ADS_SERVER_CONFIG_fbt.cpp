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

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "CAdsConnectionManager.h"

DEFINE_FIRMWARE_FB(FORTE_ADS_SERVER_CONFIG, "ADS_SERVER_CONFIG"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID, "FRIENDLY_NAME"_STRID, "SERVER_ADS_ADDRESS"_STRID,
                                          "ADS_PORT"_STRID, "SERVER_IPV4_OR_HOSTNAME"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
  const auto cEventInputNames = std::array{"INIT"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};
  const auto cEventOutputNames = std::array{"INITO"_STRID};
  const auto cEventOutputTypeIds = std::array{"EInit"_STRID};
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

FORTE_ADS_SERVER_CONFIG::FORTE_ADS_SERVER_CONFIG(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
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
    conn_STATUS(*this, 1, var_STATUS) {};

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
  switch (paEIID) {
    case scmEventINITID:
      if (QI()) {
        QO() = CIEC_BOOL(forte::ads::CAdsConnectionManager::getInstance().addConnection(
            FRIENDLY_NAME().getStorage().c_str(), SERVER_ADS_ADDRESS().getStorage().c_str(),
            static_cast<CIEC_UDINT::TValueType>(ADS_PORT()), SERVER_IPV4_OR_HOSTNAME().getStorage().c_str()));
      } else {
        forte::ads::CAdsConnectionManager::getInstance().removeConnection(FRIENDLY_NAME().getStorage().c_str());
        QO() = false_BOOL;
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_ADS_SERVER_CONFIG::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_ADS_SERVER_CONFIG::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_FRIENDLY_NAME;
    case 2: return &var_SERVER_ADS_ADDRESS;
    case 3: return &var_ADS_PORT;
    case 4: return &var_SERVER_IPV4_OR_HOSTNAME;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ADS_SERVER_CONFIG::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_ADS_SERVER_CONFIG::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_ADS_SERVER_CONFIG::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_FRIENDLY_NAME;
    case 2: return &conn_SERVER_ADS_ADDRESS;
    case 3: return &conn_ADS_PORT;
    case 4: return &conn_SERVER_IPV4_OR_HOSTNAME;
  }
  return nullptr;
}

CDataConnection *FORTE_ADS_SERVER_CONFIG::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
