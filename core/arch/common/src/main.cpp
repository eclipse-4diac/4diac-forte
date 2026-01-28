/*******************************************************************************
 * Copyright (c) 2006, 2024 ACIN, Profactor GmbH, AIT, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Rene Smodic,
 *  Martin Melik Merkumians, Ingo Hegny, Filip Andren - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/

#include <stdio.h>
#include <signal.h>
#include "forte/forteinstance.h"
#include "forte/arch/forte_architecture.h"

#include "forte/util/mainparam_utils.h"

namespace {
  forte::C4diacFORTEInstance g4diacForteInstance;

  void endForte(int) {
    g4diacForteInstance.triggerDeviceShutdown();
  }

  void callOnExit() {
    forte::arch::CForteArchitecture::deinitialize();
  }

  void hookSignals() {
    signal(SIGINT, endForte);
    signal(SIGTERM, endForte);
#ifndef WIN32
    signal(SIGHUP, endForte);
#endif
  }
} // namespace

int main(int argc, char *arg[]) {

  if (auto result = forte::arch::CForteArchitecture::initialize(argc, arg); result != 0) {
    return result;
  }

  std::atexit(callOnExit);

  hookSignals();

  const std::string ipPort = forte::util::CommandLineParser::parseCommandLineArguments(argc, arg);
  if (ipPort.empty() || ipPort.find(':') == std::string::npos) {
    //! Lists the help for FORTE
    forte::util::CommandLineParser::listHelp();
    return -1;
  }

  if (!g4diacForteInstance.startupNewDevice(ipPort)) {
    DEVLOG_ERROR("Could not start a new device\n");
    if constexpr (forte::cgBuildSharedLibs) {
      DEVLOG_ERROR("Did you load the required shared libraries using '-l'?\n");
    } else {
      DEVLOG_ERROR("Did you link the static libraries into the final executable using '--whole-archive'?\n");
    }
    return -1;
  }

  DEVLOG_INFO("FORTE is up and running\n");
  g4diacForteInstance.awaitDeviceShutdown();
  DEVLOG_INFO("FORTE finished\n");

  return 0;
}
