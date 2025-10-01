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
#include "Config_EMB_RES.h"

#include "forte/stringid.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  namespace {
    const auto cDataInputNames = std::array{"OPCUA_Namespace"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(Config_EMB_RES, "iec61499::system::Config_EMB_RES"_STRID);

  Config_EMB_RES::Config_EMB_RES(StringId paInstanceNameId, CFBContainer &paDevice) :
      CResource(paDevice, cFBInterfaceSpec, paInstanceNameId),
      fb_START("START"_STRID, *this),
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
} // namespace forte::iec61499::system
