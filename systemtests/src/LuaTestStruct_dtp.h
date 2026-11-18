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

#pragma once

#include "forte/datatypes/forte_struct.h"

#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_string.h"

namespace forte::systemtest {
  //! The struct nested inside LuaTestStruct
  class CIEC_LuaTestInner final : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(LuaTestInner)

    public:
      CIEC_LuaTestInner();

      CIEC_LuaTestInner(const CIEC_DINT &paID, const CIEC_STRING &paLABEL);

      CIEC_DINT var_ID;
      CIEC_STRING var_LABEL;

      size_t getStructSize() const override {
        return 2;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override;

      void setValue(const CIEC_ANY &paValue) override;

      CIEC_ANY *getMember(size_t) override;
      const CIEC_ANY *getMember(size_t) const override;

    private:
      static const StringId scmElementNames[];
  };

  //! A struct with a nested struct, an elementary array and a struct array, used by the Lua type system tests
  class CIEC_LuaTestStruct final : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(LuaTestStruct)

    public:
      CIEC_LuaTestStruct();

      CIEC_BOOL var_ENABLED;
      CIEC_LREAL var_VALUE;
      CIEC_LuaTestInner var_INNER;
      CIEC_ARRAY_FIXED<CIEC_INT, 0, 3> var_SAMPLES;
      CIEC_ARRAY_FIXED<CIEC_LuaTestInner, 1, 2> var_ITEMS;

      size_t getStructSize() const override {
        return 5;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override;

      void setValue(const CIEC_ANY &paValue) override;

      CIEC_ANY *getMember(size_t) override;
      const CIEC_ANY *getMember(size_t) const override;

    private:
      static const StringId scmElementNames[];
  };
} // namespace forte::systemtest
