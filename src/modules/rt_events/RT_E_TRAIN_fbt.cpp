/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "RT_E_TRAIN_fbt.h"

USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(CV);
USE_STRING_ID(Deadline);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(Event);
USE_STRING_ID(G);
USE_STRING_ID(N);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(RO);
USE_STRING_ID(RT_E_CYCLE);
USE_STRING_ID(RT_E_TRAIN);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(UINT);
USE_STRING_ID(WCET);

DEFINE_FIRMWARE_FB(FORTE_RT_E_TRAIN, STRID(RT_E_TRAIN))

namespace {
  const auto cDataInputNames = std::array{STRID(DT), STRID(N), STRID(Deadline), STRID(WCET)};
  const auto cDataOutputNames = std::array{STRID(CV)};
  const auto cEventInputNames = std::array{STRID(START), STRID(STOP)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(EO)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
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

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {STRID(RT_E_CYCLE), STRID(EO), STRID(E_CTU), STRID(CU)},
      {CStringDictionary::scmInvalidStringId, STRID(START), STRID(E_CTU), STRID(R)},
      {CStringDictionary::scmInvalidStringId, STRID(STOP), STRID(RT_E_CYCLE), STRID(STOP)},
      {STRID(E_CTU), STRID(RO), STRID(RT_E_CYCLE), STRID(START)},
      {STRID(E_CTU), STRID(CUO), STRID(E_SWITCH), STRID(EI)},
      {STRID(E_SWITCH), STRID(EO0), CStringDictionary::scmInvalidStringId, STRID(EO)},
      {STRID(E_SWITCH), STRID(EO1), STRID(RT_E_CYCLE), STRID(STOP)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(DT), STRID(RT_E_CYCLE), STRID(DT)},
      {CStringDictionary::scmInvalidStringId, STRID(Deadline), STRID(RT_E_CYCLE), STRID(Deadline)},
      {CStringDictionary::scmInvalidStringId, STRID(WCET), STRID(RT_E_CYCLE), STRID(WCET)},
      {CStringDictionary::scmInvalidStringId, STRID(N), STRID(E_CTU), STRID(PV)},
      {STRID(E_CTU), STRID(CV), CStringDictionary::scmInvalidStringId, STRID(CV)},
      {STRID(E_CTU), STRID(Q), STRID(E_SWITCH), STRID(G)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_RT_E_TRAIN::FORTE_RT_E_TRAIN(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_RT_E_CYCLE(STRID(RT_E_CYCLE), *this),
    fb_E_CTU(STRID(E_CTU), *this),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    conn_EO(*this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_Deadline(nullptr),
    conn_WCET(nullptr),
    conn_CV(*this, 0, 0_UINT),
    conn_if2in_DT(*this, 0, 0_TIME),
    conn_if2in_N(*this, 1, 0_UINT),
    conn_if2in_Deadline(*this, 2, 0_TIME),
    conn_if2in_WCET(*this, 3, 0_TIME) {};

void FORTE_RT_E_TRAIN::setInitialValues() {
  conn_if2in_DT.getValue() = 0_TIME;
  conn_if2in_N.getValue() = 0_UINT;
  conn_if2in_Deadline.getValue() = 0_TIME;
  conn_if2in_WCET.getValue() = 0_TIME;
  fb_E_CTU->conn_CV.getValue() = 0_UINT;
}

void FORTE_RT_E_TRAIN::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_DT.getValue(), conn_DT);
      readData(1, conn_if2in_N.getValue(), conn_N);
      readData(2, conn_if2in_Deadline.getValue(), conn_Deadline);
      readData(3, conn_if2in_WCET.getValue(), conn_WCET);
      break;
    }
    case scmEventSTOPID: {
      break;
    }
    default: break;
  }
}

void FORTE_RT_E_TRAIN::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventEOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_CTU->conn_CV.getValue(), conn_CV);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT.getValue();
    case 1: return &conn_if2in_N.getValue();
    case 2: return &conn_if2in_Deadline.getValue();
    case 3: return &conn_if2in_WCET.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_CTU->conn_CV.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_RT_E_TRAIN::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_TRAIN::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
    case 2: return &conn_Deadline;
    case 3: return &conn_WCET;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_TRAIN::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT;
    case 1: return &conn_if2in_N;
    case 2: return &conn_if2in_Deadline;
    case 3: return &conn_if2in_WCET;
  }
  return nullptr;
}
