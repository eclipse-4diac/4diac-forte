/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/typelib.h"

namespace forte::eclipse4diac::io::ethercat {
  class CIEC_ECModuleConfig : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(ECModuleConfig);

    public:
      CIEC_UDINT ModuleIdent;
      CIEC_UINT Slot;

      CIEC_ECModuleConfig() = default;

      size_t getStructSize() const override {
        return 2;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override;

      CIEC_ANY *getMember(size_t paMemberIndex) override {
        switch (paMemberIndex) {
          case 0: return &ModuleIdent;
          case 1: return &Slot;
        }
        return nullptr;
      }

      const CIEC_ANY *getMember(size_t paMemberIndex) const override {
        switch (paMemberIndex) {
          case 0: return &ModuleIdent;
          case 1: return &Slot;
        }
        return nullptr;
      }

    private:
      static const StringId scmElementNames[];
  };
} // namespace forte::eclipse4diac::io::ethercat

