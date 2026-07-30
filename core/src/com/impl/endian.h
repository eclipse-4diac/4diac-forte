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

#include <span>

#include "forte/com/channel.h"
#include "forte/com/buffer.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::com::impl {
  class EndianChannel final : public ComChannel<std::span<CIEC_ANY_VARIANT>>, ComObserver<std::span<CIEC_ANY_VARIANT>> {
    public:
      explicit EndianChannel(ComObserver &paObserver) : ComChannel(paObserver) {
      }

      ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) override;
      ComResult send(std::span<CIEC_ANY_VARIANT> paData) override;
      ComResult poll() override;
      ComResult close() override;

      ComResult receive(std::span<CIEC_ANY_VARIANT> paData, ComChannel &paChannel) override;
      void sent(ComResult paResult, ComChannel &paChannel) override;

      ComResult setMinReceiveSize(std::size_t paSize) override;
      ComResult setMaxReceiveSize(std::size_t paSize) override;

      std::optional<std::span<CIEC_ANY_VARIANT>> getSendBuffer() override {
        return getObserver().getSendBuffer();
      }

      std::optional<std::span<CIEC_ANY_VARIANT>> getReceiveBuffer() override {
        return getObserver().getReceiveBuffer();
      }

    private:
      std::span<CIEC_ANY_VARIANT> convert(std::span<CIEC_ANY_VARIANT> paData) const;

      std::unique_ptr<ComChannel> mChannel;
      std::endian mEndian = std::endian::native;
  };
} // namespace forte::com::impl
