/*******************************************************************************
 * Copyright (c) 2012 - 2024 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Michael Gafert - moved common functions to super class, changed to new FB style
 *******************************************************************************/

#include "X20AT2402_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CNID);
USE_STRING_ID(CNIDO);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MODID);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REAL);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(T01);
USE_STRING_ID(T02);
USE_STRING_ID(UINT);
USE_STRING_ID(USINT);
USE_STRING_ID(X20AT2402);

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_X20AT2402, STRID(X20AT2402))

const CStringDictionary::TStringId FORTE_X20AT2402::scmDataInputNames[] = {STRID(QI), STRID(CNID), STRID(MODID)};
const CStringDictionary::TStringId FORTE_X20AT2402::scmDataInputTypeIds[] = {STRID(BOOL), STRID(USINT), STRID(UINT)};
const CStringDictionary::TStringId FORTE_X20AT2402::scmDataOutputNames[] = {STRID(QO), STRID(CNIDO), STRID(STATUS),
                                                                            STRID(T01), STRID(T02)};
const CStringDictionary::TStringId FORTE_X20AT2402::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(USINT), STRID(STRING),
                                                                              STRID(REAL), STRID(REAL)};
const TDataIOID FORTE_X20AT2402::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20AT2402::scmEIWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20AT2402::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_X20AT2402::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_X20AT2402::scmEOWith[] = {0, 1, 2, scmWithListDelimiter, 2, 3, 4, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20AT2402::scmEOWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20AT2402::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_X20AT2402::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_X20AT2402::scmFBInterfaceSpec = {2,
                                                              scmEventInputNames,
                                                              scmEventInputTypeIds,
                                                              scmEIWith,
                                                              scmEIWithIndexes,
                                                              2,
                                                              scmEventOutputNames,
                                                              scmEventOutputTypeIds,
                                                              scmEOWith,
                                                              scmEOWithIndexes,
                                                              3,
                                                              scmDataInputNames,
                                                              scmDataInputTypeIds,
                                                              5,
                                                              scmDataOutputNames,
                                                              scmDataOutputTypeIds,
                                                              0,
                                                              nullptr,
                                                              0,
                                                              nullptr};

FORTE_X20AT2402::FORTE_X20AT2402(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    PowerlinkFunctionBlockAT(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_INITO(*this, 0),
    conn_CNF(*this, 1),
    conn_QI(nullptr),
    conn_CNID(nullptr),
    conn_MODID(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_CNIDO(*this, 1, var_CNIDO),
    conn_STATUS(*this, 2, var_STATUS),
    conn_T01(*this, 3, var_T01),
    conn_T02(*this, 4, var_T02) {};

void FORTE_X20AT2402::setInitialValues() {
  var_QI = 0_BOOL;
  var_CNID = 0_USINT;
  var_MODID = 0_UINT;
  var_QO = 0_BOOL;
  var_CNIDO = 0_USINT;
  var_STATUS = ""_STRING;
  var_T01 = 0_REAL;
  var_T02 = 0_REAL;
}

void FORTE_X20AT2402::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  executePowerlinkEvent(paEIID, paECET, scmEventINITID, scmEventREQID, scmEventINITOID, scmEventCNFID, var_QI, var_QO,
                        var_CNID, var_MODID);
}

void FORTE_X20AT2402::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_CNID, conn_CNID);
      readData(2, var_MODID, conn_MODID);
      break;
    }
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_X20AT2402::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_CNIDO, conn_CNIDO);
      writeData(2, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventCNFID: {
      writeData(2, var_STATUS, conn_STATUS);
      writeData(3, var_T01, conn_T01);
      writeData(4, var_T02, conn_T02);
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_X20AT2402::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_CNID;
    case 2: return &var_MODID;
  }
  return nullptr;
}

CIEC_ANY *FORTE_X20AT2402::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_CNIDO;
    case 2: return &var_STATUS;
    case 3: return &var_T01;
    case 4: return &var_T02;
  }
  return nullptr;
}

CEventConnection *FORTE_X20AT2402::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_X20AT2402::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_CNID;
    case 2: return &conn_MODID;
  }
  return nullptr;
}

CDataConnection *FORTE_X20AT2402::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_CNIDO;
    case 2: return &conn_STATUS;
    case 3: return &conn_T01;
    case 4: return &conn_T02;
  }
  return nullptr;
}
