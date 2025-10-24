/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <thread>
#include <functional>

#include <babeltrace2/babeltrace.h>
#include <boost/test/unit_test.hpp>

#include "trace_test_config.h"
#include "common.h"
#include "forte/device.h"
#include "forte/ecet.h"
#include "../../../core/src/trace/internal/EventMessage.h"
#include "../../../core/src/trace/barectf_platform_forte.h"
#include "../fbtests/fbtesterglobalfixture.h"
#include "../../../core/src/trace/reader/utils.h"
#include "../../../stdfblib/system/src/CommandParser.h"
#include "../../../stdfblib/system/src/ForteBootFileLoader.h"
#include "../../../stdfblib/system/src/replay/deviceReplayer.h"

using namespace forte::literals;

// ******************************* //
// * Helper Methods Declarations * //
// ******************************* //

namespace forte::trace::test {
  namespace {

    /**
     * @brief Create a Non Deterministic device. It contains two resources communicating with each other and a cycle
     * event which is injecting changes in the order of the external events into one of the resources
     *
     * @param paResourceName1 name of the first resource
     * @param paResourceName2 name of the second resource
     * @param paDeviceName name of the device
     * @return the created device with the network of FBs in it
     */
    std::unique_ptr<CDevice> createNonDeterministicExample(StringId paResourceName1,
                                                           StringId paResourceName2,
                                                           StringId paDeviceName = "MyDevice"_STRID);

    /**
     * @brief Create a device from file path
     *
     * @param paDeviceName name of the device
     * @param paFilePath path to the boot file of the device
     * @return the created device
     */
    std::unique_ptr<CDevice> createDeviceFromFile(StringId paDeviceName, const std::string &paFilePath);

    void testAlgorithm(std::function<std::unique_ptr<CDevice>(void)> paCreateDevice, const std::size_t milliSeconds);

    void testTraces(CDevice &paDevice,
                    std::unordered_map<std::string, std::vector<EventMessage>> &paAllTracedEvents,
                    std::unordered_map<std::string, std::vector<EventMessage>> &paAllGeneratedEvents);

  } // namespace

  BOOST_AUTO_TEST_SUITE(replay_algorithm_test)

  BOOST_AUTO_TEST_CASE(non_deterministic_events_test) {

    auto createDevice = []() {
      auto resource1Name = "MyResource"_STRID;
      auto resource2Name = "MyResource2"_STRID;
      auto deviceName = "MyDevice"_STRID;
      return createNonDeterministicExample(resource1Name, resource2Name, deviceName);
    };
    testAlgorithm(createDevice, 5000);
  }

#ifdef FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS
  BOOST_AUTO_TEST_CASE(reference_systems_test) {
    auto createDevice = []() { return createDeviceFromFile("ReferenceSystemDevice"_STRID, REFERENCE_SYSTEMS_FILE); };

    testAlgorithm(createDevice, 60000);
  }

#endif // FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS

  BOOST_AUTO_TEST_SUITE_END()

  // ****************************** //
  // * Helper Methods Definitions * //
  // ****************************** //

  namespace {

    std::unique_ptr<CDevice>
    createNonDeterministicExample(StringId paResourceName1, StringId paResourceName2, StringId paDeviceName) {
      auto device = std::make_unique<forte::test::CTesterDevice>(paDeviceName);

      BOOST_TEST_INFO("Create Resource 1");
      BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName1, "iec61499::system::EMB_RES"_STRID, ""));

      BOOST_TEST_INFO("Create Resource 2");
      BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName2, "iec61499::system::EMB_RES"_STRID, ""));

      BOOST_TEST_INFO("Start Device");
      BOOST_CHECK(device->initialize());

      // resource 1
      {
        auto resource = dynamic_cast<CResource *>(reader::utils::getFB(device.get(), paResourceName1));

        auto cycleName = "E_CYCLE"_STRID;
        auto ctuName = "E_CTU"_STRID;
        auto publishName = "PUBLISH_1"_STRID;

        BOOST_TEST_INFO(paResourceName1);

        BOOST_TEST_INFO("Create FB Cycle");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(cycleName, "iec61499::events::E_CYCLE"_STRID, ""));

        BOOST_TEST_INFO("Create FB CTU");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(ctuName, "iec61499::events::E_CTU"_STRID, ""));

        BOOST_TEST_INFO("Create FB Publish");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(publishName, "iec61499::net::PUBLISH_1"_STRID, ""));

        SManagementCMD command;
        command.mCMD = EMGMCommandType::CreateConnection;
        command.mDestination = {};

        // Events
        BOOST_TEST_INFO("Event connection: Start.COLD -> PUBLISH.INIT");
        command.mFirstParam.push_back("START"_STRID);
        command.mFirstParam.push_back("COLD"_STRID);
        command.mSecondParam.push_back(publishName);
        command.mSecondParam.push_back("INIT"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: Publish.INITO -> Cycle.START");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(publishName);
        command.mFirstParam.push_back("INITO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(cycleName);
        command.mSecondParam.push_back("START"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: Cycle.EO -> CTU.CU");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(cycleName);
        command.mFirstParam.push_back("EO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(ctuName);
        command.mSecondParam.push_back("CU"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: CTU.CUO -> Publish.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("CUO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(publishName);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        // Data
        BOOST_TEST_INFO("Event connection: CTU.CV -> Publish.SD_1");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("CV"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(publishName);
        command.mSecondParam.push_back("SD_1"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        // Literals
        command.mCMD = EMGMCommandType::Write;

        BOOST_TEST_INFO("Literal: Cycle.DT -> T#200ms");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(cycleName);
        command.mFirstParam.push_back("DT"_STRID);
        command.mAdditionalParams = "T#200ms";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: CTU.PV -> 0");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("PV"_STRID);
        command.mAdditionalParams = "0";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: Pulbish.QI -> TRUE");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(publishName);
        command.mFirstParam.push_back("QI"_STRID);
        command.mAdditionalParams = "TRUE";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: Pulbish.ID -> 239.0.0.1:61000");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(publishName);
        command.mFirstParam.push_back("ID"_STRID);
        command.mAdditionalParams = "239.0.0.1:61000";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));
      }

      // resource 2
      {
        auto resource = dynamic_cast<CResource *>(reader::utils::getFB(device.get(), paResourceName2));

        auto cycleName = "E_CYCLE"_STRID;
        auto ctuName = "E_CTU"_STRID;
        auto subscribeName = "SUBSCRIBE_1"_STRID;
        auto addName = "ADD"_STRID;
        auto mulName = "MUL"_STRID;
        auto uint2uintFirst = "UINT2UINT"_STRID;
        auto uint2uintSecond = "UINT2UINT_1"_STRID;
        auto uint2uintThird = "UINT2UINT_2"_STRID;

        BOOST_TEST_INFO(paResourceName2);

        BOOST_TEST_INFO("Create FB Subscribe");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(subscribeName, "iec61499::net::SUBSCRIBE_1"_STRID, ""));

        BOOST_TEST_INFO("Create FB Cycle");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(cycleName, "iec61499::events::E_CYCLE"_STRID, ""));

        BOOST_TEST_INFO("Create FB CTU");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(ctuName, "iec61499::events::E_CTU"_STRID, ""));

        BOOST_TEST_INFO("Create FB ADD");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(addName, "iec61131::arithmetic::F_ADD"_STRID, ""));

        BOOST_TEST_INFO("Create FB MUL");
        BOOST_REQUIRE(EMGMResponse::Ready == resource->createFB(mulName, "iec61131::arithmetic::F_MUL"_STRID, ""));

        BOOST_TEST_INFO("Create FB UINT2UINT 1");
        BOOST_REQUIRE(EMGMResponse::Ready ==
                      resource->createFB(uint2uintFirst, "iec61131::selection::F_MOVE_1UINT"_STRID, ""));

        BOOST_TEST_INFO("Create FB UINT2UINT 2");
        BOOST_REQUIRE(EMGMResponse::Ready ==
                      resource->createFB(uint2uintSecond, "iec61131::selection::F_MOVE_1UINT"_STRID, ""));

        BOOST_TEST_INFO("Create FB UINT2UINT 3");
        BOOST_REQUIRE(EMGMResponse::Ready ==
                      resource->createFB(uint2uintThird, "iec61131::selection::F_MOVE_1UINT"_STRID, ""));

        SManagementCMD command;
        command.mCMD = EMGMCommandType::CreateConnection;
        command.mDestination = {};

        // Events
        BOOST_TEST_INFO("Event connection: Start.COLD -> SUBSCRIBE.INIT");
        command.mFirstParam.push_back("START"_STRID);
        command.mFirstParam.push_back("COLD"_STRID);
        command.mSecondParam.push_back(subscribeName);
        command.mSecondParam.push_back("INIT"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: SUBSCRIBE.INIT -> Cycle.START");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(subscribeName);
        command.mFirstParam.push_back("INITO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(cycleName);
        command.mSecondParam.push_back("START"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: Cycle.EO -> CTU.CU");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(cycleName);
        command.mFirstParam.push_back("EO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(ctuName);
        command.mSecondParam.push_back("CU"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: CTU.CUO -> ADD.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("CUO"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(addName);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: ADD.CNF -> UINT2UINT_3.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(addName);
        command.mFirstParam.push_back("CNF"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintThird);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: SUBSCRIBE.IND -> UINT2UINT_1.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(subscribeName);
        command.mFirstParam.push_back("IND"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintFirst);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: UINT2UINT_1.CNF -> MUL.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(uint2uintFirst);
        command.mFirstParam.push_back("CNF"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(mulName);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: MUL.CNF -> UINT2UINT_2.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(mulName);
        command.mFirstParam.push_back("CNF"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintSecond);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: UINT2UINT_2.CNF -> ADD.REQ");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(uint2uintSecond);
        command.mFirstParam.push_back("CNF"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(addName);
        command.mSecondParam.push_back("REQ"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        // Data
        BOOST_TEST_INFO("Event connection: CTU.CV -> ADD.IN1");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("CV"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(addName);
        command.mSecondParam.push_back("IN1"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: ADD.OUT -> UINT2UINT_3.IN");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(addName);
        command.mFirstParam.push_back("OUT"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintThird);
        command.mSecondParam.push_back("IN"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: SUBSCRIBE.RD_1 -> UINT2UINT_1.IN");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(subscribeName);
        command.mFirstParam.push_back("RD_1"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintFirst);
        command.mSecondParam.push_back("IN"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: UINT2UINT_1.OUT -> MUL.IN2");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(uint2uintFirst);
        command.mFirstParam.push_back("OUT"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(mulName);
        command.mSecondParam.push_back("IN2"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: MUL.OUT -> UINT2UINT_2.IN");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(mulName);
        command.mFirstParam.push_back("OUT"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(uint2uintSecond);
        command.mSecondParam.push_back("IN"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Event connection: UINT2UINT_2.OUT -> ADD.IN2");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(uint2uintSecond);
        command.mFirstParam.push_back("OUT"_STRID);
        command.mSecondParam.clear();
        command.mSecondParam.push_back(addName);
        command.mSecondParam.push_back("IN2"_STRID);
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        // Literals
        command.mCMD = EMGMCommandType::Write;

        BOOST_TEST_INFO("Literal: Cycle.DT -> T#200ms");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(cycleName);
        command.mFirstParam.push_back("DT"_STRID);
        command.mAdditionalParams = "T#200ms";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: CTU.PV -> 0");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(ctuName);
        command.mFirstParam.push_back("PV"_STRID);
        command.mAdditionalParams = "0";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: SUBSCRIBE.QI -> TRUE");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(subscribeName);
        command.mFirstParam.push_back("QI"_STRID);
        command.mAdditionalParams = "TRUE";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: Pulbish.ID -> 239.0.0.1:61000");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(subscribeName);
        command.mFirstParam.push_back("ID"_STRID);
        command.mAdditionalParams = "239.0.0.1:61000";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));

        BOOST_TEST_INFO("Literal: MUL.IN1 -> UINT#10");
        command.mFirstParam.clear();
        command.mFirstParam.push_back(mulName);
        command.mFirstParam.push_back("IN1"_STRID);
        command.mAdditionalParams = "UINT#10";
        BOOST_REQUIRE(EMGMResponse::Ready == resource->executeMGMCommand(command));
      }

      return device;
    }

    std::unique_ptr<CDevice> createDeviceFromFile(StringId paDeviceName, const std::string &paFilePath) {
      auto device = std::make_unique<forte::test::CTesterDevice>(paDeviceName);
      device->initialize();
      iec61499::system::CommandParser commandParser(*device);

      iec61499::system::ForteBootFileLoader fileLoader(
          [&commandParser](const char *paDest, char *paCommand) -> bool {
            return EMGMResponse::Ready == commandParser.parseAndExecuteMGMCommand(paDest, paCommand);
          },
          paFilePath);
      BOOST_REQUIRE(iec61499::system::LoadBootResult::LOAD_RESULT_OK == fileLoader.loadBootFile());
      return device;
    }

    void testAlgorithm(std::function<std::unique_ptr<CDevice>(void)> paCreateDevice, const std::size_t milliSeconds) {
      prepareTraceTest("metadata");

      reader::utils::setFactoriesSettings({});

      auto killDevice = [](CDevice &paDevice, std::size_t paMillisecondsToSleepBeforeKilling = 1000) {
        // wait for all events to be triggered
        std::this_thread::sleep_for(std::chrono::milliseconds(paMillisecondsToSleepBeforeKilling));

        paDevice.changeExecutionState(EMGMCommandType::Kill);
        paDevice.awaitShutdown();
      };

      {
        auto device = paCreateDevice();
        device->startDevice();
        killDevice(*device, milliSeconds);
      }

      // disable logging
      BarectfPlatformFORTE::setup("");

      auto allTracedEvents = reader::utils::getEventMessages(cgCTFOutputDir).value();

      reader::utils::setFactoriesSettings(
          {"Fake"_STRID, "FakeTime"_STRID, CFlexibleTracer::AvailableTracers::Internal});

      // test with reproduce all
      {
        auto device = paCreateDevice();

        auto allTracedExternalEvents = reader::utils::filterEventsForReplayDevice(allTracedEvents, *device);

        std::unordered_map<std::string, std::vector<EventMessage>> reproducedEvents;

        {
          auto deviceReplayer = iec61499::system::CDeviceReplayer(*device, allTracedExternalEvents);
          device->startDevice();
          reproducedEvents = deviceReplayer.reproduceAll();
        }

        killDevice(*device);

        testTraces(*device, allTracedEvents, reproducedEvents);
      }

      // test with reproduce next event
      {
        auto device = paCreateDevice();

        auto allTracedExternalEvents = reader::utils::filterEventsForReplayDevice(allTracedEvents, *device);

        auto deviceReplayer = iec61499::system::CDeviceReplayer(*device, allTracedExternalEvents);

        device->startDevice();

        for (auto container : device->getChildren()) {
          auto resourceName = container->getInstanceName();
          while (deviceReplayer.reproduceNextEvent(resourceName))
            ;
        }

        auto reproducedEvents = deviceReplayer.getGeneratedEvents();

        killDevice(*device);

        testTraces(*device, allTracedEvents, reproducedEvents);
      }
    }

    void testTraces(CDevice &paDevice,
                    std::unordered_map<std::string, std::vector<EventMessage>> &paAllTracedEvents,
                    std::unordered_map<std::string, std::vector<EventMessage>> &paAllGeneratedEvents) {

      // To test the algorithm, we compare only the outputs events to the generated ones
      auto isInteretingType = [](const EventMessage &paMessage) {
        auto messageType = paMessage.getEventType();
        return messageType == "sendOutputEvent";
      };

      auto allInterestingEvents = reader::utils::filterEvents(paAllTracedEvents, isInteretingType);

      allInterestingEvents.erase(paDevice.getInstanceName());

      auto interestingGeneratedMessages = reader::utils::filterEvents(paAllGeneratedEvents, isInteretingType);

      checkMessages(allInterestingEvents, interestingGeneratedMessages);
    }

  } // namespace
} // namespace forte::trace::test
