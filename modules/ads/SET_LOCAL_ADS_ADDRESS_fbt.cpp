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
 *** Name: SET_LOCAL_ADS_ADDRESS
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-08-31/Martin Melik Merkumians - TU Vienna/ACIN - initial contribution
 *************************************************************************/

#include "SET_LOCAL_ADS_ADDRESS_fbt.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "sstream"
#include "AdsLib.h"

DEFINE_FIRMWARE_FB(FORTE_SET_LOCAL_ADS_ADDRESS, "SET_LOCAL_ADS_ADDRESS"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID, "LOCAL_ADS_ADDRESS"_STRID};
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

FORTE_SET_LOCAL_ADS_ADDRESS::FORTE_SET_LOCAL_ADS_ADDRESS(const forte::StringId paInstanceNameId,
                                                         forte::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_PARAMS(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_LOCAL_ADS_ADDRESS(""_STRING),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_PARAMS(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS),
    conn_LOCAL_ADS_ADDRESS(*this, 2, var_LOCAL_ADS_ADDRESS) {};

void FORTE_SET_LOCAL_ADS_ADDRESS::setInitialValues() {
  var_QI = 0_BOOL;
  var_PARAMS = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
  var_LOCAL_ADS_ADDRESS = ""_STRING;
}

void FORTE_SET_LOCAL_ADS_ADDRESS::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      AmsNetId desiredAddress(std::string(PARAMS().getStorage().c_str()));
      if (QI()) {
        AdsSetLocalAddress(desiredAddress);
        AmsAddr actualAddress = {0};
        long port = AdsPortOpenEx();
        AdsGetLocalAddressEx(port, &actualAddress);
        if (actualAddress.netId == desiredAddress) {
          STATUS() = CIEC_WSTRING("OK");
          QO() = true_BOOL;
          std::stringstream amsNetId;
          amsNetId << std::to_string(actualAddress.netId.b[0]) << "." + std::to_string(actualAddress.netId.b[1]) << "."
                   << std::to_string(actualAddress.netId.b[2]) << "." << std::to_string(actualAddress.netId.b[3]) << "."
                   << std::to_string(actualAddress.netId.b[4]) << "." << std::to_string(actualAddress.netId.b[5]);
          LOCAL_ADS_ADDRESS() = CIEC_STRING(amsNetId.str().c_str());
        } else {
          STATUS() = CIEC_WSTRING("NOT OK");
          QO() = false_BOOL;
        }
      } else {
        QO() = false_BOOL;
      }

      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_SET_LOCAL_ADS_ADDRESS::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_SET_LOCAL_ADS_ADDRESS::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_LOCAL_ADS_ADDRESS, conn_LOCAL_ADS_ADDRESS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_SET_LOCAL_ADS_ADDRESS::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PARAMS;
  }
  return nullptr;
}

CIEC_ANY *FORTE_SET_LOCAL_ADS_ADDRESS::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
    case 2: return &var_LOCAL_ADS_ADDRESS;
  }
  return nullptr;
}

CEventConnection *FORTE_SET_LOCAL_ADS_ADDRESS::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_SET_LOCAL_ADS_ADDRESS::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PARAMS;
  }
  return nullptr;
}

CDataConnection *FORTE_SET_LOCAL_ADS_ADDRESS::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
    case 2: return &conn_LOCAL_ADS_ADDRESS;
  }
  return nullptr;
}
