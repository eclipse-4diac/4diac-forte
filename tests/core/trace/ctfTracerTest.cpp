/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <functional>
#include <thread>

#include <babeltrace2/babeltrace.h>
#include <boost/test/unit_test.hpp>

#include "../fbtests/fbtesterglobalfixture.h"
#include "common.h"
#include "trace_test_config.h"
#include "../../../../core/src/trace/internal/EventMessage.h"
#include "../../../../core/src/trace/reader/utils.h"
#include "forte/device.h"
#include "forte/ecet.h"
#include "forte/mgmcmdstruct.h"
#include "../../../../core/src/trace/barectf_platform_forte.h"

using namespace forte::literals;

// ******************************* //
// * Helper Methods Declarations * //
// ******************************* //

namespace forte::trace::test {
  namespace {

    /**
     * @brief create a FB network with a E_SWITCH and E_CTU  with some connections
     *
     * @param paResourceName name for the resource where the network is located
     * @param paDeviceName name for the device
     *
     * @return the created device wuth the network of FB in it
     */
    std::unique_ptr<CDevice> createExampleDevice(StringId paResourceName, StringId paDeviceName = "MyDevice"_STRID);
  } // namespace

  BOOST_AUTO_TEST_SUITE(tracer_test)

  BOOST_AUTO_TEST_CASE(sequential_events_test) {
    prepareTraceTest("metadata");

    auto resourceName = "MyResource"_STRID;
    auto deviceName = "MyDevice"_STRID;

    // The inner scope is to make sure the destructors of the resources are
    // called which flushes the output
    {
      auto device = createExampleDevice(resourceName, deviceName);

      auto resource = dynamic_cast<CResource *>(reader::utils::getFB(device.get(), resourceName));

      device->startDevice();
      // wait for all events to be triggered
      while (resource->getResourceEventExecution()->isProcessingEvents()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
      device->changeExecutionState(EMGMCommandType::Stop);
      resource->getResourceEventExecution()->joinEventChainExecutionThread();
    }

    // disable logging
    BarectfPlatformFORTE::setup("");

    std::unordered_map<std::string, std::vector<EventMessage>> expectedMessages;

    auto eventCounter = 0; // used only for replay debugging, but it's easier to keep here instaed of extra #ifdef

    auto addInitiaOrFinalEvent = [&eventCounter](std::vector<EventMessage> &paMessages, bool paIsInitial) {
      paMessages.emplace_back("receiveInputEvent",
                              std::make_unique<FBInputEventPayload>("iec61499::events::E_RESTART", "START", 65534), 0);
      paMessages.emplace_back("sendOutputEvent",
                              std::make_unique<FBOutputEventPayload>("iec61499::events::E_RESTART", "START",
                                                                     paIsInitial ? 0 : 2
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                     ,
                                                                     eventCounter, std::vector<std::string>{}
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                     ),
                              0);
    };

    // device resource has no events
    expectedMessages[std::string(deviceName)] = {};

    // default resource in the test device
    expectedMessages["EMB_RES"] = {};

    auto &embResMessages = expectedMessages["EMB_RES"];
    addInitiaOrFinalEvent(embResMessages, true);
    addInitiaOrFinalEvent(embResMessages, false);

    // resource with example FBs
    expectedMessages[std::string(resourceName)] = {};

    auto &resourceMessages = expectedMessages[std::string(resourceName)];

    // timestamp cannot properly be tested, so setting everythin to zero
    addInitiaOrFinalEvent(resourceMessages, true);

    eventCounter++;

    resourceMessages.emplace_back("receiveInputEvent",
                                  std::make_unique<FBInputEventPayload>("iec61499::events::E_CTU", "Counter", 0), 0);
    resourceMessages.emplace_back(
        "instanceData",
        std::make_unique<FBInstanceDataPayload>("iec61499::events::E_CTU", "Counter", std::vector<std::string>{"1"},
                                                std::vector<std::string>{"FALSE", "0"}, std::vector<std::string>{},
                                                std::vector<std::string>{}),
        0);

    resourceMessages.emplace_back("sendOutputEvent",
                                  std::make_unique<FBOutputEventPayload>("iec61499::events::E_CTU", "Counter", 0
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ,
                                                                         eventCounter,
                                                                         std::vector<std::string>{"TRUE", "1"}
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ),
                                  0);
    resourceMessages.emplace_back("outputData",
                                  std::make_unique<FBDataPayload>("iec61499::events::E_CTU", "Counter", 0, "TRUE"), 0);
    resourceMessages.emplace_back("outputData",
                                  std::make_unique<FBDataPayload>("iec61499::events::E_CTU", "Counter", 1, "1"), 0);
    eventCounter++;

    resourceMessages.emplace_back("receiveInputEvent",
                                  std::make_unique<FBInputEventPayload>("iec61499::events::E_SWITCH", "Switch", 0), 0);
    resourceMessages.emplace_back(
        "instanceData",
        std::make_unique<FBInstanceDataPayload>("iec61499::events::E_SWITCH", "Switch",
                                                std::vector<std::string>{"FALSE"}, std::vector<std::string>{},
                                                std::vector<std::string>{}, std::vector<std::string>{}),
        0);
    resourceMessages.emplace_back(
        "inputData", std::make_unique<FBDataPayload>("iec61499::events::E_SWITCH", "Switch", 0, "TRUE"), 0);

    resourceMessages.emplace_back("sendOutputEvent",
                                  std::make_unique<FBOutputEventPayload>("iec61499::events::E_SWITCH", "Switch", 1
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ,
                                                                         eventCounter, std::vector<std::string>{}
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ),
                                  0);
    eventCounter++;

    resourceMessages.emplace_back("receiveInputEvent",
                                  std::make_unique<FBInputEventPayload>("iec61499::events::E_CTU", "Counter", 1), 0);
    resourceMessages.emplace_back(
        "instanceData",
        std::make_unique<FBInstanceDataPayload>("iec61499::events::E_CTU", "Counter", std::vector<std::string>{"1"},
                                                std::vector<std::string>{"TRUE", "1"}, std::vector<std::string>{},
                                                std::vector<std::string>{}),
        0);

    resourceMessages.emplace_back("sendOutputEvent",
                                  std::make_unique<FBOutputEventPayload>("iec61499::events::E_CTU", "Counter", 1
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ,
                                                                         eventCounter,
                                                                         std::vector<std::string>{"TRUE", "1"}
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                                                         ),
                                  0);
    resourceMessages.emplace_back("outputData",
                                  std::make_unique<FBDataPayload>("iec61499::events::E_CTU", "Counter", 0, "FALSE"), 0);
    resourceMessages.emplace_back("outputData",
                                  std::make_unique<FBDataPayload>("iec61499::events::E_CTU", "Counter", 1, "0"), 0);

    addInitiaOrFinalEvent(resourceMessages, false);

    auto ctfMessages = reader::utils::getEventMessages(cgCTFOutputDir).value();

    checkMessages(expectedMessages, ctfMessages);
  }

  BOOST_AUTO_TEST_SUITE_END()

  // ****************************** //
  // * Helper Methods Definitions * //
  // ****************************** //

  namespace {

    std::unique_ptr<CDevice> createExampleDevice(StringId paResourceName, StringId paDeviceName) {
      auto device = std::make_unique<forte::test::CTesterDevice>(paDeviceName);

      BOOST_TEST_INFO("Create Resource");
      BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName, "iec61499::system::EMB_RES"_STRID, ""));

      BOOST_TEST_INFO("Start Device");
      BOOST_CHECK(device->initialize());

      auto resource = dynamic_cast<CResource *>(reader::utils::getFB(device.get(), paResourceName));

      auto startInstanceName = "START"_STRID;
      auto counterInstanceName = "Counter"_STRID;
      auto switchInstanceName = "Switch"_STRID;

      BOOST_TEST_INFO("Create E_CTU");
      BOOST_CHECK(EMGMResponse::Ready == resource->createFB(counterInstanceName, "iec61499::events::E_CTU"_STRID, ""));

      BOOST_TEST_INFO("Create E_SWITCH");
      BOOST_CHECK(EMGMResponse::Ready ==
                  resource->createFB(switchInstanceName, "iec61499::events::E_SWITCH"_STRID, ""));

      SManagementCMD command;
      command.mCMD = EMGMCommandType::CreateConnection;
      command.mDestination = {};

      BOOST_TEST_INFO("Event connection: Start.COLD -> Counter.CU");
      command.mFirstParam.push_back(startInstanceName);
      command.mFirstParam.push_back("COLD"_STRID);
      command.mSecondParam.push_back(counterInstanceName);
      command.mSecondParam.push_back("CU"_STRID);

      BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: Counter.CUO -> Switch.EI");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(counterInstanceName);
      command.mFirstParam.push_back("CUO"_STRID);
      command.mSecondParam.clear();
      command.mSecondParam.push_back(switchInstanceName);
      command.mSecondParam.push_back("EI"_STRID);
      BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Data connection: Counter.Q -> Switch.G ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(counterInstanceName);
      command.mFirstParam.push_back("Q"_STRID);
      command.mSecondParam.clear();
      command.mSecondParam.push_back(switchInstanceName);
      command.mSecondParam.push_back("G"_STRID);
      BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO(" Data constant value: Counter.PV = 1");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(counterInstanceName);
      command.mFirstParam.push_back("PV"_STRID);
      BOOST_CHECK(EMGMResponse::Ready == resource->writeValue(command.mFirstParam, std::string("1"), false));

      BOOST_TEST_INFO("Event connection: Switch.EO1 -> Counter.R ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(switchInstanceName);
      command.mFirstParam.push_back("EO1"_STRID);
      command.mSecondParam.clear();
      command.mSecondParam.push_back(counterInstanceName);
      command.mSecondParam.push_back("R"_STRID);
      BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

      return device;
    }

  } // namespace
} // namespace forte::trace::test
