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

#include "F_LREAL_AS_WSTRING_fbt.h"

#include "core/datatypes/forte_lreal.h"
#include "core/datatypes/forte_wstring.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_LREAL_AS_WSTRING);
USE_STRING_ID(IN);
USE_STRING_ID(LREAL);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_F_LREAL_AS_WSTRING, STRID(F_LREAL_AS_WSTRING))

const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmDataInputTypeIds[] = {STRID(LREAL)};
const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmDataOutputTypeIds[] = {STRID(WSTRING)};
const TDataIOID FORTE_F_LREAL_AS_WSTRING::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LREAL_AS_WSTRING::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_LREAL_AS_WSTRING::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LREAL_AS_WSTRING::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LREAL_AS_WSTRING::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_LREAL_AS_WSTRING::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_LREAL_AS_WSTRING::FORTE_F_LREAL_AS_WSTRING(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_LREAL),
    var_OUT(u""_WSTRING),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_LREAL_AS_WSTRING::setInitialValues() {
  var_IN = 0_LREAL;
  var_OUT = u""_WSTRING;
}

void FORTE_F_LREAL_AS_WSTRING::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_LREAL_AS_WSTRING::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_LREAL_AS_WSTRING::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_LREAL_AS_WSTRING::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_LREAL_AS_WSTRING::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LREAL_AS_WSTRING::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LREAL_AS_WSTRING::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LREAL_AS_WSTRING::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LREAL_AS_WSTRING::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LREAL_AS_WSTRING::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_LREAL_AS_WSTRING::alg_REQ(void) {

  #line 2 "F_LREAL_AS_WSTRING.fbt"
  var_OUT = func_LREAL_AS_WSTRING(var_IN);
}
