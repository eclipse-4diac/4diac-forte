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


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_TRAIN, STRID(RT_E_TRAIN))

const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmDataInputNames[] = {STRID(DT), STRID(N), STRID(Deadline), STRID(WCET)};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmDataInputTypeIds[] = {STRID(TIME), STRID(UINT), STRID(TIME), STRID(TIME)};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmDataOutputNames[] = {STRID(CV)};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmDataOutputTypeIds[] = {STRID(UINT)};
const TDataIOID FORTE_RT_E_TRAIN::scmEIWith[] = {0, 1, 2, 3, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_TRAIN::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmEventInputNames[] = {STRID(START), STRID(STOP)};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};
const TDataIOID FORTE_RT_E_TRAIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_TRAIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmEventOutputNames[] = {STRID(EO)};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_RT_E_TRAIN::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  4, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_RT_E_TRAIN::FORTE_RT_E_TRAIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_RT_E_CYCLE(STRID(RT_E_CYCLE), *this),
    fb_E_CTU(STRID(E_CTU), *this),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_Deadline(nullptr),
    conn_WCET(nullptr),
    conn_CV(this, 0, var_CV) {
};

void FORTE_RT_E_TRAIN::setInitialValues() {
  var_DT = 0_TIME;
  var_N = 0_UINT;
  var_Deadline = 0_TIME;
  var_WCET = 0_TIME;
  var_CV = 0_UINT;
}

const SCFB_FBInstanceData FORTE_RT_E_TRAIN::scmInternalFBs[] = {
  {STRID(RT_E_CYCLE), STRID(RT_E_CYCLE)},
  {STRID(E_CTU), STRID(E_CTU)},
  {STRID(E_SWITCH), STRID(E_SWITCH)}
};


const SCFB_FBConnectionData FORTE_RT_E_TRAIN::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(EO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(CU)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(R)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(STOP)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(RO)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(START)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(CUO)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EI)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO0)), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EO)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO1)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(STOP)), 0},
};

const SCFB_FBConnectionData FORTE_RT_E_TRAIN::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(DT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(DT)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Deadline)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(Deadline)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(WCET)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RT_E_CYCLE), STRID(WCET)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(PV)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(CV)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CV)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(G)), 2},
};

const SCFB_FBNData FORTE_RT_E_TRAIN::scmFBNData = {
  3, scmInternalFBs,
  7, scmEventConnections,
  0, nullptr,
  6, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr
};


void FORTE_RT_E_TRAIN::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, var_DT, conn_DT);
      readData(1, var_N, conn_N);
      readData(2, var_Deadline, conn_Deadline);
      readData(3, var_WCET, conn_WCET);
      break;
    }
    case scmEventSTOPID: {
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_TRAIN::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      writeData(0, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_TRAIN::readInternal2InterfaceOutputData(TEventID paEOID) {
  switch(paEOID) {
    case scmEventEOID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_CV); }
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
    case 2: return &var_Deadline;
    case 3: return &var_WCET;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_RT_E_TRAIN::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_TRAIN::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
    case 2: return &conn_Deadline;
    case 3: return &conn_WCET;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_TRAIN::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}

