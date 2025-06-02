/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
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
 *** Name: PLCnextBusAdapter
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextBusAdapter.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(Index);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MasterId);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);

DEFINE_ADAPTER_TYPE(FORTE_PLCnextBusAdapter, STRID(PLCnextBusAdapter))

const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataInputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataOutputNames[] = {
    STRID(QI), STRID(MasterId), STRID(Index), STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UINT),
                                                                                      STRID(UINT), STRID(UINT)};
const TDataIOID FORTE_PLCnextBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventInputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_PLCnextBusAdapter::scmEOWith[] = {2, 3, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventOutputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventOutputTypeIds[] = {STRID(EInit)};

const SFBInterfaceSpec FORTE_PLCnextBusAdapter::scmFBInterfaceSpecSocket = {1,
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
                                                                            4,
                                                                            scmDataOutputNames,
                                                                            scmDataOutputTypeIds,
                                                                            0,
                                                                            nullptr,
                                                                            0,
                                                                            nullptr};

const SFBInterfaceSpec FORTE_PLCnextBusAdapter::scmFBInterfaceSpecPlug = {1,
                                                                          scmEventOutputNames,
                                                                          scmEventOutputTypeIds,
                                                                          scmEOWith,
                                                                          scmEOWithIndexes,
                                                                          1,
                                                                          scmEventInputNames,
                                                                          scmEventInputTypeIds,
                                                                          scmEIWith,
                                                                          scmEIWithIndexes,
                                                                          4,
                                                                          scmDataOutputNames,
                                                                          scmDataOutputTypeIds,
                                                                          1,
                                                                          scmDataInputNames,
                                                                          scmDataInputTypeIds,
                                                                          0,
                                                                          nullptr,
                                                                          0,
                                                                          nullptr};

FORTE_PLCnextBusAdapter::FORTE_PLCnextBusAdapter(forte::core::CFBContainer &paContainer,
                                                 const SFBInterfaceSpec &paInterfaceSpec,
                                                 const CStringDictionary::TStringId paInstanceNameId,
                                                 TForteUInt8 paParentAdapterlistID) :
    IOConfigFBMultiAdapter({}, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
    var_QO(0_BOOL),
    var_QI(0_BOOL),
    var_MasterId(0_UINT),
    var_Index(0_UINT),
    var_UpdateInterval(0_UINT) {
}

void FORTE_PLCnextBusAdapter::setInitialValues() {
  forte::CAdapter::setInitialValues();
  var_QO = 0_BOOL;
  var_QI = 0_BOOL;
  var_MasterId = 0_UINT;
  var_Index = 0_UINT;
  var_UpdateInterval = 0_UINT;
}

FORTE_PLCnextBusAdapter_Plug::FORTE_PLCnextBusAdapter_Plug(CStringDictionary::TStringId paInstanceNameId,
                                                           forte::core::CFBContainer &paContainer,
                                                           TForteUInt8 paParentAdapterlistID) :
    FORTE_PLCnextBusAdapter(
        paContainer, FORTE_PLCnextBusAdapter::scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_MasterId(nullptr),
    conn_Index(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO) {
}

void FORTE_PLCnextBusAdapter_Plug::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(3, var_Index, conn_Index);
      readData(4, var_UpdateInterval, conn_UpdateInterval);
      readData(2, var_MasterId, conn_MasterId);
      readData(1, var_QI, conn_QI);
      if (auto peer = static_cast<FORTE_PLCnextBusAdapter_Socket *>(getPeer()); peer) {
        peer->var_Index = var_Index;
        peer->var_UpdateInterval = var_UpdateInterval;
        peer->var_MasterId = var_MasterId;
        peer->var_QI = var_QI;
      }
      break;
    }
    default: break;
  }
}

void FORTE_PLCnextBusAdapter_Plug::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_PLCnextBusAdapter_Plug::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
    case 3: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextBusAdapter_Plug::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextBusAdapter_Plug::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextBusAdapter_Plug::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
    case 3: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextBusAdapter_Plug::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

FORTE_PLCnextBusAdapter_Socket::FORTE_PLCnextBusAdapter_Socket(CStringDictionary::TStringId paInstanceNameId,
                                                               forte::core::CFBContainer &paContainer,
                                                               TForteUInt8 paParentAdapterlistID) :
    FORTE_PLCnextBusAdapter(
        paContainer, FORTE_PLCnextBusAdapter::scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_INIT(*this, 0),
    conn_QO(nullptr),
    conn_QI(*this, 0, var_QI),
    conn_MasterId(*this, 1, var_MasterId),
    conn_Index(*this, 2, var_Index),
    conn_UpdateInterval(*this, 3, var_UpdateInterval) {
}

void FORTE_PLCnextBusAdapter_Socket::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      readData(0, var_QO, conn_QO);
      if (auto peer = static_cast<FORTE_PLCnextBusAdapter_Plug *>(getPeer()); peer) {
        peer->var_QO = var_QO;
      }
      break;
    }
    default: break;
  }
}

void FORTE_PLCnextBusAdapter_Socket::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      writeData(3, var_Index, conn_Index);
      writeData(4, var_UpdateInterval, conn_UpdateInterval);
      writeData(2, var_MasterId, conn_MasterId);
      writeData(1, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_PLCnextBusAdapter_Socket::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextBusAdapter_Socket::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
    case 3: return &var_UpdateInterval;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextBusAdapter_Socket::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INIT;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextBusAdapter_Socket::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextBusAdapter_Socket::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
    case 3: return &conn_UpdateInterval;
  }
  return nullptr;
}
