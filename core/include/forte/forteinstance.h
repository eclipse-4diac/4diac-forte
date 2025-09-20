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
 *    Alois Zoitl, Martin Jobst - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/device.h"

namespace forte {
  class C4diacFORTEInstance {
    public:
      C4diacFORTEInstance() = default;

      ~C4diacFORTEInstance();

      bool startupNewDevice(const std::string &paMGRID);

      void triggerDeviceShutdown();

      void awaitDeviceShutdown();

    private:
      std::unique_ptr<CDevice> mActiveDevice;
  };
} // namespace forte
