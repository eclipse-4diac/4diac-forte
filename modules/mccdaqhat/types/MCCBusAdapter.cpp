/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include "MCCBusAdapter.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  namespace {
    const forte::StringId cDataInputNames[] = {"QO"_STRID};
    const forte::StringId cDataOutputNames[] = {"QI"_STRID, "MasterId"_STRID, "Index"_STRID};
    const forte::StringId cEventInputNames[] = {"INITO"_STRID};
    const forte::StringId cEventInputTypeIds[] = {"EInit"_STRID};
    const forte::StringId cEventOutputNames[] = {"INIT"_STRID};
    const forte::StringId cEventOutputTypeIds[] = {"EInit"_STRID};

    const SFBInterfaceSpec scmFBInterfaceSpecSocket = {
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

    const SFBInterfaceSpec scmFBInterfaceSpecPlug = {
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

    const std::array<const TForteUInt8, 0> scmDeviceConfigurationIO{};
  } // namespace

  DEFINE_ADAPTER_TYPE(FORTE_MCCBusAdapter, "eclipse4diac::io::mccdaqhat::MCCBusAdapter"_STRID)

  FORTE_MCCBusAdapter::FORTE_MCCBusAdapter(CFBContainer &paContainer,
                                           const SFBInterfaceSpec &paInterfaceSpec,
                                           const forte::StringId paInstanceNameId,
                                           TForteUInt8 paParentAdapterlistID) :
      IOConfigFBMultiAdapter(
          scmDeviceConfigurationIO, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID) {
  }

  void FORTE_MCCBusAdapter::setInitialValues() {
  }

  FORTE_MCCBusAdapter_Plug::FORTE_MCCBusAdapter_Plug(forte::StringId paInstanceNameId,
                                                     CFBContainer &paContainer,
                                                     TForteUInt8 paParentAdapterlistID) :
      FORTE_MCCBusAdapter(paContainer, scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
      conn_INITO(*this, 0),
      conn_QI(nullptr),
      conn_MasterId(nullptr),
      conn_Index(nullptr),
      conn_QO(*this, 0, var_QO) {
  }

  void FORTE_MCCBusAdapter_Plug::readInputData(TEventID paEIID) {
    if (paEIID == scmEventINITID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_MasterId, conn_MasterId);
      readData(2, var_Index, conn_Index);
      if (getPeer() != nullptr) {
        getSocket()->var_QI = var_QI;
        getSocket()->var_MasterId = var_MasterId;
        getSocket()->var_Index = var_Index;
      }
    }
  }

  void FORTE_MCCBusAdapter_Plug::writeOutputData(TEventID paEIID) {
    if (paEIID == scmEventINITOID) {
      writeData(scmFBInterfaceSpecPlug.getNumDIs() + 0, var_QO, conn_QO);
    }
  }

  CEventConnection *FORTE_MCCBusAdapter_Plug::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_INITO : nullptr;
  }

  CIEC_ANY *FORTE_MCCBusAdapter_Plug::getDI(TPortId paDINum) {
    switch (paDINum) {
      case 0: return &var_QI;
      case 1: return &var_MasterId;
      case 2: return &var_Index;
    }
    return nullptr;
  }

  CDataConnection **FORTE_MCCBusAdapter_Plug::getDIConUnchecked(TPortId paDINum) {
    switch (paDINum) {
      case 0: return &conn_QI;
      case 1: return &conn_MasterId;
      case 2: return &conn_Index;
    }
    return nullptr;
  }

  CDataConnection *FORTE_MCCBusAdapter_Plug::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_QO : nullptr;
  }

  CIEC_ANY *FORTE_MCCBusAdapter_Plug::getDO(TPortId paDONum) {
    return (paDONum == 0) ? &var_QO : nullptr;
  }

  FORTE_MCCBusAdapter_Socket *FORTE_MCCBusAdapter_Plug::getSocket() {
    return static_cast<FORTE_MCCBusAdapter_Socket *>(getPeer());
  }

  FORTE_MCCBusAdapter_Socket::FORTE_MCCBusAdapter_Socket(forte::StringId paInstanceNameId,
                                                         CFBContainer &paContainer,
                                                         TForteUInt8 paParentAdapterlistID) :
      FORTE_MCCBusAdapter(paContainer, scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
      conn_QO(nullptr),
      conn_INIT(*this, 0),
      conn_QI(*this, 0, var_QI),
      conn_MasterId(*this, 1, var_MasterId),
      conn_Index(*this, 2, var_Index) {
  }

  void FORTE_MCCBusAdapter_Socket::readInputData(TEventID paEIID) {
    if (paEIID == scmEventINITOID) {
      readData(0, var_QO, conn_QO);
      if (getPeer() != nullptr) {
        getPlug()->var_QO = var_QO;
      }
    }
  }

  void FORTE_MCCBusAdapter_Socket::writeOutputData(TEventID paEIID) {
    if (paEIID == scmEventINITID) {
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 0, var_QI, conn_QI);
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 1, var_MasterId, conn_MasterId);
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 2, var_Index, conn_Index);
    }
  }

  CEventConnection *FORTE_MCCBusAdapter_Socket::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_INIT : nullptr;
  }

  CIEC_ANY *FORTE_MCCBusAdapter_Socket::getDI(TPortId paDINum) {
    return (paDINum == 0) ? &var_QO : nullptr;
  }

  CDataConnection **FORTE_MCCBusAdapter_Socket::getDIConUnchecked(TPortId paDINum) {
    return (paDINum == 0) ? &conn_QO : nullptr;
  }

  CDataConnection *FORTE_MCCBusAdapter_Socket::getDOConUnchecked(TPortId paDONum) {
    switch (paDONum) {
      case 0: return &conn_QI;
      case 1: return &conn_MasterId;
      case 2: return &conn_Index;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_MCCBusAdapter_Socket::getDO(TPortId paDONum) {
    switch (paDONum) {
      case 0: return &var_QI;
      case 1: return &var_MasterId;
      case 2: return &var_Index;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
