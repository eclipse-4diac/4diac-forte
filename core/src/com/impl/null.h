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

#include "forte/com/buffer.h"
#include "forte/com/channel.h"

namespace forte::com::impl {

  class NullChannel final : public ComChannel<ComBuffer> {
    public:
      explicit NullChannel(ComObserver<ComBuffer> &paObserver) : ComChannel(paObserver) {
      }

      ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) override;
      ComResult send(ComBuffer paData) override;
      ComResult poll() override;
      ComResult close() override;

      ComResult setMinReceiveSize(std::size_t paSize) override;
  };

} // namespace forte::com::impl
