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

const CStringDictionary::TStringId FORTE_E_TABLE::scmDataInputNames[] = {STRID(DT), STRID(N)};
const CStringDictionary::TStringId FORTE_E_TABLE::scmDataInputTypeIds[] = {STRID(ARRAY), static_cast<CStringDictionary::TStringId>(0), static_cast<CStringDictionary::TStringId>(3), STRID(TIME), STRID(UINT)};
const CStringDictionary::TStringId FORTE_E_TABLE::scmDataOutputNames[] = {STRID(CV)};
const CStringDictionary::TStringId FORTE_E_TABLE::scmDataOutputTypeIds[] = {STRID(UINT)};
const TDataIOID FORTE_E_TABLE::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TABLE::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_TABLE::scmEventInputNames[] = {STRID(START), STRID(STOP)};
const TDataIOID FORTE_E_TABLE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TABLE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TABLE::scmEventOutputNames[] = {STRID(EO)};
const SFBInterfaceSpec FORTE_E_TABLE::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_TABLE::FORTE_E_TABLE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_TABLE_CTRL(STRID(E_TABLE_CTRL), *this),
    fb_E_DELAY(STRID(E_DELAY), *this),
    var_DT(CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
    var_N(0_UINT),
    var_CV(0_UINT),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(this, 0, var_CV) {
};

void FORTE_E_TABLE::setInitialValues() {
  var_DT = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
  var_N = 0_UINT;
  var_CV = 0_UINT;
}

const SCFB_FBInstanceData FORTE_E_TABLE::scmInternalFBs[] = {
  {STRID(E_TABLE_CTRL), STRID(E_TABLE_CTRL)},
  {STRID(E_DELAY), STRID(E_DELAY)}
};

const SCFB_FBConnectionData FORTE_E_TABLE::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CLKO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EO)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(START)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_TABLE::scmFannedOutEventConnections[] = {
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CLK)), 0},
};

const SCFB_FBConnectionData FORTE_E_TABLE::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(DTO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(CV)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CV)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(DT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(DT)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TABLE_CTRL), STRID(N)), 0},
};

const SCFB_FBNData FORTE_E_TABLE::scmFBNData = {
  2, scmInternalFBs,
  4, scmEventConnections,
  1, scmFannedOutEventConnections,
  4, scmDataConnections,
  0, nullptr,
  0, nullptr
};

void FORTE_E_TABLE::readInternal2InterfaceOutputData(const TEventID paEOID) {
  switch(paEOID) {
    case scmEventEOID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_CV); }
      break;
    }
    default:
      break;
  }
}
void FORTE_E_TABLE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, var_DT, conn_DT);
      readData(1, var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TABLE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      writeData(0, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TABLE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TABLE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_TABLE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TABLE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TABLE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}
