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

#include "forte/com/basefb.h"
#include "forte/com/factory.h"

#include "forte/datatypes/forte_any_variant.h"

namespace forte::com {
  ComResult BaseCommunicationFB::open(const std::string_view paID) {
    std::vector<ComChannelDescriptor> descriptors = parseComId(paID);
    if (descriptors.empty()) {
      return ComResult::InvalidId;
    }

    mChannel = ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::create(descriptors.front().mChannel, *this);
    if (!mChannel) {
      return ComResult::InvalidId;
    }

    if (const ComResult result = mChannel->open(descriptors.front().mConfigString, std::span{descriptors}.subspan(1));
        result != ComResult::Ok) {
      mChannel.reset();
      return result;
    }

    return ComResult::Ok;
  }

  void BaseCommunicationFB::close() {
    if (mChannel) {
      mChannel->close();
      mChannel.reset();
    }
  }

  ComResult BaseCommunicationFB::send(const std::span<CIEC_ANY_VARIANT> paData) {
    return mChannel ? mChannel->send(paData) : ComResult::NoSocket;
  }

  ComResult BaseCommunicationFB::poll() {
    return mChannel ? mChannel->poll() : ComResult::NoSocket;
  }

  EMGMResponse BaseCommunicationFB::changeExecutionState(const EMGMCommandType paCommand) {
    const EMGMResponse retVal = CEventSourceFB::changeExecutionState(paCommand);
    if (retVal == EMGMResponse::Ready && paCommand == EMGMCommandType::Kill && mChannel) {
      close();
    }
    return retVal;
  }
} // namespace forte::com
