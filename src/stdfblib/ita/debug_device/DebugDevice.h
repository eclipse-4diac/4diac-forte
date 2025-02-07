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
#include "DebugMGR.h"

// std includes
#include <future>

/**
 * @brief Device that adds debug commands to the device. The commands are defined in DebugMGR
 * 
 */
class DebugDevice : public CDevice {
public:

  DebugDevice(const std::string &paMGRID = "localhost:61499");
  ~DebugDevice() override = default;

  int startDevice() override;

  OPCUA_MGR mOpcuaMgr;

  DebugMGR mDebugMgr;

  private:

    // the kill signal sent by main is handled by this promise
    // which is used just as a inter-thread communication
    // to avoid condition variables and such
    std::promise<void> mKillSignal;

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    void awaitShutdown() override;

    // needed as these are abstract in the parent
    CIEC_ANY *getDI(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
};
