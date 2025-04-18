/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Markus Meingast
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "../../core/stringdict.h"
#include "Config_EMB_RES.h"

USE_STRING_ID(Config_EMB_RES);
USE_STRING_ID(OPCUA_Namespace);
USE_STRING_ID(START);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(Config_EMB_RES, STRID(Config_EMB_RES));

const CStringDictionary::TStringId Config_EMB_RES::scmVarInputNameIds[] = {STRID(OPCUA_Namespace)};
const CStringDictionary::TStringId Config_EMB_RES::scmDIDataTypeIds[] = {STRID(WSTRING)};

const SFBInterfaceSpec Config_EMB_RES::scmFBInterfaceSpec = {0,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             0,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             nullptr,
                                                             1,
                                                             scmVarInputNameIds,
                                                             scmDIDataTypeIds,
                                                             0,
                                                             nullptr,
                                                             nullptr,
                                                             0,
                                                             nullptr,
                                                             0,
                                                             nullptr};

Config_EMB_RES::Config_EMB_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice) :
    CResource(paDevice, scmFBInterfaceSpec, paInstanceNameId),
    fb_START(STRID(START), *this),
    conn_opcua_namespace(nullptr),
    conn_opcua_namespace_int(*this, 0, u""_WSTRING) {
}

Config_EMB_RES::~Config_EMB_RES() = default;

bool Config_EMB_RES::initialize() {
  if (!CResource::initialize()) {
    return false;
  }
  return true;
}

CIEC_ANY *Config_EMB_RES::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_opcua_namespace_int.getValue();
  }
  return nullptr;
}

CDataConnection **Config_EMB_RES::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_opcua_namespace;
  }
  return nullptr;
}

CConnection *Config_EMB_RES::getResIf2InConnectionUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_opcua_namespace_int;
  }
  return nullptr;
}
