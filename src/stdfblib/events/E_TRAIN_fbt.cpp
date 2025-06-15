/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TRAIN
 *** Description: Generate of a finite train of events seperated by the time DT
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2025-02-15/Franz Höpfinger -  HR Agrartechnik GmbH   - Bugfix
 *************************************************************************/

#include "E_TRAIN_fbt.h"

USE_STRING_ID(CTR);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(CV);
USE_STRING_ID(DLY);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_TRAIN);
USE_STRING_ID(G);
USE_STRING_ID(GATE);
USE_STRING_ID(N);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(RO);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(UINT);

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, STRID(E_TRAIN))

namespace {
  const auto cDataInputNames = std::array{STRID(DT), STRID(N)};
  const auto cDataOutputNames = std::array{STRID(CV)};
  const auto cEventInputNames = std::array{STRID(START), STRID(STOP)};
  const auto cEventOutputNames = std::array{STRID(EO)};
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
      {STRID(CTR), STRID(CUO), STRID(GATE), STRID(EI)},
      {STRID(CTR), STRID(RO), STRID(GATE), STRID(EI)},
      {CStringDictionary::scmInvalidStringId, STRID(START), STRID(CTR), STRID(R)},
      {STRID(GATE), STRID(EO0), STRID(DLY), STRID(START)},
      {CStringDictionary::scmInvalidStringId, STRID(STOP), STRID(DLY), STRID(STOP)},
      {STRID(DLY), STRID(EO), CStringDictionary::scmInvalidStringId, STRID(EO)},
      {STRID(DLY), STRID(EO), STRID(CTR), STRID(CU)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(N), STRID(CTR), STRID(PV)},
      {STRID(CTR), STRID(CV), CStringDictionary::scmInvalidStringId, STRID(CV)},
      {CStringDictionary::scmInvalidStringId, STRID(DT), STRID(DLY), STRID(DT)},
      {STRID(CTR), STRID(Q), STRID(GATE), STRID(G)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_TRAIN::FORTE_E_TRAIN(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_CTR(STRID(CTR), *this),
    fb_GATE(STRID(GATE), *this),
    fb_DLY(STRID(DLY), *this),
    conn_EO(*this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(*this, 0, 0_UINT),
    conn_if2in_DT(*this, 0, 0_TIME),
    conn_if2in_N(*this, 1, 0_UINT) {};

void FORTE_E_TRAIN::setInitialValues() {
  conn_if2in_DT.getValue() = 0_TIME;
  conn_if2in_N.getValue() = 0_UINT;
  fb_CTR->conn_CV.getValue() = 0_UINT;
}

void FORTE_E_TRAIN::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_DT.getValue(), conn_DT);
      readData(1, conn_if2in_N.getValue(), conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_E_TRAIN::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventEOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_CTR->conn_CV.getValue(), conn_CV);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_TRAIN::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT.getValue();
    case 1: return &conn_if2in_N.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TRAIN::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_CTR->conn_CV.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TRAIN::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TRAIN::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TRAIN::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT;
    case 1: return &conn_if2in_N;
  }
  return nullptr;
}
