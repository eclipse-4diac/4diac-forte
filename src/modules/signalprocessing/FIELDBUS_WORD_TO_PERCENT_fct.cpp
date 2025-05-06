/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_WORD_TO_PERCENT
 *** Description: Convert a REAL in the Range 0.0 to 100.0 to a WORD Value Range 0-FAFF
 *** Version:
 ***     1.0: 2023-10-21/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Update to a more Function like Interface
 *************************************************************************/

#include "FIELDBUS_WORD_TO_PERCENT_fct.h"

USE_STRING_ID();
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(REAL);
USE_STRING_ID(REQ);
USE_STRING_ID(signalprocessing__FIELDBUS_WORD_TO_PERCENT);
USE_STRING_ID(WI);
USE_STRING_ID(WO);
USE_STRING_ID(WORD);

#include "forte_real.h"
#include "forte_udint.h"
#include "forte_word.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "FIELDBUS_SIGNAL_gcf.h"
#include "FIELDBUS_WORD_TO_PERCENT_fct.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT, STRID(signalprocessing__FIELDBUS_WORD_TO_PERCENT))

const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataInputNames[] = {STRID(WI)};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataInputTypeIds[] = {
    STRID(WORD)};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataOutputNames[] = {STRID(),
                                                                                                             STRID(WO)};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataOutputTypeIds[] = {
    STRID(REAL), STRID(WORD)};
const TDataIOID FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventInputNames[] = {
    STRID(REQ)};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventInputTypeIds[] = {
    STRID(Event)};
const TDataIOID FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventOutputNames[] = {
    STRID(CNF)};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventOutputTypeIds[] = {
    STRID(Event)};
const SFBInterfaceSpec FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmFBInterfaceSpec = {1,
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
                                                                                               2,
                                                                                               scmDataOutputNames,
                                                                                               scmDataOutputTypeIds,
                                                                                               0,
                                                                                               nullptr,
                                                                                               0,
                                                                                               nullptr};

FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT(
    const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_WI(nullptr),
    conn_(*this, 0, var_),
    conn_WO(*this, 1, var_WO) {
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::setInitialValues() {
  var_WI = 0_WORD;
  var_ = 0_REAL;
  var_WO = 0_WORD;
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_WI, conn_WI);
      break;
    }
    default: break;
  }
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_, conn_);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_WO, conn_WO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_WI;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_;
    case 1: return &var_WO;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_WI;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_;
    case 1: return &conn_WO;
  }
  return nullptr;
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::executeEvent(const TEventID,
                                                                    CEventChainExecutionThread *const paECET) {
  var_ = func_FIELDBUS_WORD_TO_PERCENT(var_WI, var_WO);
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_REAL func_FIELDBUS_WORD_TO_PERCENT(CIEC_WORD st_lv_WI, CIEC_WORD &st_lv_WO) {
  CIEC_REAL st_ret_val = 0_REAL;
  st_lv_WO = 0_WORD;

#line 13 "FIELDBUS_WORD_TO_PERCENT.fct"
  if (func_LE(st_lv_WI, FORTE_signalprocessing__FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)) {
#line 14 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_lv_WO = st_lv_WI;
#line 15 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_ret_val = func_DIV<CIEC_REAL>(func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_lv_WO)),
                                     func_UDINT_TO_REAL(func_WORD_TO_UDINT(FORTE_signalprocessing__FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)));
  }

  return st_ret_val;
}
