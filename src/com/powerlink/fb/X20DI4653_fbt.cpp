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

#include "X20DI4653_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CNID);
USE_STRING_ID(CNIDO);
USE_STRING_ID(DI01);
USE_STRING_ID(DI02);
USE_STRING_ID(DI03);
USE_STRING_ID(DI04);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MODID);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(UINT);
USE_STRING_ID(USINT);
USE_STRING_ID(X20DI4653);


DEFINE_FIRMWARE_FB(FORTE_X20DI4653, STRID(X20DI4653))

const CStringDictionary::TStringId FORTE_X20DI4653::scmDataInputNames[] = {STRID(QI), STRID(CNID), STRID(MODID)};
const CStringDictionary::TStringId FORTE_X20DI4653::scmDataInputTypeIds[] = {STRID(BOOL), STRID(USINT), STRID(UINT)};
const CStringDictionary::TStringId FORTE_X20DI4653::scmDataOutputNames[] = {
    STRID(QO), STRID(CNIDO), STRID(STATUS), STRID(DI01), STRID(DI02), STRID(DI03), STRID(DI04)};
const CStringDictionary::TStringId FORTE_X20DI4653::scmDataOutputTypeIds[] = {
    STRID(BOOL), STRID(USINT), STRID(STRING), STRID(BOOL), STRID(BOOL), STRID(BOOL), STRID(BOOL)};
const TDataIOID FORTE_X20DI4653::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI4653::scmEIWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_X20DI4653::scmEOWith[] = {0, 2, 1, scmWithListDelimiter, 3, 4, 5, 6, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI4653::scmEOWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_X20DI4653::scmFBInterfaceSpec = {2,
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
                                                              7,
                                                              scmDataOutputNames,
                                                              scmDataOutputTypeIds,
                                                              0,
                                                              nullptr,
                                                              0,
                                                              nullptr};

FORTE_X20DI4653::FORTE_X20DI4653(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    PowerlinkFunctionBlockDI(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_INITO(*this, 0),
    conn_CNF(*this, 1),
    conn_QI(nullptr),
    conn_CNID(nullptr),
    conn_MODID(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_CNIDO(*this, 1, var_CNIDO),
    conn_STATUS(*this, 2, var_STATUS),
    conn_DI01(*this, 3, var_DI01),
    conn_DI02(*this, 4, var_DI02),
    conn_DI03(*this, 5, var_DI03),
    conn_DI04(*this, 6, var_DI04) {};

void FORTE_X20DI4653::setInitialValues() {
  var_QI = 0_BOOL;
  var_CNID = 0_USINT;
  var_MODID = 0_UINT;
  var_QO = 0_BOOL;
  var_CNIDO = 0_USINT;
  var_STATUS = ""_STRING;
  var_DI01 = 0_BOOL;
  var_DI02 = 0_BOOL;
  var_DI03 = 0_BOOL;
  var_DI04 = 0_BOOL;
}

void FORTE_X20DI4653::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  executePowerlinkEvent(paEIID, paECET, scmEventINITID, scmEventREQID, scmEventINITOID, scmEventCNFID, var_QI, var_QO,
                        var_CNID, var_MODID);
}

void FORTE_X20DI4653::readInputData(const TEventID paEIID) {
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

void FORTE_X20DI4653::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 2, var_STATUS, conn_STATUS);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_CNIDO, conn_CNIDO);
      break;
    }
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 3, var_DI01, conn_DI01);
      writeData(scmFBInterfaceSpec.mNumDIs + 4, var_DI02, conn_DI02);
      writeData(scmFBInterfaceSpec.mNumDIs + 5, var_DI03, conn_DI03);
      writeData(scmFBInterfaceSpec.mNumDIs + 6, var_DI04, conn_DI04);
      writeData(scmFBInterfaceSpec.mNumDIs + 2, var_STATUS, conn_STATUS);
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_X20DI4653::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_CNID;
    case 2: return &var_MODID;
  }
  return nullptr;
}

CIEC_ANY *FORTE_X20DI4653::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_CNIDO;
    case 2: return &var_STATUS;
    case 3: return &var_DI01;
    case 4: return &var_DI02;
    case 5: return &var_DI03;
    case 6: return &var_DI04;
  }
  return nullptr;
}

CEventConnection *FORTE_X20DI4653::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_X20DI4653::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_CNID;
    case 2: return &conn_MODID;
  }
  return nullptr;
}

CDataConnection *FORTE_X20DI4653::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_CNIDO;
    case 2: return &conn_STATUS;
    case 3: return &conn_DI01;
    case 4: return &conn_DI02;
    case 5: return &conn_DI03;
    case 6: return &conn_DI04;
  }
  return nullptr;
}
