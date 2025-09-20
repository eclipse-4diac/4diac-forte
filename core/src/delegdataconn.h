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

  template<typename T>
  class CDelegatingDataConnection : public CDataConnection {
    public:
      CDelegatingDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, T &paValue) :
          CDataConnection(paSrcFB, paSrcPortId),
          mValue(paValue) {
      }

      void writeData(const CIEC_ANY &) final {
        // a delegating connection is only for reading from the value no writing permitted
        // writing is only done on the original connection
      }

      T &getValue() override {
        return mValue;
      }

      const T &getValue() const {
        return mValue;
      }

      bool isDelegating() const final {
        return true;
      }

    private:
      T &mValue;
  };

} // namespace forte::internal
