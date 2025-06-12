/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
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
 *** Name: E_TABLE
 *** Description: Composite Function Block Type
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "E_TABLE_fbt.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(CLK);
USE_STRING_ID(CLKO);
USE_STRING_ID(CV);
USE_STRING_ID(DT);
USE_STRING_ID(DTO);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EO);
USE_STRING_ID(E_TABLE);
USE_STRING_ID(E_TABLE_CTRL);
USE_STRING_ID(N);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(UINT);

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_TABLE, STRID(E_TABLE))

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
} // namespace

FORTE_E_TABLE::FORTE_E_TABLE(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_TABLE_CTRL(STRID(E_TABLE_CTRL), *this),
    fb_E_DELAY(STRID(E_DELAY), *this),
    conn_EO(*this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(*this, 0, 0_UINT),
    conn_if2in_DT(*this, 0, CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
    conn_if2in_N(*this, 0, 0_UINT) {};

void FORTE_E_TABLE::setInitialValues() {
  conn_if2in_DT.getValue() = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
  conn_if2in_N.getValue() = 0_UINT;
  fb_E_TABLE_CTRL->conn_CV.getValue() = 0_UINT;
}

const SCFB_FBInstanceData FORTE_E_TABLE::scmInternalFBs[] = {{STRID(E_TABLE_CTRL), STRID(E_TABLE_CTRL)},
                                                             {STRID(E_DELAY), STRID(E_DELAY)}};

const SCFB_FBConnectionData FORTE_E_TABLE::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CLKO)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EO)), -1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CLK)), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(START)), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)),
     1},
};

const SCFB_FBConnectionData FORTE_E_TABLE::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(DTO)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CV)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CV)),
     -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(DT)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(DT)), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(N)),
     0},
};

const SCFB_FBNData FORTE_E_TABLE::scmFBNData = {
    2, scmInternalFBs, 5, scmEventConnections, 4, scmDataConnections, 0, nullptr, 0, nullptr};

void FORTE_E_TABLE::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_DT.getValue(), conn_DT);
      readData(1, conn_if2in_N.getValue(), conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_E_TABLE::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventEOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_TABLE_CTRL->conn_CV.getValue(), conn_CV);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_TABLE::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT.getValue();
    case 1: return &conn_if2in_N.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TABLE::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_TABLE_CTRL->conn_CV.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TABLE::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TABLE::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TABLE::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TABLE::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT;
    case 1: return &conn_if2in_N;
  }
  return nullptr;
}
