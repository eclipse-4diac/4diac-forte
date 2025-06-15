/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "CFB_TEST.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CFB_TEST);
USE_STRING_ID(CHANGED);
USE_STRING_ID(CNF);
USE_STRING_ID(E_DEMUX_2);
USE_STRING_ID(EI);
USE_STRING_ID(EI1);
USE_STRING_ID(EI2);
USE_STRING_ID(E_MUX_2);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(EO2);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(Event);
USE_STRING_ID(G);
USE_STRING_ID(K);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PERMIT_OP);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(R);
USE_STRING_ID(RESET);
USE_STRING_ID(RESET_CHANGED);
USE_STRING_ID(S);
USE_STRING_ID(SET);
USE_STRING_ID(SET_CHANGED);

DEFINE_FIRMWARE_FB(FORTE_CFB_TEST, STRID(CFB_TEST))

namespace {
  const auto cDataInputNames = std::array{STRID(QI)};
  const auto cDataOutputNames = std::array{STRID(QO)};
  const auto cEventInputNames = std::array{STRID(SET), STRID(RESET)};
  const auto cEventOutputNames = std::array{STRID(CNF), STRID(CHANGED)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(SET), STRID(E_MUX_2), STRID(EI1)},
      {CStringDictionary::scmInvalidStringId, STRID(RESET), STRID(E_MUX_2), STRID(EI2)},
      {STRID(E_MUX_2), STRID(EO), STRID(PERMIT_OP), STRID(EI)},
      {STRID(SET_CHANGED), STRID(EO1), CStringDictionary::scmInvalidStringId, STRID(CNF)},
      {STRID(SET_CHANGED), STRID(EO0), STRID(E_SR), STRID(S)},
      {STRID(RESET_CHANGED), STRID(EO0), CStringDictionary::scmInvalidStringId, STRID(CNF)},
      {STRID(RESET_CHANGED), STRID(EO1), STRID(E_SR), STRID(R)},
      {STRID(PERMIT_OP), STRID(EO), STRID(E_DEMUX_2), STRID(EI)},
      {STRID(E_DEMUX_2), STRID(EO1), STRID(SET_CHANGED), STRID(EI)},
      {STRID(E_DEMUX_2), STRID(EO2), STRID(RESET_CHANGED), STRID(EI)},
      {STRID(E_SR), STRID(EO), CStringDictionary::scmInvalidStringId, STRID(CNF)},
      {STRID(E_SR), STRID(EO), CStringDictionary::scmInvalidStringId, STRID(CHANGED)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(QI), STRID(PERMIT_OP), STRID(PERMIT)},
      {STRID(E_SR), STRID(Q), CStringDictionary::scmInvalidStringId, STRID(QO)},
      {STRID(E_SR), STRID(Q), STRID(SET_CHANGED), STRID(G)},
      {STRID(E_SR), STRID(Q), STRID(RESET_CHANGED), STRID(G)},
      {STRID(E_MUX_2), STRID(K), STRID(E_DEMUX_2), STRID(K)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_CFB_TEST::FORTE_CFB_TEST(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_PERMIT_OP(STRID(PERMIT_OP), *this),
    fb_E_SR(STRID(E_SR), *this),
    fb_SET_CHANGED(STRID(SET_CHANGED), *this),
    fb_E_DEMUX_2(STRID(E_DEMUX_2), "E_DEMUX_2", *this),
    fb_E_MUX_2(STRID(E_MUX_2), "E_MUX_2", *this),
    fb_RESET_CHANGED(STRID(RESET_CHANGED), *this),
    conn_CNF(*this, 0),
    conn_CHANGED(*this, 1),
    conn_QI(nullptr),
    conn_QO(*this, 0, 0_BOOL),
    conn_if2in_QI(*this, 0, 0_BOOL) {};

void FORTE_CFB_TEST::setInitialValues() {
  conn_if2in_QI.getValue() = 0_BOOL;
  fb_E_SR->conn_Q.getValue() = 0_BOOL;
}

void FORTE_CFB_TEST::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSETID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    case scmEventRESETID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_CFB_TEST::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_SR->conn_Q.getValue(), conn_QO);
      break;
    }
    case scmEventCHANGEDID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_SR->conn_Q.getValue(), conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_CFB_TEST::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_QI.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_CFB_TEST::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_SR->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_CFB_TEST::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
    case 1: return &conn_CHANGED;
  }
  return nullptr;
}

CDataConnection **FORTE_CFB_TEST::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_CFB_TEST::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CDataConnection *FORTE_CFB_TEST::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_QI;
  }
  return nullptr;
}
