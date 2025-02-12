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

#include "device.h"
#include "MultiMGR.h"

#include "core/ecetFake.h"


#include <vector>
#include <memory>
#include <future>
#include <variant>

/**
 * @brief Encapsulates a device so that the device can be remotely restarted. 
 * The timer for example can therefore be changed and the managed device restarted.
 * This wrapper device itself does not offer any capability and is actually killing itself 
 * on initialization in order to disable all handlers.
 * 
 */
class MultiDevice : public CDevice {
public:

  MultiDevice(const std::string &paMGRID = "localhost:61499");
  ~MultiDevice() override = default;

  int startDevice() override;

  void setRestartSignal(std::future<void> paSignal);

  int resetControlledDevice();

  static inline const std::string scmMultiDeviceName{"MultiDevice"};

private:

  std::unique_ptr<CDevice> mControlledDevice;

  std::unique_ptr<MultiMGR> mMultiMgr;

  std::variant<OPCUA_MGR*, std::unique_ptr<OPCUA_MGR>> mOpcuaMgr;

  // the kill signal sent by main is handle by this promise
  // which is used just as a inter-thread communication
  // to avoid condition variables and such
  std::promise<void> mKillSignal;

  std::string mMGRID;

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  static const inline std::string scmDefaultDeviceToCreate{"RMT_DEV"};

  // For restart signal coming from the OPCUA server
  // killing the device will wait for the opcua handler to finish before returning
  // so it the method callback will block.
  // The killing of the device (and the opcua server in the handler) is made asynchonously.
  // the last future referencing the shared state with the promise of the async will block untill
  // the async finishes, so we move the future into this class outside of the opcua callback,
  // so the callback does not block. 
  std::future<void> mRestartSignalHandler;

  EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;
  
  void awaitShutdown() override;

  void killControlledDevice();

  // needed as these are abstract in the parent
  CIEC_ANY *getDI(size_t) override;
  CDataConnection **getDIConUnchecked(TPortId) override;

};
