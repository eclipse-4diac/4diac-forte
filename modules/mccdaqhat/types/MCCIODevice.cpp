/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include "MCCIODevice.h"

#include "forte/util/devlog.h"
#include "../mccdaqhat_config.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  MCCIODevice::MCCIODevice(HatType paType,
                     CFBContainer &paContainer,
                     const SFBInterfaceSpec &paInterfaceSpec,
                     const forte::StringId paInstanceNameId) :
      IOConfigFBMultiSlave(nullptr, 0, static_cast<int>(paType), paContainer, paInterfaceSpec, paInstanceNameId),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 1) {
  }

  forte::IPlugPin *MCCIODevice::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *MCCIODevice::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  bool MCCIODevice::createSlaveHandler() {
    const unsigned int address = getConfiguredAddress();
    const std::string error = validateAddress(address);
    if (!error.empty()) {
      DEVLOG_ERROR("[MCCDaqHat] %s\n", error.c_str());
      return false;
    }
    // On a duplicate the controller has already logged which other device owns the address.
    return getMCCController().registerDeviceAddress(mIndex, static_cast<uint8_t>(address));
  }

  void MCCIODevice::addChannel(CIEC_STRING &paId,
                            forte::io::IOMapper::Direction paDirection,
                            MCCDaqHatController::HandleKind paKind,
                            uint8_t paChannel) {
    // HandleDescriptor keeps a reference to the id, so the string must outlive the descriptor.
    const std::string id(paId.getStorage());
    if (id.empty()) {
      return;
    }
    MCCDaqHatController::HandleDescriptor descriptor(id, paDirection, mIndex, paKind, paChannel);
    initHandle(descriptor);
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
