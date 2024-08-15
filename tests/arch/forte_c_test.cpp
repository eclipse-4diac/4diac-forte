/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#define BOOST_TEST_MODULE FORTE_C_TEST
#include <boost/test/included/unit_test.hpp>

#include "../../src/arch/c_interface/forte_c.h"

#include <chrono>
#include <thread>

BOOST_AUTO_TEST_CASE(forte_c_interface)
{
  TForteInstance instance{nullptr};

  // forteStartInstance
  // invalid instance result
  BOOST_TEST(forteStartInstance(61499, nullptr) == FORTE_WRONG_PARAMETERS);
  
  // invalid port number, greater than maximum 65535
  BOOST_TEST(forteStartInstance(65536, &instance) == FORTE_WRONG_PARAMETERS);

  char executable[] = "forte";
  char ipPortFlag[] = "-c";

  int validArgc = 1;
  char* validArgV[] = {executable};

  // invalid ipPort address and parameter
  {
    int argc = 3;
    char invalidAddress[] = "localhost";
    char* argv[] = {executable, ipPortFlag, invalidAddress};
    BOOST_TEST(forteStartInstanceGeneric(argc, argv, nullptr) == FORTE_WRONG_PARAMETERS);

    char nonExistingParameter[] = "--unknown";
    char* argv2[] = {executable, nonExistingParameter};
    BOOST_TEST(forteStartInstanceGeneric(argc, argv2, nullptr) == FORTE_WRONG_PARAMETERS);
  }

  // architecture not initialized
  {
    BOOST_TEST(forteStartInstanceGeneric(validArgc, validArgV, &instance) == FORTE_ARCHITECTURE_NOT_READY);
  }

  // valid case and invalid case based on an existing running device
  {
    BOOST_TEST(forteGlobalInitialize(validArgc, validArgV) == 0);

    // double initialization of the architecture should not fail
    BOOST_TEST(forteGlobalInitialize(validArgc, validArgV) == 0);

    BOOST_TEST(forteStartInstanceGeneric(validArgc, validArgV, &instance) == FORTE_OK);

    // try starting on already started device
    BOOST_TEST(forteStartInstanceGeneric(validArgc, validArgV, &instance) == FORTE_DEVICE_ALREADY_STARTED);

    // another instance on same port
    TForteInstance instance2{nullptr};

    // stopping non runnning instance should no crash
    forteRequestStopInstance(instance2);
    forteWaitForInstanceToStop(instance2);

    // let it sleep for some time to since if too fast, the stopping signal 
    // comes too early
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // stop running instance
    forteRequestStopInstance(instance);
    forteWaitForInstanceToStop(instance);

    BOOST_TEST(forteGlobalDeinitialize() == 0);

    // double de-initialization of the architecture should not fail
    BOOST_TEST(forteGlobalDeinitialize() == 0);
  }
}

