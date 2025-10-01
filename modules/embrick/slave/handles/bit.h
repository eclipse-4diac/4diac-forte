/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#pragma once

#include "../slave.h"
#include "../handle.h"

namespace forte::eclipse4diac::io::embrick {

  class EmbrickBitSlaveHandle : public EmbrickSlaveHandle {
    public:
      EmbrickBitSlaveHandle(forte::io::IODeviceController *paController,
                            forte::io::IOMapper::Direction paDirection,
                            uint8_t paOffset,
                            uint8_t paPosition,
                            EmbrickSlaveHandler *paSlave);

      void set(const CIEC_ANY &) override;
      void get(CIEC_ANY &) override;

      bool equal(unsigned char *mOldBuffer) override;

    protected:
      void reset() override {
        set(false_BOOL);
      }

      const uint8_t mMask;
  };

} // namespace forte::eclipse4diac::io::embrick
