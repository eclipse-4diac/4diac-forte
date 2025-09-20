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

#include "forte/com/channel.h"

#include "forte/esfb.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::com {

  class BaseCommunicationFB : public CEventSourceFB, ComObserver<std::span<CIEC_ANY_VARIANT>> {
    public:
      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    protected:
      BaseCommunicationFB(CFBContainer &paContainer,
                          const SFBInterfaceSpec &paInterfaceSpec,
                          StringId paInstanceNameId) :
          CEventSourceFB(paContainer, paInterfaceSpec, paInstanceNameId) {
      }

      virtual ComResult open(std::string_view paID);
      virtual void close();
      virtual ComResult send(std::span<CIEC_ANY_VARIANT> paData);
      virtual ComResult poll();

      [[nodiscard]] ComChannel<std::span<CIEC_ANY_VARIANT>> *getChannel() const {
        return mChannel.get();
      }

    private:
      std::unique_ptr<ComChannel<std::span<CIEC_ANY_VARIANT>>> mChannel;
  };

} // namespace forte::com
