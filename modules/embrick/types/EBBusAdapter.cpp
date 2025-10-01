/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Alois Zoitl - Upgraded to new adapter architecture
 *******************************************************************************/

#include "EBBusAdapter.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::embrick {

  namespace {
    const forte::StringId cDataInputNames[] = {"QO"_STRID};
    const forte::StringId cDataOutputNames[] = {"QI"_STRID, "MasterId"_STRID, "Index"_STRID, "UpdateInterval"_STRID};
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

    const auto scmSlaveConfigurationIO = std::array<const TForteUInt8, 1>{3};

  } // namespace

  DEFINE_ADAPTER_TYPE(FORTE_EBBusAdapter, "eclipse4diac::io::embrick::EBBusAdapter"_STRID)

  FORTE_EBBusAdapter::FORTE_EBBusAdapter(CFBContainer &paContainer,
                                         const SFBInterfaceSpec &paInterfaceSpec,
                                         const forte::StringId paInstanceNameId,
                                         TForteUInt8 paParentAdapterlistID) :
      IOConfigFBMultiAdapter(
          scmSlaveConfigurationIO, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID) {
  }

  void FORTE_EBBusAdapter::setInitialValues() {
    var_UpdateInterval = 0_UINT;
  }

  FORTE_EBBusAdapter_Plug::FORTE_EBBusAdapter_Plug(forte::StringId paInstanceNameId,
                                                   CFBContainer &paContainer,
                                                   TForteUInt8 paParentAdapterlistID) :
      FORTE_EBBusAdapter(paContainer, scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
      conn_INITO(*this, 0),
      conn_QI(nullptr),
      conn_MasterId(nullptr),
      conn_MasterIndex(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO) {
  }

  void FORTE_EBBusAdapter_Plug::readInputData(TEventID paEIID) {
    if (paEIID == scmEventINITID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_MasterId, conn_MasterId);
      readData(2, var_Index, conn_MasterIndex);
      readData(3, var_UpdateInterval, conn_UpdateInterval);
      if (getPeer() != nullptr) {
        getSocket()->var_QI = var_QI;
        getSocket()->var_MasterId = var_MasterId;
        getSocket()->var_Index = var_Index;
        getSocket()->var_UpdateInterval = var_UpdateInterval;
      }
    }
  }

  void FORTE_EBBusAdapter_Plug::writeOutputData(TEventID paEIID) {
    if (paEIID == scmEventINITOID) {
      writeData(scmFBInterfaceSpecPlug.getNumDIs() + 0, var_QO, conn_QO);
    }
  }

  CEventConnection *FORTE_EBBusAdapter_Plug::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_INITO : nullptr;
  }

  CIEC_ANY *FORTE_EBBusAdapter_Plug::getDI(TPortId paDINum) {
    switch (paDINum) {
      case 0: return &var_QI; break;
      case 1: return &var_MasterId; break;
      case 2: return &var_Index; break;
      case 3: return &var_UpdateInterval; break;
    }
    return nullptr;
  }

  CDataConnection **FORTE_EBBusAdapter_Plug::getDIConUnchecked(TPortId paDINum) {
    switch (paDINum) {
      case 0: return &conn_QI; break;
      case 1: return &conn_MasterId; break;
      case 2: return &conn_MasterIndex; break;
      case 3: return &conn_UpdateInterval; break;
    }
    return nullptr;
  }

  CDataConnection *FORTE_EBBusAdapter_Plug::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_QO : nullptr;
  }

  CIEC_ANY *FORTE_EBBusAdapter_Plug::getDO(TPortId paDONum) {
    return (paDONum == 0) ? &var_QO : nullptr;
  }

  FORTE_EBBusAdapter_Socket *FORTE_EBBusAdapter_Plug::getSocket() {
    return static_cast<FORTE_EBBusAdapter_Socket *>(getPeer());
  }

  FORTE_EBBusAdapter_Socket::FORTE_EBBusAdapter_Socket(forte::StringId paInstanceNameId,
                                                       CFBContainer &paContainer,
                                                       TForteUInt8 paParentAdapterlistID) :
      FORTE_EBBusAdapter(paContainer, scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
      conn_QO(nullptr),
      conn_INIT(*this, 0),
      conn_QI(*this, 0, var_QI),
      conn_MasterId(*this, 1, var_MasterId),
      conn_MasterIndex(*this, 2, var_Index),
      conn_UpdateInterval(*this, 3, var_UpdateInterval) {
  }

  void FORTE_EBBusAdapter_Socket::readInputData(TEventID paEIID) {
    if (paEIID == scmEventINITOID) {
      readData(0, var_QO, conn_QO);
      if (getPeer() != nullptr) {
        getPlug()->var_QO = var_QO;
      }
    }
  }

  void FORTE_EBBusAdapter_Socket::writeOutputData(TEventID paEIID) {
    if (paEIID == scmEventINITID) {
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 0, var_QI, conn_QI);
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 1, var_MasterId, conn_MasterId);
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 2, var_Index, conn_MasterIndex);
      writeData(scmFBInterfaceSpecSocket.getNumDIs() + 3, var_UpdateInterval, conn_UpdateInterval);
    }
  }

  CEventConnection *FORTE_EBBusAdapter_Socket::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_INIT : nullptr;
  }

  CIEC_ANY *FORTE_EBBusAdapter_Socket::getDI(TPortId paDINum) {
    return (paDINum == 0) ? &var_QO : nullptr;
  }

  CDataConnection **FORTE_EBBusAdapter_Socket::getDIConUnchecked(TPortId paDINum) {
    return (paDINum == 0) ? &conn_QO : nullptr;
  }

  CDataConnection *FORTE_EBBusAdapter_Socket::getDOConUnchecked(TPortId paDONum) {
    switch (paDONum) {
      case 0: return &conn_QI; break;
      case 1: return &conn_MasterId; break;
      case 2: return &conn_MasterIndex; break;
      case 3: return &conn_UpdateInterval; break;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_EBBusAdapter_Socket::getDO(TPortId paDONum) {
    switch (paDONum) {
      case 0: return &var_QI; break;
      case 1: return &var_MasterId; break;
      case 2: return &var_Index; break;
      case 3: return &var_UpdateInterval; break;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::io::embrick
