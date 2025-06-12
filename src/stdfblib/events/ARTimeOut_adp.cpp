/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "ARTimeOut_adp.h"

#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

using namespace std::literals;

USE_STRING_ID(ARTimeOut);
USE_STRING_ID(DT);
USE_STRING_ID(Event);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(TimeOut);

DEFINE_ADAPTER_TYPE(FORTE_ARTimeOut, STRID(ARTimeOut))

namespace {
  const auto cDataOutputNames = std::array{STRID(DT)};
  const auto cEventInputNames = std::array{STRID(TimeOut)};
  const auto cEventOutputNames = std::array{STRID(START), STRID(STOP)};

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

FORTE_ARTimeOut::FORTE_ARTimeOut(forte::core::CFBContainer &paContainer,
                                 const SFBInterfaceSpec &paInterfaceSpec,
                                 const CStringDictionary::TStringId paInstanceNameId,
                                 TForteUInt8 paParentAdapterlistID) :
    CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
    var_DT(0_TIME) {
}

void FORTE_ARTimeOut::setInitialValues() {
  forte::CAdapter::setInitialValues();
  var_DT = 0_TIME;
}

FORTE_ARTimeOut_Plug::FORTE_ARTimeOut_Plug(CStringDictionary::TStringId paInstanceNameId,
                                           forte::core::CFBContainer &paContainer,
                                           TForteUInt8 paParentAdapterlistID) :
    FORTE_ARTimeOut(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_TimeOut(*this, 0),
    conn_DT(nullptr) {
}

void FORTE_ARTimeOut_Plug::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, var_DT, conn_DT);
      if (auto peer = static_cast<FORTE_ARTimeOut_Socket *>(getPeer()); peer) {
        peer->var_DT = var_DT;
      }
      break;
    }
    default: break;
  }
}

void FORTE_ARTimeOut_Plug::writeOutputData(TEventID) {
  // nothing to do
}
CIEC_ANY *FORTE_ARTimeOut_Plug::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_DT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ARTimeOut_Plug::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_ARTimeOut_Plug::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_TimeOut;
  }
  return nullptr;
}

CDataConnection **FORTE_ARTimeOut_Plug::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
  }
  return nullptr;
}

CDataConnection *FORTE_ARTimeOut_Plug::getDOConUnchecked(TPortId) {
  return nullptr;
}

FORTE_ARTimeOut_Socket::FORTE_ARTimeOut_Socket(CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer,
                                               TForteUInt8 paParentAdapterlistID) :
    FORTE_ARTimeOut(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_START(*this, 0),
    conn_STOP(*this, 1),
    conn_DT(*this, 0, var_DT) {
}

void FORTE_ARTimeOut_Socket::readInputData(TEventID) {
  // nothing to do
}

void FORTE_ARTimeOut_Socket::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      writeData(0, var_DT, conn_DT);
      break;
    }
    default: break;
  }
}
CIEC_ANY *FORTE_ARTimeOut_Socket::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_ARTimeOut_Socket::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_DT;
  }
  return nullptr;
}

CEventConnection *FORTE_ARTimeOut_Socket::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_START;
    case 1: return &conn_STOP;
  }
  return nullptr;
}

CDataConnection **FORTE_ARTimeOut_Socket::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_ARTimeOut_Socket::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
  }
  return nullptr;
}
