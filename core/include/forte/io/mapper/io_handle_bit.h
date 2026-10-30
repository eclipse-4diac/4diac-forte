/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
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

#include "forte/io/mapper/io_handle.h"
#include "forte/io/device/io_controller.h"
#include "forte/datatypes/forte_bool.h"

namespace forte::io {

  class IOHandleBit : public IOHandle {
    public:
      IOHandleBit(IODeviceController *paController,
                  IOMapper::Direction paDirection,
                  uint8_t paOffset,
                  uint8_t paPosition,
                  uint8_t *paImage);

      void set(const CIEC_ANY &) override;
      void get(CIEC_ANY &) override;

      bool equal(unsigned char *paOldImage) const;

    protected:
      void onObserver(IOObserver *paObserver) override;

      void dropObserver() override;

      virtual void reset() {
        set(CIEC_BOOL());
      }

      const uint8_t mOffset;
      const uint8_t mMask;

    private:
      uint8_t *mImage;
  };

} // namespace forte::io
