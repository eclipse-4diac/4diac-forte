/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_struct.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_string.h"

namespace forte::systemtest {
  class CIEC_MemberAccessTestStruct final : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(MemberAccessTestStruct)

    public:
      CIEC_MemberAccessTestStruct();

      CIEC_MemberAccessTestStruct(const CIEC_BOOL &paVAR1, const CIEC_INT &paVAR2, const CIEC_STRING &paVAR3);

      CIEC_BOOL var_VAR1;
      CIEC_INT var_VAR2;
      CIEC_STRING var_VAR3;

      size_t getStructSize() const override {
        return 3;
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
