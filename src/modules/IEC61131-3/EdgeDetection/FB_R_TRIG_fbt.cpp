/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "FB_R_TRIG_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CLK);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(FB_R_TRIG);
USE_STRING_ID(MEM);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);

#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_R_TRIG, STRID(FB_R_TRIG))

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmDataInputNames[] = {STRID(CLK)};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmDataInputTypeIds[] = {STRID(BOOL)};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmDataOutputNames[] = {STRID(Q)};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmDataOutputTypeIds[] = {STRID(BOOL)};

const TDataIOID FORTE_FB_R_TRIG::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FB_R_TRIG::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FB_R_TRIG::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FB_R_TRIG::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FB_R_TRIG::scmFBInterfaceSpec = {1,
                                                              scmEventInputNames,
                                                              scmEventInputTypeIds,
                                                              scmEIWith,
                                                              scmEIWithIndexes,
                                                              1,
                                                              scmEventOutputNames,
                                                              scmEventOutputTypeIds,
                                                              scmEOWith,
                                                              scmEOWithIndexes,
                                                              1,
                                                              scmDataInputNames,
                                                              scmDataInputTypeIds,
                                                              1,
                                                              scmDataOutputNames,
                                                              scmDataOutputTypeIds,
                                                              0,
                                                              nullptr,
                                                              0,
                                                              nullptr};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmInternalsNames[] = {STRID(MEM)};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scmInternalsTypeIds[] = {STRID(BOOL)};
const SInternalVarsInformation FORTE_FB_R_TRIG::scmInternalVars = {1, scmInternalsNames, scmInternalsTypeIds};

FORTE_FB_R_TRIG::FORTE_FB_R_TRIG(CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, &scmInternalVars),
    var_MEM(false_BOOL),
    var_CLK(false_BOOL),
    var_Q(false_BOOL),
    conn_CNF(*this, 0),
    conn_CLK(nullptr),
    conn_Q(*this, 0, var_Q) {
}

void FORTE_FB_R_TRIG::alg_REQ(void) {

  var_Q = func_AND<CIEC_BOOL>(var_CLK, func_NOT<CIEC_BOOL>(var_MEM));
  var_MEM = var_CLK;
}

void FORTE_FB_R_TRIG::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_R_TRIG::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_CLK, conn_CLK);
      break;
    }
    default: break;
  }
}

void FORTE_FB_R_TRIG::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_Q, conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_FB_R_TRIG::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_CLK;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_R_TRIG::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_R_TRIG::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_R_TRIG::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CLK;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_R_TRIG::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_R_TRIG::getVarInternal(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_MEM;
  }
  return nullptr;
}
