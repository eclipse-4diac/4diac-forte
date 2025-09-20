/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "WagoBusAdapter.h"

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_WagoBusAdapter, "WagoBusAdapter"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QO"_STRID};
  const auto cDataOutputNames = std::array{"QI"_STRID, "MasterId"_STRID, "Index"_STRID};
  const auto cEventInputNames = std::array{"INITO"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};
  const auto cEventOutputNames = std::array{"INIT"_STRID};
  const auto cEventOutputTypeIds = std::array{"EInit"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpecSocket = {
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

  const SFBInterfaceSpec cFBInterfaceSpecPlug = {
      .mEINames = cEventOutputNames,
      .mEITypeNames = cEventOutputTypeIds,
      .mEONames = cEventInputNames,
      .mEOTypeNames = cEventInputTypeIds,
      .mDINames = cDataOutputNames,
      .mDONames = cDataInputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

} // namespace

FORTE_WagoBusAdapter::FORTE_WagoBusAdapter(CFBContainer &paContainer,
                                           const SFBInterfaceSpec &paInterfaceSpec,
                                           const forte::StringId paInstanceNameId,
                                           TForteUInt8 paParentAdapterlistID) :
    IOConfigFBMultiAdapter({}, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
    var_QO(0_BOOL),
    var_QI(0_BOOL),
    var_MasterId(0_UINT),
    var_Index(0_UINT) {
}

void FORTE_WagoBusAdapter::setInitialValues() {
  CAdapter::setInitialValues();
  var_QO = 0_BOOL;
  var_QI = 0_BOOL;
  var_MasterId = 0_UINT;
  var_Index = 0_UINT;
}

FORTE_WagoBusAdapter_Plug::FORTE_WagoBusAdapter_Plug(forte::StringId paInstanceNameId,
                                                     CFBContainer &paContainer,
                                                     TForteUInt8 paParentAdapterlistID) :
    FORTE_WagoBusAdapter(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_MasterId(nullptr),
    conn_Index(nullptr),
    conn_QO(*this, 0, var_QO) {
}

void FORTE_WagoBusAdapter_Plug::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(3, var_Index, conn_Index);
      readData(2, var_MasterId, conn_MasterId);
      readData(1, var_QI, conn_QI);
      if (auto peer = static_cast<FORTE_WagoBusAdapter_Socket *>(getPeer()); peer) {
        peer->var_Index = var_Index;
        peer->var_MasterId = var_MasterId;
        peer->var_QI = var_QI;
      }
      break;
    }
    default: break;
  }
}

void FORTE_WagoBusAdapter_Plug::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_WagoBusAdapter_Plug::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WagoBusAdapter_Plug::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_WagoBusAdapter_Plug::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_WagoBusAdapter_Plug::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
  }
  return nullptr;
}

CDataConnection *FORTE_WagoBusAdapter_Plug::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

FORTE_WagoBusAdapter_Socket::FORTE_WagoBusAdapter_Socket(forte::StringId paInstanceNameId,
                                                         CFBContainer &paContainer,
                                                         TForteUInt8 paParentAdapterlistID) :
    FORTE_WagoBusAdapter(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_INIT(*this, 0),
    conn_QO(nullptr),
    conn_QI(*this, 0, var_QI),
    conn_MasterId(*this, 1, var_MasterId),
    conn_Index(*this, 2, var_Index) {
}

void FORTE_WagoBusAdapter_Socket::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      readData(0, var_QO, conn_QO);
      if (auto peer = static_cast<FORTE_WagoBusAdapter_Plug *>(getPeer()); peer) {
        peer->var_QO = var_QO;
      }
      break;
    }
    default: break;
  }
}

void FORTE_WagoBusAdapter_Socket::writeOutputData(const TEventID paEIID) {
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
CIEC_ANY *FORTE_WagoBusAdapter_Socket::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WagoBusAdapter_Socket::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_MasterId;
    case 2: return &var_Index;
  }
  return nullptr;
}

CEventConnection *FORTE_WagoBusAdapter_Socket::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INIT;
  }
  return nullptr;
}

CDataConnection **FORTE_WagoBusAdapter_Socket::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CDataConnection *FORTE_WagoBusAdapter_Socket::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_MasterId;
    case 2: return &conn_Index;
  }
  return nullptr;
}
