/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ernst Blecha - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "GetInstancePathAndName_fbt.h"

USE_STRING_ID(CHAR);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(GetInstancePathAndName);
USE_STRING_ID(Name);
USE_STRING_ID(Path);
USE_STRING_ID(REQ);
USE_STRING_ID(Sep);
USE_STRING_ID(STRING);

#include "core/datatypes/forte_char.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_GetInstancePathAndName, STRID(GetInstancePathAndName))

namespace {
  const auto cDataInputNames = std::array{STRID(Sep)};
  const auto cDataOutputNames = std::array{STRID(Path), STRID(Name)};
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

FORTE_GetInstancePathAndName::FORTE_GetInstancePathAndName(const CStringDictionary::TStringId paInstanceNameId,
                                                           forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
    var_Sep(0x2f_CHAR),
    conn_CNF(*this, 0),
    conn_Sep(nullptr),
    conn_Path(*this, 0, var_Path),
    conn_Name(*this, 1, var_Name) {
}

void FORTE_GetInstancePathAndName::setInitialValues() {
  var_Sep = 0x2f_CHAR;
  var_Path = ""_STRING;
  var_Name = ""_STRING;
}

void FORTE_GetInstancePathAndName::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_GetInstancePathAndName::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_Sep, conn_Sep);
      break;
    }
    default: break;
  }
}

void FORTE_GetInstancePathAndName::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Path, conn_Path);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_Name, conn_Name);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_GetInstancePathAndName::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Sep;
  }
  return nullptr;
}

CIEC_ANY *FORTE_GetInstancePathAndName::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Path;
    case 1: return &var_Name;
  }
  return nullptr;
}

CEventConnection *FORTE_GetInstancePathAndName::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_GetInstancePathAndName::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Sep;
  }
  return nullptr;
}

CDataConnection *FORTE_GetInstancePathAndName::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Path;
    case 1: return &conn_Name;
  }
  return nullptr;
}

CIEC_ANY *FORTE_GetInstancePathAndName::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_GetInstancePathAndName::alg_REQ(void) {
  var_Path = CIEC_STRING(getParent().getFullQualifiedApplicationInstanceName(var_Sep.operator TForteChar()));
  var_Name = CIEC_STRING(getInstanceName(), strlen(getInstanceName()));
}
