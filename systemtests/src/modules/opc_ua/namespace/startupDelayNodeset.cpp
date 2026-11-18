/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Franz Höpfinger - initial implementation
 *******************************************************************************/

#include "forte/com/opc_ua/opcua_nodesets.h"
#include "forte/util/mainparam_utils.h"
#include "forte/arch/forte_thread.h"

namespace forte::com_infra::opc_ua::test {
  namespace {
    class OpcuaStartupDelayOption final
        : public util::CommandLineParser::IntOptionImpl<unsigned int,
                                                        "osd",
                                                        "opc_ua-startup-delay",
                                                        "<ms>",
                                                        "Delay the startup of the OPC UA server (systemtests only)"> {
      public:
        bool setOption(const unsigned int paArgument) override {
          mArgument = paArgument;
          return true;
        }

        unsigned int mArgument = 0;
    };

    OpcuaStartupDelayOption gOpcuaStartupDelay;

    // emulates a slow system: the nodesets are initialized before the server is started
    UA_StatusCode delayStartup(UA_Server *) {
      if (gOpcuaStartupDelay.mArgument > 0) {
        arch::CThread::sleepThread(gOpcuaStartupDelay.mArgument);
      }
      return UA_STATUSCODE_GOOD;
    }

    [[maybe_unused]] const OPC_UA_Nodesets::EntryImpl<delayStartup> entry;
  } // namespace
} // namespace forte::com_infra::opc_ua::test
