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

#include "forte/stringid.h"
#include "forte/ecetfactory.h"
#include "forte/timerhandlerfactory.h"

using namespace forte::literals;

namespace forte {
  class CDevice;

  namespace iec61499::system {
    class OPCUA_MGR;

    /**
     * @brief Get the OpcuaMgr of the device if it has any
     *
     * @param paDevice the device were to look for the opcua mgr. The lookup is based on the current default device
     * value set in the device factory
     * @return a pointer to the opcua mgr of the device, nullptr if the device does not have a opcua mgr
     */
    OPCUA_MGR *getOpcuaMgr(CDevice &paDevice);

    struct FactoriesSettings {
        StringId mEcet{"default"_STRID};
        StringId mTimer{"default"_STRID};
    };

    /**
     * @brief Set the desired factories settings
     *
     * @param paFactoriesSettings settings to be set
     */
    void setFactoriesSettings(FactoriesSettings paFactoriesSettings);

  } // namespace iec61499::system
} // namespace forte
