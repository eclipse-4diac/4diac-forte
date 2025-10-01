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

#include <functional>
#include <string>
#include <vector>
#include <optional>
#include "../OPCUA_MGR.h"
#include "deviceReplayer.h"

namespace forte {
  class CDevice;
  class CResource;
  class CFakeEventExecutionThread;

  namespace iec61499::system {
    class ReplayDevice;

    /**
     * @brief Gets a OPCUA_MGR object and adds device and resource methods for replaying on top of that
     * Device-level Methods:
     * - Read Traces (path): void -> Reads traces from a local path and prepare a device to be replayed
     *
     * Resource-level methods:
     * - Replay Next Event: Event -> Execute one event in a resource. Returns the event that was triggered
     */
    class ReplayMGR {

      public:
        ReplayMGR(ReplayDevice &paDevice, OPCUA_MGR &paOpcuaMgr);
        ~ReplayMGR() = default;

        /**
         * @brief Add reaplaying methods to the OPCUA_MGR object
         *
         * @return true if no problem occurred while initializing, false otherwise.
         */
        bool initialize();

      private:
        // device on which the methods will be executed
        ReplayDevice &mDevice;

        // OpcUa Mgr on top of which the extra debugging methods will be added
        OPCUA_MGR &mOpcuaMgr;

        std::unique_ptr<CDeviceReplayer> mDeviceReplayer;

        // the definition of the methods (and therefore its arguments) is done in different functions
        // therefore we need to store the strings for the the arguments so they live until
        // the methods are created in the opc ua server
        std::vector<std::string> mArgumentsInformation;

        /**
         * @brief Store a string and get a reference to it
         *
         * @param paString String to look for from the stored ones
         * @return reference to the stored string
         */
        std::string &getArgumentString(std::string paString);

        void addReadTracesMethod();

        void addReplayNextEventMethod();

        static UA_StatusCode onReadTraces(UA_Server *,
                                          const UA_NodeId *,
                                          void *,
                                          const UA_NodeId *,
                                          void *,
                                          const UA_NodeId *,
                                          void *,
                                          size_t,
                                          const UA_Variant *input,
                                          size_t,
                                          UA_Variant *);

        static UA_StatusCode onReplayNextEvent(UA_Server *,
                                               const UA_NodeId *,
                                               void *,
                                               const UA_NodeId *,
                                               void *methodContext,
                                               const UA_NodeId *,
                                               void *objectContext,
                                               size_t,
                                               const UA_Variant *,
                                               size_t,
                                               UA_Variant *output);
    };
  } // namespace iec61499::system
} // namespace forte
