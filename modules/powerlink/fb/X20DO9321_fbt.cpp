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

#include "X20DO9321_fbt.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_X20DO9321, "X20DO9321"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID,   "CNID"_STRID, "MODID"_STRID, "DO01"_STRID, "DO02"_STRID,
                                          "DO03"_STRID, "DO04"_STRID, "DO05"_STRID,  "DO06"_STRID, "DO07"_STRID,
                                          "DO08"_STRID, "DO09"_STRID, "DO10"_STRID,  "DO11"_STRID, "DO12"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "CNIDO"_STRID, "STATUS"_STRID};
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

FORTE_X20DO9321::FORTE_X20DO9321(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    PowerlinkFunctionBlockDO(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_INITO(*this, 0),
    conn_CNF(*this, 1),
    conn_QI(nullptr),
    conn_CNID(nullptr),
    conn_MODID(nullptr),
    conn_DO01(nullptr),
    conn_DO02(nullptr),
    conn_DO03(nullptr),
    conn_DO04(nullptr),
    conn_DO05(nullptr),
    conn_DO06(nullptr),
    conn_DO07(nullptr),
    conn_DO08(nullptr),
    conn_DO09(nullptr),
    conn_DO10(nullptr),
    conn_DO11(nullptr),
    conn_DO12(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_CNIDO(*this, 1, var_CNIDO),
    conn_STATUS(*this, 2, var_STATUS) {};

void FORTE_X20DO9321::setInitialValues() {
  var_QI = 0_BOOL;
  var_CNID = 0_USINT;
  var_MODID = 0_UINT;
  var_DO01 = 0_BOOL;
  var_DO02 = 0_BOOL;
  var_DO03 = 0_BOOL;
  var_DO04 = 0_BOOL;
  var_DO05 = 0_BOOL;
  var_DO06 = 0_BOOL;
  var_DO07 = 0_BOOL;
  var_DO08 = 0_BOOL;
  var_DO09 = 0_BOOL;
  var_DO10 = 0_BOOL;
  var_DO11 = 0_BOOL;
  var_DO12 = 0_BOOL;
  var_QO = 0_BOOL;
  var_CNIDO = 0_USINT;
  var_STATUS = ""_STRING;
}

void FORTE_X20DO9321::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  executePowerlinkEvent(paEIID, paECET, scmEventINITID, scmEventREQID, scmEventINITOID, scmEventCNFID, var_QI, var_QO,
                        var_CNID, var_MODID);
}

void FORTE_X20DO9321::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_CNID, conn_CNID);
      readData(2, var_MODID, conn_MODID);
      break;
    }
    case scmEventREQID: {
      readData(3, var_DO01, conn_DO01);
      readData(4, var_DO02, conn_DO02);
      readData(5, var_DO03, conn_DO03);
      readData(6, var_DO04, conn_DO04);
      readData(7, var_DO05, conn_DO05);
      readData(8, var_DO06, conn_DO06);
      readData(9, var_DO07, conn_DO07);
      readData(10, var_DO08, conn_DO08);
      readData(11, var_DO09, conn_DO09);
      readData(12, var_DO10, conn_DO10);
      readData(13, var_DO11, conn_DO11);
      readData(14, var_DO12, conn_DO12);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_X20DO9321::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_STATUS, conn_STATUS);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_CNIDO, conn_CNIDO);
      break;
    }
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_STATUS, conn_STATUS);
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_X20DO9321::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_CNID;
    case 2: return &var_MODID;
    case 3: return &var_DO01;
    case 4: return &var_DO02;
    case 5: return &var_DO03;
    case 6: return &var_DO04;
    case 7: return &var_DO05;
    case 8: return &var_DO06;
    case 9: return &var_DO07;
    case 10: return &var_DO08;
    case 11: return &var_DO09;
    case 12: return &var_DO10;
    case 13: return &var_DO11;
    case 14: return &var_DO12;
  }
  return nullptr;
}

CIEC_ANY *FORTE_X20DO9321::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_CNIDO;
    case 2: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_X20DO9321::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_X20DO9321::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_CNID;
    case 2: return &conn_MODID;
    case 3: return &conn_DO01;
    case 4: return &conn_DO02;
    case 5: return &conn_DO03;
    case 6: return &conn_DO04;
    case 7: return &conn_DO05;
    case 8: return &conn_DO06;
    case 9: return &conn_DO07;
    case 10: return &conn_DO08;
    case 11: return &conn_DO09;
    case 12: return &conn_DO10;
    case 13: return &conn_DO11;
    case 14: return &conn_DO12;
  }
  return nullptr;
}

CDataConnection *FORTE_X20DO9321::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_CNIDO;
    case 2: return &conn_STATUS;
  }
  return nullptr;
}
