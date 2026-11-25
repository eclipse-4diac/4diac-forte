/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *    Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#include "LuaTestStruct_dtp.h"

using namespace forte::literals;

namespace forte::systemtest {
  DEFINE_FIRMWARE_DATATYPE(LuaTestInner, "luatest::LuaTestInner"_STRID);

  const StringId CIEC_LuaTestInner::scmElementNames[] = {"ID"_STRID, "LABEL"_STRID};

  CIEC_LuaTestInner::CIEC_LuaTestInner() : CIEC_STRUCT(), var_ID(0_DINT), var_LABEL(""_STRING) {
  }

  CIEC_LuaTestInner::CIEC_LuaTestInner(const CIEC_DINT &paID, const CIEC_STRING &paLABEL) :
      CIEC_STRUCT(),
      var_ID(paID),
      var_LABEL(paLABEL) {
  }

  StringId CIEC_LuaTestInner::getStructTypeNameID() const {
    return "luatest::LuaTestInner"_STRID;
  }

  void CIEC_LuaTestInner::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("luatest::LuaTestInner"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_LuaTestInner &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_LuaTestInner::getMember(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_ID;
      case 1: return &var_LABEL;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_LuaTestInner::getMember(const size_t paIndex) const {
    switch (paIndex) {
      case 0: return &var_ID;
      case 1: return &var_LABEL;
    }
    return nullptr;
  }

  DEFINE_FIRMWARE_DATATYPE(LuaTestStruct, "luatest::LuaTestStruct"_STRID);

  const StringId CIEC_LuaTestStruct::scmElementNames[] = {"ENABLED"_STRID, "VALUE"_STRID, "INNER"_STRID,
                                                          "SAMPLES"_STRID, "ITEMS"_STRID};

  CIEC_LuaTestStruct::CIEC_LuaTestStruct() : CIEC_STRUCT(), var_ENABLED(0_BOOL), var_VALUE(0.0_LREAL) {
  }

  StringId CIEC_LuaTestStruct::getStructTypeNameID() const {
    return "luatest::LuaTestStruct"_STRID;
  }

  void CIEC_LuaTestStruct::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("luatest::LuaTestStruct"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_LuaTestStruct &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_LuaTestStruct::getMember(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_ENABLED;
      case 1: return &var_VALUE;
      case 2: return &var_INNER;
      case 3: return &var_SAMPLES;
      case 4: return &var_ITEMS;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_LuaTestStruct::getMember(const size_t paIndex) const {
    switch (paIndex) {
      case 0: return &var_ENABLED;
      case 1: return &var_VALUE;
      case 2: return &var_INNER;
      case 3: return &var_SAMPLES;
      case 4: return &var_ITEMS;
    }
    return nullptr;
  }
} // namespace forte::systemtest
