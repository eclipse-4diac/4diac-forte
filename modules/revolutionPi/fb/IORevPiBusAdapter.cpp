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

using namespace forte::literals;

namespace forte::eclipse4diac::io::revpi {

  DEFINE_ADAPTER_TYPE(FORTE_IORevPiBusAdapter, "eclipse4diac::io::revpi::IORevPiBusAdapter"_STRID)

  namespace {

    const auto cDataInputNames = std::array{"QO"_STRID};
    const auto cDataOutputNames = std::array{"QI"_STRID, "MasterId"_STRID, "Index"_STRID};
    const auto cEventInputNames = std::array{"INITO"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"INIT"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

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

  FORTE_IORevPiBusAdapter::FORTE_IORevPiBusAdapter(CFBContainer &paContainer,
                                                   const SFBInterfaceSpec &paInterfaceSpec,
                                                   const forte::StringId paInstanceNameId,
                                                   TForteUInt8 paParentAdapterlistID) :
      ::forte::io::IOConfigFBMultiAdapter({}, paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
      var_QO(0_BOOL),
      var_QI(0_BOOL),
      var_MasterId(0_UINT),
      var_Index(0_UINT) {
  }

  void FORTE_IORevPiBusAdapter::setInitialValues() {
    CAdapter::setInitialValues();
    var_QO = 0_BOOL;
    var_QI = 0_BOOL;
    var_MasterId = 0_UINT;
    var_Index = 0_UINT;
  }

  FORTE_IORevPiBusAdapter_Plug::FORTE_IORevPiBusAdapter_Plug(forte::StringId paInstanceNameId,
                                                             CFBContainer &paContainer,
                                                             TForteUInt8 paParentAdapterlistID) :
      FORTE_IORevPiBusAdapter(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
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

  FORTE_IORevPiBusAdapter_Socket::FORTE_IORevPiBusAdapter_Socket(forte::StringId paInstanceNameId,
                                                                 CFBContainer &paContainer,
                                                                 TForteUInt8 paParentAdapterlistID) :
      FORTE_IORevPiBusAdapter(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
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
} // namespace forte::eclipse4diac::io::revpi
