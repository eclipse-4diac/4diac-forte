/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SCALE
 *** Description: Scaling Function Block
 *** Version:
 ***     1.0: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "SCALE_fct.h"

using namespace forte::core::literals;

#include "core/datatypes/forte_real.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"
#include "SCALE_fct.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__SCALE, "signalprocessing__SCALE"_STRID)

namespace {
  const auto cDataInputNames = std::array{"IN"_STRID, "MAX_IN"_STRID, "MIN_IN"_STRID, "MAX_OUT"_STRID, "MIN_OUT"_STRID};
  const auto cDataOutputNames = std::array{""_STRID};
  const auto cEventInputNames = std::array{"REQ"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID};
  const auto cEventOutputNames = std::array{"CNF"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID};
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

FORTE_signalprocessing__SCALE::FORTE_signalprocessing__SCALE(const forte::core::StringId paInstanceNameId,
                                                             forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_MAX_IN(nullptr),
    conn_MIN_IN(nullptr),
    conn_MAX_OUT(nullptr),
    conn_MIN_OUT(nullptr),
    conn_(*this, 0, var_) {
}

void FORTE_signalprocessing__SCALE::setInitialValues() {
  var_IN = 0_REAL;
  var_MAX_IN = 0_REAL;
  var_MIN_IN = 0_REAL;
  var_MAX_OUT = 0_REAL;
  var_MIN_OUT = 0_REAL;
  var_ = 0_REAL;
}

void FORTE_signalprocessing__SCALE::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_MAX_IN, conn_MAX_IN);
      readData(2, var_MIN_IN, conn_MIN_IN);
      readData(3, var_MAX_OUT, conn_MAX_OUT);
      readData(4, var_MIN_OUT, conn_MIN_OUT);
      break;
    }
    default: break;
  }
}

void FORTE_signalprocessing__SCALE::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_signalprocessing__SCALE::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
    case 1: return &var_MAX_IN;
    case 2: return &var_MIN_IN;
    case 3: return &var_MAX_OUT;
    case 4: return &var_MIN_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__SCALE::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__SCALE::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__SCALE::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_MAX_IN;
    case 2: return &conn_MIN_IN;
    case 3: return &conn_MAX_OUT;
    case 4: return &conn_MIN_OUT;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__SCALE::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_signalprocessing__SCALE::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_SCALE(var_IN, var_MAX_IN, var_MIN_IN, var_MAX_OUT, var_MIN_OUT);
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_REAL func_SCALE(CIEC_REAL st_lv_IN,
                     CIEC_REAL st_lv_MAX_IN,
                     CIEC_REAL st_lv_MIN_IN,
                     CIEC_REAL st_lv_MAX_OUT,
                     CIEC_REAL st_lv_MIN_OUT) {
  CIEC_REAL st_ret_val = 0_REAL;

#line 13 "SCALE.fct"
  st_ret_val =
      func_ADD<CIEC_REAL>(func_DIV<CIEC_REAL>(func_MUL<CIEC_REAL>(func_SUB<CIEC_REAL>(st_lv_IN, st_lv_MIN_IN),
                                                                  func_SUB<CIEC_REAL>(st_lv_MAX_OUT, st_lv_MIN_OUT)),
                                              func_SUB<CIEC_REAL>(st_lv_MAX_IN, st_lv_MIN_IN)),
                          st_lv_MIN_OUT);

  return st_ret_val;
}
