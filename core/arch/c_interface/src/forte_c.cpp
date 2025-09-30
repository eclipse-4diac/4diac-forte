/*******************************************************************************
 * Copyright (c) 2018 - 2024 fortiss GmbH, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/

#include "forte/forte_c.h"

#include "forte/arch/forte_architecture.h"
#include "forte/arch/forte_printer.h"
#include "forte/forteinstance.h"
#include "forte/util/mainparam_utils.h"

namespace {
  const unsigned int defaultPort = 61499;
  const unsigned int maxPortValue = 65535;
} // namespace

int forteGlobalInitialize(int argc, char *argv[]) {
  return forte::arch::CForteArchitecture::initialize(argc, argv);
}

int forteGlobalDeinitialize() {
  return forte::arch::CForteArchitecture::deinitialize();
}

FORTE_STATUS forteStartInstance(unsigned int paPort, TForteInstance *paResultInstance) {

  if (maxPortValue < paPort) {
    DEVLOG_ERROR("Provided port %d is not valid\n", paPort);
    return FORTE_WRONG_PARAMETERS;
  }

  if (0 == paPort) {
    paPort = defaultPort;
  }

  char progName[] = "forte";
  char flag[] = "-c";
  char address[17] = "localhost:";
  char port[6];
  forte_snprintf(port, 6, "%u", paPort);
  strcat(address, port);

  char *arguments[] = {progName, flag, address};
  return forteStartInstanceGeneric(3, arguments, paResultInstance);
}

FORTE_STATUS forteStartInstanceGeneric(int argc, char *argv[], TForteInstance *paResultInstance) {

  if (nullptr == paResultInstance) {
    DEVLOG_ERROR("Provided result instance parameter is not valid\n");
    return FORTE_WRONG_PARAMETERS;
  }

  if (nullptr != *paResultInstance) {
    DEVLOG_ERROR("Provided result instance already started\n");
    return FORTE_DEVICE_ALREADY_STARTED;
  }

  if (!forte::arch::CForteArchitecture::isInitialized()) {
    DEVLOG_ERROR("The low level platform should be initialized before starting a forte instance\n");
    return FORTE_ARCHITECTURE_NOT_READY;
  }

  const std::string ipPort = forte::util::CommandLineParser::parseCommandLineArguments(argc, argv);
  if (ipPort.empty() || ipPort.find(':') == std::string::npos) {
    forte::util::CommandLineParser::listHelp();
    return FORTE_WRONG_PARAMETERS;
  }

  forte::C4diacFORTEInstance *instance = new forte::C4diacFORTEInstance();
  if (!instance->startupNewDevice(ipPort)) {
    delete instance;
    return FORTE_COULD_NOT_CREATE_DEVICE;
  }
  *paResultInstance = instance;

  return FORTE_OK;
}

void forteRequestStopInstance(TForteInstance paInstance) {
  if (!forte::arch::CForteArchitecture::isInitialized() || paInstance == nullptr) {
    return;
  }
  auto *instance = static_cast<forte::C4diacFORTEInstance *>(paInstance);
  instance->triggerDeviceShutdown();
}

void forteWaitForInstanceToStop(TForteInstance paInstance) {
  if (!forte::arch::CForteArchitecture::isInitialized() || paInstance == nullptr) {
    return;
  }
  auto *instance = static_cast<forte::C4diacFORTEInstance *>(paInstance);
  instance->awaitDeviceShutdown();
  delete instance;
}
