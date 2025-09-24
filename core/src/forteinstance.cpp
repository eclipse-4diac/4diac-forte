/*******************************************************************************
 * Copyright (c) 2024, 2025 Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Martin Jobst - initial implementation and rework communication
 *                                infrastructure
 *******************************************************************************/

#include "forte/forteinstance.h"
#include "forte/devicefactory.h"

namespace forte {
  C4diacFORTEInstance::~C4diacFORTEInstance() {
    if (mActiveDevice) {
      mActiveDevice->deinitialize();
    }
  }

  bool C4diacFORTEInstance::startupNewDevice(const std::string &paMGRID) {
    if (mActiveDevice) {
      // we have a current active device stop it
      triggerDeviceShutdown();
      awaitDeviceShutdown();
      mActiveDevice->deinitialize();
    }
    mActiveDevice = DeviceFactory::create(paMGRID);
    if (mActiveDevice) {
      mActiveDevice->initialize();
      mActiveDevice->startDevice();
    }
    return mActiveDevice.operator bool();
  }

  void C4diacFORTEInstance::triggerDeviceShutdown() {
    if (mActiveDevice) {
      mActiveDevice->changeExecutionState(EMGMCommandType::Kill);
    }
  }

  void C4diacFORTEInstance::awaitDeviceShutdown() {
    if (mActiveDevice) {
      mActiveDevice->awaitShutdown();
    }
  }
} // namespace forte
