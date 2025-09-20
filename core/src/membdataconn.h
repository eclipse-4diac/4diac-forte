/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
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

#include "delegdataconn.h"

namespace forte::internal {

  class CMemberDataConnection : public CDelegatingDataConnection<CIEC_ANY> {

    public:
      CMemberDataConnection(CFunctionBlock &paSrcFB,
                            const TPortId paSrcPortId,
                            CIEC_ANY &paMember,
                            const std::span<const StringId> paMemberName) :
          CDelegatingDataConnection(paSrcFB, paSrcPortId, paMember),
          cmMemberName(paMemberName.begin(), paMemberName.end()) {
      }

      void readData(CIEC_ANY &paValue) const override {
        paValue.setValue(getValue());
      }

      void getSourcePortName(TNameIdentifier &paResult) const override;

    private:
      const TNameIdentifier cmMemberName;
  };
} // namespace forte::internal
