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

#include "X20DI9371_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CNID);
USE_STRING_ID(CNIDO);
USE_STRING_ID(DI01);
USE_STRING_ID(DI02);
USE_STRING_ID(DI03);
USE_STRING_ID(DI04);
USE_STRING_ID(DI05);
USE_STRING_ID(DI06);
USE_STRING_ID(DI07);
USE_STRING_ID(DI08);
USE_STRING_ID(DI09);
USE_STRING_ID(DI10);
USE_STRING_ID(DI11);
USE_STRING_ID(DI12);
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
USE_STRING_ID(X20DI9371);

#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_X20DI9371, STRID(X20DI9371))

const CStringDictionary::TStringId FORTE_X20DI9371::scmDataInputNames[] = {STRID(QI), STRID(CNID), STRID(MODID)};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataInputTypeIds[] = {STRID(BOOL), STRID(USINT), STRID(UINT)};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataOutputNames[] = {
    STRID(QO),   STRID(CNIDO), STRID(STATUS), STRID(DI01), STRID(DI02), STRID(DI03), STRID(DI04), STRID(DI05),
    STRID(DI06), STRID(DI07),  STRID(DI08),   STRID(DI09), STRID(DI10), STRID(DI11), STRID(DI12)};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataOutputTypeIds[] = {
    STRID(BOOL), STRID(USINT), STRID(STRING), STRID(BOOL), STRID(BOOL), STRID(BOOL), STRID(BOOL), STRID(BOOL),
    STRID(BOOL), STRID(BOOL),  STRID(BOOL),   STRID(BOOL), STRID(BOOL), STRID(BOOL), STRID(BOOL)};
const TDataIOID FORTE_X20DI9371::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI9371::scmEIWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_X20DI9371::scmEOWith[] = {0,  2, 1, scmWithListDelimiter, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                                                14, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI9371::scmEOWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_X20DI9371::scmFBInterfaceSpec = {2,
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
                                                              15,
                                                              scmDataOutputNames,
                                                              scmDataOutputTypeIds,
                                                              0,
                                                              nullptr,
                                                              0,
                                                              nullptr};

FORTE_X20DI9371::FORTE_X20DI9371(const CStringDictionary::TStringId paInstanceNameId,
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
    conn_DI04(*this, 6, var_DI04),
    conn_DI05(*this, 7, var_DI05),
    conn_DI06(*this, 8, var_DI06),
    conn_DI07(*this, 9, var_DI07),
    conn_DI08(*this, 10, var_DI08),
    conn_DI09(*this, 11, var_DI09),
    conn_DI10(*this, 12, var_DI10),
    conn_DI11(*this, 13, var_DI11),
    conn_DI12(*this, 14, var_DI12) {};

void FORTE_X20DI9371::setInitialValues() {
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
  var_DI05 = 0_BOOL;
  var_DI06 = 0_BOOL;
  var_DI07 = 0_BOOL;
  var_DI08 = 0_BOOL;
  var_DI09 = 0_BOOL;
  var_DI10 = 0_BOOL;
  var_DI11 = 0_BOOL;
  var_DI12 = 0_BOOL;
}

void FORTE_X20DI9371::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  executePowerlinkEvent(paEIID, paECET, scmEventINITID, scmEventREQID, scmEventINITOID, scmEventCNFID, var_QI, var_QO,
                        var_CNID, var_MODID);
}

void FORTE_X20DI9371::readInputData(const TEventID paEIID) {
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

void FORTE_X20DI9371::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(2, var_STATUS, conn_STATUS);
      writeData(1, var_CNIDO, conn_CNIDO);
      break;
    }
    case scmEventCNFID: {
      writeData(3, var_DI01, conn_DI01);
      writeData(4, var_DI02, conn_DI02);
      writeData(5, var_DI03, conn_DI03);
      writeData(6, var_DI04, conn_DI04);
      writeData(7, var_DI05, conn_DI05);
      writeData(8, var_DI06, conn_DI06);
      writeData(9, var_DI07, conn_DI07);
      writeData(10, var_DI08, conn_DI08);
      writeData(11, var_DI09, conn_DI09);
      writeData(12, var_DI10, conn_DI10);
      writeData(13, var_DI11, conn_DI11);
      writeData(14, var_DI12, conn_DI12);
      writeData(2, var_STATUS, conn_STATUS);
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_X20DI9371::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_CNID;
    case 2: return &var_MODID;
  }
  return nullptr;
}

CIEC_ANY *FORTE_X20DI9371::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_CNIDO;
    case 2: return &var_STATUS;
    case 3: return &var_DI01;
    case 4: return &var_DI02;
    case 5: return &var_DI03;
    case 6: return &var_DI04;
    case 7: return &var_DI05;
    case 8: return &var_DI06;
    case 9: return &var_DI07;
    case 10: return &var_DI08;
    case 11: return &var_DI09;
    case 12: return &var_DI10;
    case 13: return &var_DI11;
    case 14: return &var_DI12;
  }
  return nullptr;
}

CEventConnection *FORTE_X20DI9371::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_X20DI9371::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_CNID;
    case 2: return &conn_MODID;
  }
  return nullptr;
}

CDataConnection *FORTE_X20DI9371::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_CNIDO;
    case 2: return &conn_STATUS;
    case 3: return &conn_DI01;
    case 4: return &conn_DI02;
    case 5: return &conn_DI03;
    case 6: return &conn_DI04;
    case 7: return &conn_DI05;
    case 8: return &conn_DI06;
    case 9: return &conn_DI07;
    case 10: return &conn_DI08;
    case 11: return &conn_DI09;
    case 12: return &conn_DI10;
    case 13: return &conn_DI11;
    case 14: return &conn_DI12;
  }
  return nullptr;
}
