/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "eGenAdapter_adp.h"
#include "adapter.h"

USE_STRING_ID(eGenAdapter);

namespace {
  const SFBInterfaceSpec scmFBInterfaceSpecSocket = {0,       nullptr, nullptr, nullptr, nullptr, 0,       nullptr,
                                                     nullptr, nullptr, nullptr, 0,       nullptr, nullptr, 0,
                                                     nullptr, nullptr, 0,       nullptr, 0,       nullptr};

  const SFBInterfaceSpec scmFBInterfaceSpecPlug = {0,       nullptr, nullptr, nullptr, nullptr, 0,       nullptr,
                                                   nullptr, nullptr, nullptr, 0,       nullptr, nullptr, 0,
                                                   nullptr, nullptr, 0,       nullptr, 0,       nullptr};

} // namespace

DEFINE_ADAPTER_TYPE(FORTE_eGenAdapter, STRID(eGenAdapter))

void FORTE_eGenAdapter::setInitialValues() {
  forte::CAdapter::setInitialValues();
}

FORTE_eGenAdapter_Plug::FORTE_eGenAdapter_Plug(CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer,
                                               TForteUInt8 paParentAdapterlistID) :
    FORTE_eGenAdapter(paContainer, scmFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID) {
}

void FORTE_eGenAdapter_Plug::readInputData(TEventID) {
  // no pins nothing to do
}

void FORTE_eGenAdapter_Plug::writeOutputData(TEventID) {
  // no pins nothing to do
}

CEventConnection *FORTE_eGenAdapter_Plug::getEOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_eGenAdapter_Plug::getDI(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_eGenAdapter_Plug::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_eGenAdapter_Plug::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_eGenAdapter_Plug::getDO(TPortId) {
  return nullptr;
}

FORTE_eGenAdapter_Socket *FORTE_eGenAdapter_Plug::getSocket() {
  return static_cast<FORTE_eGenAdapter_Socket *>(getPeer());
}

FORTE_eGenAdapter_Socket::FORTE_eGenAdapter_Socket(CStringDictionary::TStringId paInstanceNameId,
                                                   forte::core::CFBContainer &paContainer,
                                                   TForteUInt8 paParentAdapterlistID) :
    FORTE_eGenAdapter(paContainer, scmFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID) {
}

void FORTE_eGenAdapter_Socket::readInputData(TEventID) {
  // no pins nothing to do
}

void FORTE_eGenAdapter_Socket::writeOutputData(TEventID) {
  // no pins nothing to do
}

CEventConnection *FORTE_eGenAdapter_Socket::getEOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_eGenAdapter_Socket::getDI(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_eGenAdapter_Socket::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_eGenAdapter_Socket::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_eGenAdapter_Socket::getDO(TPortId) {
  return nullptr;
}
