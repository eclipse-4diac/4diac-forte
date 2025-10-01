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

#include <open62541.h>
#include <string>
#include <vector>

namespace forte::iec61499::system {
  class MultiDevice;
  class OPCUA_MGR;

  /**
   * @brief Gets an OPCUA_MGR object and adds device on top of that
   * - Restart device: deleted the current device and creates a new one according the default one
   * - Set default device to create: set the default device to create
   */
  class MultiMGR {
    public:
      MultiMGR(MultiDevice &paDevice, OPCUA_MGR &paOpcuaMgr);
      ~MultiMGR() = default;

      bool initialize();

    private:
      MultiDevice &mDevice;

      OPCUA_MGR &mOpcuaMgr;

      // the arguments strings are passed by pointer and not stored, so they should leave until
      // the methods are created
      std::vector<std::string> mArgumentsInformation;

      std::string &getArgumentString(std::string paString);

      void addRestartDeviceMethod();

      void addSetDefaultDeviceMethod();

      static UA_StatusCode onRestartDevice(UA_Server *,
                                           const UA_NodeId *,
                                           void *,
                                           const UA_NodeId *,
                                           void *methodContext,
                                           const UA_NodeId *,
                                           void *,
                                           size_t,
                                           const UA_Variant *,
                                           size_t,
                                           UA_Variant *);

      static UA_StatusCode onSetDefaultDevice(UA_Server *,
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
  };
} // namespace forte::iec61499::system
