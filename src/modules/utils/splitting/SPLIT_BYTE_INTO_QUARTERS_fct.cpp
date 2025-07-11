/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SPLIT_BYTE_INTO_QUARTERS
 *** Description: this Function extracts the 4 QUARTER BYTE from a byte
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#include "SPLIT_BYTE_INTO_QUARTERS_fct.h"

USE_STRING_ID(BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(QUARTER_BYTE_00);
USE_STRING_ID(QUARTER_BYTE_01);
USE_STRING_ID(QUARTER_BYTE_02);
USE_STRING_ID(QUARTER_BYTE_03);
USE_STRING_ID(REQ);
USE_STRING_ID(SPLIT_BYTE_INTO_QUARTERS);

#include "core/datatypes/forte_any_int_variant.h"
#include "core/datatypes/forte_byte.h"
#include "core/datatypes/forte_any_bit_variant.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"
#include "quarterconst_gcf.h"

DEFINE_FIRMWARE_FB(FORTE_SPLIT_BYTE_INTO_QUARTERS, STRID(SPLIT_BYTE_INTO_QUARTERS))

namespace {
  const auto cDataInputNames = std::array{STRID(IN)};
  const auto cDataOutputNames =
      std::array{STRID(QUARTER_BYTE_00), STRID(QUARTER_BYTE_01), STRID(QUARTER_BYTE_02), STRID(QUARTER_BYTE_03)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
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

FORTE_SPLIT_BYTE_INTO_QUARTERS::FORTE_SPLIT_BYTE_INTO_QUARTERS(const CStringDictionary::TStringId paInstanceNameId,
                                                               forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_QUARTER_BYTE_00(*this, 0, var_QUARTER_BYTE_00),
    conn_QUARTER_BYTE_01(*this, 1, var_QUARTER_BYTE_01),
    conn_QUARTER_BYTE_02(*this, 2, var_QUARTER_BYTE_02),
    conn_QUARTER_BYTE_03(*this, 3, var_QUARTER_BYTE_03) {
}

void FORTE_SPLIT_BYTE_INTO_QUARTERS::setInitialValues() {
  var_IN = 0_BYTE;
  var_QUARTER_BYTE_00 = 0_BYTE;
  var_QUARTER_BYTE_01 = 0_BYTE;
  var_QUARTER_BYTE_02 = 0_BYTE;
  var_QUARTER_BYTE_03 = 0_BYTE;
}

void FORTE_SPLIT_BYTE_INTO_QUARTERS::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_SPLIT_BYTE_INTO_QUARTERS::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
      writeData(cFBInterfaceSpec.getNumDIs() + 2, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
      writeData(cFBInterfaceSpec.getNumDIs() + 3, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QUARTER_BYTE_00;
    case 1: return &var_QUARTER_BYTE_01;
    case 2: return &var_QUARTER_BYTE_02;
    case 3: return &var_QUARTER_BYTE_03;
  }
  return nullptr;
}

CEventConnection *FORTE_SPLIT_BYTE_INTO_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_SPLIT_BYTE_INTO_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QUARTER_BYTE_00;
    case 1: return &conn_QUARTER_BYTE_01;
    case 2: return &conn_QUARTER_BYTE_02;
    case 3: return &conn_QUARTER_BYTE_03;
  }
  return nullptr;
}

void FORTE_SPLIT_BYTE_INTO_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  func_SPLIT_BYTE_INTO_QUARTERS(var_IN, var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02,
                                var_QUARTER_BYTE_03);
  sendOutputEvent(scmEventCNFID, paECET);
}

void func_SPLIT_BYTE_INTO_QUARTERS(CIEC_BYTE st_lv_IN,
                                   CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                   CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                   CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                   CIEC_BYTE &st_lv_QUARTER_BYTE_03) {
  st_lv_QUARTER_BYTE_00 = 0_BYTE;
  st_lv_QUARTER_BYTE_01 = 0_BYTE;
  st_lv_QUARTER_BYTE_02 = 0_BYTE;
  st_lv_QUARTER_BYTE_03 = 0_BYTE;

#line 15 "SPLIT_BYTE_INTO_QUARTERS.fct"
  st_lv_QUARTER_BYTE_00 = func_SHR(func_AND<CIEC_BYTE>(st_lv_IN, FORTE_quarterconst::var_BYTE_QUARTER_00),
                                   FORTE_quarterconst::var_SHIFT_QUARTER_00);
#line 16 "SPLIT_BYTE_INTO_QUARTERS.fct"
  st_lv_QUARTER_BYTE_01 = func_SHR(func_AND<CIEC_BYTE>(st_lv_IN, FORTE_quarterconst::var_BYTE_QUARTER_01),
                                   FORTE_quarterconst::var_SHIFT_QUARTER_01);
#line 17 "SPLIT_BYTE_INTO_QUARTERS.fct"
  st_lv_QUARTER_BYTE_02 = func_SHR(func_AND<CIEC_BYTE>(st_lv_IN, FORTE_quarterconst::var_BYTE_QUARTER_02),
                                   FORTE_quarterconst::var_SHIFT_QUARTER_02);
#line 18 "SPLIT_BYTE_INTO_QUARTERS.fct"
  st_lv_QUARTER_BYTE_03 = func_SHR(func_AND<CIEC_BYTE>(st_lv_IN, FORTE_quarterconst::var_BYTE_QUARTER_03),
                                   FORTE_quarterconst::var_SHIFT_QUARTER_03);
}
