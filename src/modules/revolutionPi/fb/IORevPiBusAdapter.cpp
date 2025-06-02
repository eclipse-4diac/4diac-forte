/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "IORevPiBusAdapter.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(Event);
USE_STRING_ID(Index);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(IORevPiBusAdapter);
USE_STRING_ID(MasterId);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(UINT);

DEFINE_ADAPTER_TYPE(FORTE_IORevPiBusAdapter, STRID(IORevPiBusAdapter))

const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmDataInputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmDataOutputNames[] = {STRID(QI), STRID(MasterId),
                                                                                    STRID(Index)};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UINT),
                                                                                      STRID(UINT)};
const TDataIOID FORTE_IORevPiBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmEventInputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_IORevPiBusAdapter::scmEOWith[] = {2, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmEventOutputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec FORTE_IORevPiBusAdapter::scmFBInterfaceSpecSocket = {1,
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
                                                                            3,
                                                                            scmDataOutputNames,
                                                                            scmDataOutputTypeIds,
                                                                            0,
                                                                            nullptr,
                                                                            0,
                                                                            nullptr};

const SFBInterfaceSpec FORTE_IORevPiBusAdapter::scmFBInterfaceSpecPlug = {1,
                                                                          scmEventOutputNames,
                                                                          scmEventOutputTypeIds,
                                                                          scmEOWith,
                                                                          scmEOWithIndexes,
                                                                          1,
                                                                          scmEventInputNames,
                                                                          scmEventInputTypeIds,
                                                                          scmEIWith,
                                                                          scmEIWithIndexes,
                                                                          3,
                                                                          scmDataOutputNames,
                                                                          scmDataOutputTypeIds,
                                                                          1,
                                                                          scmDataInputNames,
                                                                          scmDataInputTypeIds,
                                                                          0,
                                                                          nullptr,
                                                                          0,
                                                                          nullptr};

FORTE_IORevPiBusAdapter::FORTE_IORevPiBusAdapter(forte::core::CFBContainer &paContainer,
                                                 const SFBInterfaceSpec &paInterfaceSpec,
                                                 const CStringDictionary::TStringId paInstanceNameId,
                                                 TForteUInt8 paParentAdapterlistID) :
    IOConfigFBMultiAdapter({}, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
    var_QO(0_BOOL),
    var_QI(0_BOOL),
    var_MasterId(0_UINT),
    var_Index(0_UINT) {
}

void FORTE_IORevPiBusAdapter::setInitialValues() {
  forte::CAdapter::setInitialValues();
  var_QO = 0_BOOL;
  var_QI = 0_BOOL;
  var_MasterId = 0_UINT;
  var_Index = 0_UINT;
}

FORTE_IORevPiBusAdapter_Plug::FORTE_IORevPiBusAdapter_Plug(CStringDictionary::TStringId paInstanceNameId,
                                                           forte::core::CFBContainer &paContainer,
                                                           TForteUInt8 paParentAdapterlistID) :
    FORTE_IORevPiBusAdapter(
        paContainer, FORTE_IORevPiBusAdapter::scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_MasterId(nullptr),
    conn_Index(nullptr),
    conn_QO(*this, 0, var_QO) {
}

void FORTE_IORevPiBusAdapter_Plug::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(3, var_Index, conn_Index);
      readData(2, var_MasterId, conn_MasterId);
      readData(1, var_QI, conn_QI);
      if (auto peer = static_cast<FORTE_IORevPiBusAdapter_Socket *>(getPeer()); peer) {
        peer->var_Index = var_Index;
        peer->var_MasterId = var_MasterId;
        peer->var_QI = var_QI;
      }
      break;
    }
    default: break;
  }
}

void FORTE_IORevPiBusAdapter_Plug::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_IORevPiBusAdapter_Plug::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IORevPiBusAdapter_Plug::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_IORevPiBusAdapter_Plug::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_IORevPiBusAdapter_Plug::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
  }
  return nullptr;
}

CDataConnection *FORTE_IORevPiBusAdapter_Plug::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

FORTE_IORevPiBusAdapter_Socket::FORTE_IORevPiBusAdapter_Socket(CStringDictionary::TStringId paInstanceNameId,
                                                               forte::core::CFBContainer &paContainer,
                                                               TForteUInt8 paParentAdapterlistID) :
    FORTE_IORevPiBusAdapter(
        paContainer, FORTE_IORevPiBusAdapter::scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_INIT(*this, 0),
    conn_QO(nullptr),
    conn_QI(*this, 0, var_QI),
    conn_MasterId(*this, 1, var_MasterId),
    conn_Index(*this, 2, var_Index) {
}

void FORTE_IORevPiBusAdapter_Socket::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      readData(0, var_QO, conn_QO);
      if (auto peer = static_cast<FORTE_IORevPiBusAdapter_Plug *>(getPeer()); peer) {
        peer->var_QO = var_QO;
      }
      break;
    }
    default: break;
  }
}

void FORTE_IORevPiBusAdapter_Socket::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      writeData(3, var_Index, conn_Index);
      writeData(2, var_MasterId, conn_MasterId);
      writeData(1, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_IORevPiBusAdapter_Socket::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IORevPiBusAdapter_Socket::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
  }
  return nullptr;
}

CEventConnection *FORTE_IORevPiBusAdapter_Socket::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INIT;
  }
  return nullptr;
}

CDataConnection **FORTE_IORevPiBusAdapter_Socket::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CDataConnection *FORTE_IORevPiBusAdapter_Socket::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
  }
  return nullptr;
}
