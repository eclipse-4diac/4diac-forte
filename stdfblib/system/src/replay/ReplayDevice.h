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

#pragma once

#include "ReplayMGR.h"
#include "../RMT_DEV.h"

namespace forte::iec61499::system {
  /**
   * @brief Device that adds replay commands to the device. The commands are defined in ReplayMGR
   *
   */
  class ReplayDevice : public RMT_DEV {
    public:
      explicit ReplayDevice(std::string_view paMGRID = "localhost:61499");
      ~ReplayDevice() override = default;

      int startDevice() override;

      EMGMResponse executeMGMCommand(SManagementCMD &paCommand) override;

      void startControlling();

      OPCUA_MGR mOpcuaMgr;

      ReplayMGR mReplayMgr;

    private:
      bool mAlreadyControlled{false};

      // the timer factory has to be set to the fake one, which needs to be done before the
      // parent class is constructed, since the timer is created in the device execution,
      // a member of the CDevice, so in the constructor of this class it's already too late.
      // this method then is executed before calling the parent, giving the chance to set
      // the timer to the fake one.
      const std::string_view setInitialState(std::string_view paMGRID);
  };
} // namespace forte::iec61499::system
