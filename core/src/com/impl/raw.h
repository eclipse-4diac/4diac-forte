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
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include <span>

#include "forte/com/channel.h"
#include "forte/com/buffer.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::com::impl {
  class RawChannel final : public ComChannel<std::span<CIEC_ANY_VARIANT>>, ComObserver<ComBuffer> {
    public:
      explicit RawChannel(ComObserver<std::span<CIEC_ANY_VARIANT>> &paObserver) : ComChannel(paObserver) {
      }

      ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) override;
      ComResult send(std::span<CIEC_ANY_VARIANT> paData) override;
      ComResult poll() override;
      ComResult close() override;

      ComResult receive(ComBuffer paData, ComChannel<ComBuffer> &paChannel) override;
      void sent(ComResult paResult, ComChannel<ComBuffer> &paChannel) override;

      ComResult setMinReceiveSize(std::size_t paSize) override;
      ComResult setMaxReceiveSize(std::size_t paSize) override;

      std::optional<ComBuffer> getSendBuffer() override {
        return ComBuffer(mSendBuffer);
      }

    private:
      std::unique_ptr<ComChannel<ComBuffer>> mChannel;
      std::string mSendBuffer;
  };
} // namespace forte::com::impl
