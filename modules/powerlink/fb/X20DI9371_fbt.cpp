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

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_X20DI9371, "X20DI9371"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID, "CNID"_STRID, "MODID"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID,   "CNIDO"_STRID, "STATUS"_STRID, "DI01"_STRID, "DI02"_STRID,
                                           "DI03"_STRID, "DI04"_STRID,  "DI05"_STRID,   "DI06"_STRID, "DI07"_STRID,
                                           "DI08"_STRID, "DI09"_STRID,  "DI10"_STRID,   "DI11"_STRID, "DI12"_STRID};
  const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
  const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
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
} // namespace

FORTE_X20DI9371::FORTE_X20DI9371(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    PowerlinkFunctionBlockDI(paContainer, cFBInterfaceSpec, paInstanceNameId),
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
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_STATUS, conn_STATUS);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_CNIDO, conn_CNIDO);
      break;
    }
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 3, var_DI01, conn_DI01);
      writeData(cFBInterfaceSpec.getNumDIs() + 4, var_DI02, conn_DI02);
      writeData(cFBInterfaceSpec.getNumDIs() + 5, var_DI03, conn_DI03);
      writeData(cFBInterfaceSpec.getNumDIs() + 6, var_DI04, conn_DI04);
      writeData(cFBInterfaceSpec.getNumDIs() + 7, var_DI05, conn_DI05);
      writeData(cFBInterfaceSpec.getNumDIs() + 8, var_DI06, conn_DI06);
      writeData(cFBInterfaceSpec.getNumDIs() + 9, var_DI07, conn_DI07);
      writeData(cFBInterfaceSpec.getNumDIs() + 10, var_DI08, conn_DI08);
      writeData(cFBInterfaceSpec.getNumDIs() + 11, var_DI09, conn_DI09);
      writeData(cFBInterfaceSpec.getNumDIs() + 12, var_DI10, conn_DI10);
      writeData(cFBInterfaceSpec.getNumDIs() + 13, var_DI11, conn_DI11);
      writeData(cFBInterfaceSpec.getNumDIs() + 14, var_DI12, conn_DI12);
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_STATUS, conn_STATUS);
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
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
