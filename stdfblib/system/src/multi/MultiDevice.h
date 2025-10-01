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

#include "forte/device.h"
#include <memory>
#include <future>
#include <variant>
#include "MultiMGR.h"

namespace forte::iec61499::system {
  /**
   * @brief Encapsulates a device so that the device can be remotely restarted.
   * This wrapper device itself does not offer any capability apart from the extra methods offered the MultMGE
   * and is actually killing itself on initialization in order to disable all handlers.
   */
  class MultiDevice : public CDevice {
    public:
      explicit MultiDevice(std::string_view paMGRID = "localhost:61499");
      ~MultiDevice() override = default;

      int startDevice() override;

      /**
       * @brief request a reset of the controlled device. This function returns immediately
       * and resetting occurs asynchronoysly
       */
      void requestResetControlledDevice();

    private:
      int resetControlledDevice();

      std::unique_ptr<CDevice> mControlledDevice;

      std::unique_ptr<MultiMGR> mMultiMgr;

      std::variant<OPCUA_MGR *, std::unique_ptr<OPCUA_MGR>> mOpcuaMgr;

      // the kill signal sent by main is handle by this promise
      // which is used just as a inter-thread communication
      // to avoid condition variables and such
      std::promise<void> mKillSignal;

      std::string mMGRID;

      // resetinng the controlled device must be done asynchronously
      // since otherwise the opc ua handler will block for request coming from it,
      // as when the device is reset, the opcua handler is disabled, and the current
      // callbacks need to return
      std::future<void> mRestartSignalHandler;

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

      void awaitShutdown() override;

      void killControlledDevice();

      // needed as these are abstract in the parent
      CIEC_ANY *getDI(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::system
