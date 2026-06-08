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
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_struct.h"

namespace forte {
  class CIEC_ANY_STRUCT : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(ANY_STRUCT)

    public:
      CIEC_ANY_STRUCT();

      size_t getStructSize() const override {
        return 0;
      }

      const StringId *elementNames() const override {
        return nullptr;
      }

      StringId getStructTypeNameID() const override;

      CIEC_ANY *getMember(size_t paMemberIndex) override;

      const CIEC_ANY *getMember(size_t paMemberIndex) const override;
  };
} // namespace forte
