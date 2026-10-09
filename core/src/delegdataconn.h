/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/
#pragma once

#include "forte/dataconn.h"

namespace forte::internal {
  class CDelegatingDataConnection : public CDataConnection {
    public:
      CDelegatingDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CIEC_ANY &paValue) :
          CDataConnection(paSrcFB, paSrcPortId),
          mValue(paValue) {
      }

      void writeData(const CIEC_ANY &) final {
        // a delegating connection is only for reading from the value no writing permitted
        // writing is only done on the original connection
      }

      CIEC_ANY &getValue() override {
        return mValue;
      }

      const CIEC_ANY &getValue() const {
        return mValue;
      }

      CIEC_ANY::EDataTypeID getDataTypeID() const override {
        return mValue.getDataTypeID();
      }

      bool isDelegating() const final {
        return true;
      }

    private:
      CIEC_ANY &mValue;
  };

} // namespace forte::internal
