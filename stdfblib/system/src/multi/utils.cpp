/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#include "utils.h"
#include "forte/devicefactory.h"
#include "forte/device.h"
#include "forte/timerhandlerfactory.h"
#include "forte/ecetfactory.h"

#ifdef FORTE_DEBUG_DEVICE
#include "../debug_device/DebugDevice.h"
#endif // FORTE_DEBUG_DEVICE

#ifdef FORTE_OPCUA_DEVICE
#include "../OPCUA_DEV.h"
#endif // FORTE_OPCUA_DEVICE

#ifdef FORTE_REPLAY_DEVICE
#include "../replay/ReplayDevice.h"
#endif // FORTE_REPLAY_DEVICE

using namespace forte::literals;

namespace forte::iec61499::system {

  OPCUA_MGR *getOpcuaMgr(CDevice &paDevice) {
    auto currentDevice = DeviceFactory::getDefaultImpl();
#ifdef FORTE_DEBUG_DEVICE
    if (currentDevice == "Debug"_STRID) {
      return &static_cast<DebugDevice *>(&paDevice)->mOpcuaMgr;
    }
#endif // FORTE_DEBUG_DEVICE
#ifdef FORTE_OPCUA_DEVICE
    if (currentDevice == "OPCUA_DEV"_STRID) {
      return &static_cast<OPCUA_DEV *>(&paDevice)->mOPCUAMgr;
    }
#endif // FORTE_OPCUA_DEVICE

#ifdef FORTE_REPLAY_DEVICE
    if (currentDevice == "Replay"_STRID) {
      return &static_cast<ReplayDevice *>(&paDevice)->mOpcuaMgr;
    }
#endif
    return nullptr;
  }

  void setFactoriesSettings(FactoriesSettings paFactoriesSettings) {
    EcetFactory::setDefaultImpl(paFactoriesSettings.mEcet);
    TimerHandlerFactory::setDefaultImpl(paFactoriesSettings.mTimer);
  }

} // namespace forte::iec61499::system
