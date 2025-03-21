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


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_F_TRIG, STRID(E_F_TRIG))

const CStringDictionary::TStringId FORTE_E_F_TRIG::scmDataInputNames[] = {STRID(QI)};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scmDataInputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_E_F_TRIG::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_F_TRIG::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scmEventInputNames[] = {STRID(EI)};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scmEventInputTypeIds[] = {STRID(Event)};
const TForteInt16 FORTE_E_F_TRIG::scmEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scmEventOutputNames[] = {STRID(EO)};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_E_F_TRIG::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, nullptr, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_F_TRIG::FORTE_E_F_TRIG(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_D_FF(STRID(E_D_FF), *this),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    conn_EO(this, 0),
    conn_QI(nullptr),
    conn_if2in_QI(this, 0, 0_BOOL) {
};

void FORTE_E_F_TRIG::setInitialValues() {
  conn_if2in_QI.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_F_TRIG::scmInternalFBs[] = {
  {STRID(E_D_FF), STRID(E_D_FF)},
  {STRID(E_SWITCH), STRID(E_SWITCH)}
};


const SCFB_FBConnectionData FORTE_E_F_TRIG::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EI)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_D_FF), STRID(CLK)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_D_FF), STRID(EO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EI)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO0)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EO)), -1},
};

const SCFB_FBConnectionData FORTE_E_F_TRIG::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(QI)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_D_FF), STRID(D)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_D_FF), STRID(Q)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(G)), 1},
};

const SCFB_FBNData FORTE_E_F_TRIG::scmFBNData = {
  2, scmInternalFBs,
  3, scmEventConnections,
  0, nullptr,
  2, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr
};


void FORTE_E_F_TRIG::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEIID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_F_TRIG::writeOutputData(TEventID) {
}

CIEC_ANY *FORTE_E_F_TRIG::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_QI.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_F_TRIG::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_F_TRIG::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_F_TRIG::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_E_F_TRIG::getDOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_F_TRIG::getIf2InConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_QI;
  }
  return nullptr;
}


