/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_F_TRIG_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CLK);
USE_STRING_ID(D);
USE_STRING_ID(E_D_FF);
USE_STRING_ID(E_F_TRIG);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(Event);
USE_STRING_ID(G);
USE_STRING_ID(Q);
USE_STRING_ID(QI);

DEFINE_FIRMWARE_FB(FORTE_E_F_TRIG, STRID(E_F_TRIG))

namespace {
  const auto cDataInputNames = std::array{STRID(QI)};
  const auto cEventInputNames = std::array{STRID(EI)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(EO)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(EI), STRID(E_D_FF), STRID(CLK)},
      {STRID(E_D_FF), STRID(EO), STRID(E_SWITCH), STRID(EI)},
      {STRID(E_SWITCH), STRID(EO0), CStringDictionary::scmInvalidStringId, STRID(EO)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(QI), STRID(E_D_FF), STRID(D)},
      {STRID(E_D_FF), STRID(Q), STRID(E_SWITCH), STRID(G)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_F_TRIG::FORTE_E_F_TRIG(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_E_D_FF(STRID(E_D_FF), *this),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    conn_EO(*this, 0),
    conn_QI(nullptr),
    conn_if2in_QI(*this, 0, 0_BOOL) {};

void FORTE_E_F_TRIG::setInitialValues() {
  conn_if2in_QI.getValue() = 0_BOOL;
}

void FORTE_E_F_TRIG::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventEIID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_E_F_TRIG::writeOutputData(TEventID) {
}

CIEC_ANY *FORTE_E_F_TRIG::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_QI.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_F_TRIG::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_F_TRIG::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_F_TRIG::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_E_F_TRIG::getDOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_F_TRIG::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_QI;
  }
  return nullptr;
}
