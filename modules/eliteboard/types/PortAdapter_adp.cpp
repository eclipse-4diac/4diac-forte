/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include "PortAdapter_adp.h"

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;

using namespace forte::literals;

namespace {
  const auto cEventInputNames = std::array{"MAPO"_STRID};
  const auto cEventOutputNames = std::array{"MAP"_STRID};
  const auto cDataOutputNames = std::array{"GPIO_Port_Addr"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpecSocket = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  const SFBInterfaceSpec cFBInterfaceSpecPlug = {
      .mEINames = cEventOutputNames,
      .mEITypeNames = {},
      .mEONames = cEventInputNames,
      .mEOTypeNames = {},
      .mDINames = cDataOutputNames,
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

DEFINE_ADAPTER_TYPE(FORTE_PortAdapter, "PortAdapter"_STRID)

FORTE_PortAdapter::FORTE_PortAdapter(CFBContainer &paContainer,
                                     const SFBInterfaceSpec &paInterfaceSpec,
                                     const forte::StringId paInstanceNameId,
                                     TForteUInt8 paParentAdapterlistID) :
    CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
    var_GPIO_Port_Addr(0_DWORD) {
}

void FORTE_PortAdapter::setInitialValues() {
  CAdapter::setInitialValues();
  var_GPIO_Port_Addr = 0_DWORD;
}

FORTE_PortAdapter_Plug::FORTE_PortAdapter_Plug(forte::StringId paInstanceNameId,
                                               CFBContainer &paContainer,
                                               TForteUInt8 paParentAdapterlistID) :
    FORTE_PortAdapter(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_MAPO(*this, 0),
    conn_GPIO_Port_Addr(nullptr) {
}

void FORTE_PortAdapter_Plug::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(0, var_GPIO_Port_Addr, conn_GPIO_Port_Addr);
      if (auto peer = static_cast<FORTE_PortAdapter_Socket *>(getPeer()); peer) {
        peer->var_GPIO_Port_Addr = var_GPIO_Port_Addr;
      }
      break;
    }
    default: break;
  }
}

void FORTE_PortAdapter_Plug::writeOutputData(TEventID) {
  // nothing to do
}
CIEC_ANY *FORTE_PortAdapter_Plug::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_GPIO_Port_Addr;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PortAdapter_Plug::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_PortAdapter_Plug::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
  }
  return nullptr;
}

CDataConnection **FORTE_PortAdapter_Plug::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_GPIO_Port_Addr;
  }
  return nullptr;
}

CDataConnection *FORTE_PortAdapter_Plug::getDOConUnchecked(TPortId) {
  return nullptr;
}

FORTE_PortAdapter_Socket::FORTE_PortAdapter_Socket(forte::StringId paInstanceNameId,
                                                   CFBContainer &paContainer,
                                                   TForteUInt8 paParentAdapterlistID) :
    FORTE_PortAdapter(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_MAP(*this, 0),
    conn_GPIO_Port_Addr(*this, 0, var_GPIO_Port_Addr) {
}

void FORTE_PortAdapter_Socket::readInputData(TEventID) {
  // nothing to do
}

void FORTE_PortAdapter_Socket::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      writeData(0, var_GPIO_Port_Addr, conn_GPIO_Port_Addr);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_PortAdapter_Socket::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_PortAdapter_Socket::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_GPIO_Port_Addr;
  }
  return nullptr;
}

CEventConnection *FORTE_PortAdapter_Socket::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAP;
  }
  return nullptr;
}

CDataConnection **FORTE_PortAdapter_Socket::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_PortAdapter_Socket::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_GPIO_Port_Addr;
  }
  return nullptr;
}
