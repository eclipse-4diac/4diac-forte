/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "ATimeOut_adp.h"
#include "eventconn.h"

USE_STRING_ID(ATimeOut);
USE_STRING_ID(DT);
USE_STRING_ID(Event);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(TimeOut);

namespace {
  const CStringDictionary::TStringId scmDataOutputNames[] = {STRID(DT)};
  const CStringDictionary::TStringId scmDataOutputTypeIds[] = {STRID(TIME)};
  const TForteInt16 scmEIWithIndexes[] = {-1};
  const CStringDictionary::TStringId scmEventInputNames[] = {STRID(TimeOut)};
  const CStringDictionary::TStringId scmEventInputTypeIds[] = {STRID(Event)};
  const TDataIOID scmEOWith[] = {0, CFunctionBlock::scmWithListDelimiter};
  const TForteInt16 scmEOWithIndexes[] = {0, -1};
  const CStringDictionary::TStringId scmEventOutputNames[] = {STRID(START), STRID(STOP)};
  const CStringDictionary::TStringId scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

  const SFBInterfaceSpec scmFBInterfaceSpecSocket = {1,
                                                     scmEventInputNames,
                                                     scmEventInputTypeIds,
                                                     nullptr,
                                                     scmEIWithIndexes,
                                                     2,
                                                     scmEventOutputNames,
                                                     scmEventOutputTypeIds,
                                                     scmEOWith,
                                                     scmEOWithIndexes,
                                                     0,
                                                     nullptr,
                                                     nullptr,
                                                     1,
                                                     scmDataOutputNames,
                                                     scmDataOutputTypeIds,
                                                     0,
                                                     nullptr,
                                                     0,
                                                     nullptr};

  const SFBInterfaceSpec scmFBInterfaceSpecPlug = {2,
                                                   scmEventOutputNames,
                                                   scmEventOutputTypeIds,
                                                   scmEOWith,
                                                   scmEOWithIndexes,
                                                   1,
                                                   scmEventInputNames,
                                                   scmEventInputTypeIds,
                                                   nullptr,
                                                   scmEIWithIndexes,
                                                   1,
                                                   scmDataOutputNames,
                                                   scmDataOutputTypeIds,
                                                   0,
                                                   nullptr,
                                                   nullptr,
                                                   0,
                                                   nullptr,
                                                   0,
                                                   nullptr};
} // namespace

DEFINE_ADAPTER_TYPE(FORTE_ATimeOut, STRID(ATimeOut))

void FORTE_ATimeOut::setInitialValues() {
  var_DT = 0_TIME;
}

FORTE_ATimeOut::FORTE_ATimeOut(forte::core::CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               const CStringDictionary::TStringId paInstanceNameId,
                               TForteUInt8 paParentAdapterlistID) :
    CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID) {
}

FORTE_ATimeOut_Plug::FORTE_ATimeOut_Plug(CStringDictionary::TStringId paInstanceNameId,
                                         forte::core::CFBContainer &paContainer,
                                         TForteUInt8 paParentAdapterlistID) :
    FORTE_ATimeOut(paContainer, scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
    conn_TimeOUT(*this, 0) {
}

void FORTE_ATimeOut_Plug::readInputData(TEventID paEIID) {
  if (paEIID == scmEventSTARTID) {
    readData(0, var_DT, conn_DT);
    if (getPeer() != nullptr) {
      getSocket()->var_DT = var_DT;
    }
  }
}
void FORTE_ATimeOut_Plug::writeOutputData(TEventID) {
  // nothing to do for this plug
}

CEventConnection *FORTE_ATimeOut_Plug::getEOConUnchecked(TPortId paEONum) {
  return (paEONum == 0) ? &conn_TimeOUT : nullptr;
}

CIEC_ANY *FORTE_ATimeOut_Plug::getDI(TPortId paDINum) {
  return (paDINum == 0) ? &var_DT : nullptr;
}

CDataConnection **FORTE_ATimeOut_Plug::getDIConUnchecked(TPortId paDINum) {
  return (paDINum == 0) ? &conn_DT : nullptr;
}

CDataConnection *FORTE_ATimeOut_Plug::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_ATimeOut_Plug::getDO(TPortId) {
  return nullptr;
}

FORTE_ATimeOut_Socket *FORTE_ATimeOut_Plug::getSocket() {
  return static_cast<FORTE_ATimeOut_Socket *>(getPeer());
}

FORTE_ATimeOut_Socket::FORTE_ATimeOut_Socket(CStringDictionary::TStringId paInstanceNameId,
                                             forte::core::CFBContainer &paContainer,
                                             TForteUInt8 paParentAdapterlistID) :
    FORTE_ATimeOut(paContainer, scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
    conn_START(*this, 0),
    conn_STOP(*this, 1),
    conn_DT(*this, 0, var_DT) {
}

void FORTE_ATimeOut_Socket::readInputData(TEventID) {
  // nothing to be done for socket
}

void FORTE_ATimeOut_Socket::writeOutputData(TEventID paEIID) {
  if (paEIID == scmEventSTARTID) {
    writeData(0, var_DT, conn_DT);
  }
}

CEventConnection *FORTE_ATimeOut_Socket::getEOConUnchecked(TPortId paEONum) {
  switch (paEONum) {
    case 0: return &conn_START; break;
    case 1: return &conn_STOP; break;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ATimeOut_Socket::getDI(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_ATimeOut_Socket::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_ATimeOut_Socket::getDOConUnchecked(TPortId paDONum) {
  return (paDONum == 0) ? &conn_DT : nullptr;
}

CIEC_ANY *FORTE_ATimeOut_Socket::getDO(TPortId paDONum) {
  return (paDONum == 0) ? &var_DT : nullptr;
}
