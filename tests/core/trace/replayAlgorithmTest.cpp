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

#include "config.h"
#include "common.h"
#include "device.h"
#include "ecet.h"
#include "core/trace/internal/EventMessage.h"
#include "trace/barectf_platform_forte.h"
#include "../fbtests/fbtesterglobalfixture.h"
#include "stdfblib/ita/replay/deviceReplayer.h"
#include "core/trace/reader/utils.h"
#include "ForteBootFileLoader.h"
#include "CommandParser.h"

USE_STRING_ID(ADD);
USE_STRING_ID(CNF);
USE_STRING_ID(COLD);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(CV);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_CYCLE);
USE_STRING_ID(EMB_RES);
USE_STRING_ID(EO);
USE_STRING_ID(F_ADD);
USE_STRING_ID(F_MUL);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MUL);
USE_STRING_ID(MyDevice);
USE_STRING_ID(MyResource);
USE_STRING_ID(MyResource2);
USE_STRING_ID(OUT);
USE_STRING_ID(PUBLISH_1);
USE_STRING_ID(PV);
USE_STRING_ID(QI);
USE_STRING_ID(RD_1);
USE_STRING_ID(ReferenceSystemDevice);
USE_STRING_ID(REQ);
USE_STRING_ID(SD_1);
USE_STRING_ID(START);
USE_STRING_ID(SUBSCRIBE_1);
USE_STRING_ID(UINT2UINT_1);
USE_STRING_ID(UINT2UINT_2);
USE_STRING_ID(UINT2UINT);

// ******************************* //
// * Helper Methods Declarations * //
// ******************************* //

namespace {

  /**
   * @brief Create a Non Deterministic device. It contains two resources communicating with each other and a cycle event
   * which is injecting changes in the order of the external events into one of the resources
   *
   * @param paResourceName1 name of the first resource
   * @param paResourceName2 name of the second resource
   * @param paDeviceName name of the device
   * @return the created device with the network of FBs in it
   */
  std::unique_ptr<CDevice> createNonDeterministicExample(CStringDictionary::TStringId paResourceName1,
                                                         CStringDictionary::TStringId paResourceName2,
                                                         CStringDictionary::TStringId paDeviceName = STRID(MyDevice));

  /**
   * @brief Create a device from file path
   *
   * @param paDeviceName name of the device
   * @param paFilePath path to the boot file of the device
   * @return the created device
   */
  std::unique_ptr<CDevice> createDeviceFromFile(CStringDictionary::TStringId paDeviceName,
                                                const std::string &paFilePath);

  void testAlgorithm(std::function<std::unique_ptr<CDevice>(void)> paCreateDevice, const std::size_t milliSeconds);

  void testTraces(CDevice &paDevice,
                  std::unordered_map<std::string, std::vector<EventMessage>> &paAllTracedEvents,
                  std::unordered_map<std::string, std::vector<EventMessage>> &paAllGeneratedEvents);

} // namespace

BOOST_AUTO_TEST_SUITE(replay_algorithm_test)

BOOST_AUTO_TEST_CASE(non_deterministic_events_test) {

  auto createDevice = []() {
    auto resource1Name = STRID(MyResource);
    auto resource2Name = STRID(MyResource2);
    auto deviceName = STRID(MyDevice);
    return createNonDeterministicExample(resource1Name, resource2Name, deviceName);
  };
  testAlgorithm(createDevice, 5000);
}

#ifdef FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS
BOOST_AUTO_TEST_CASE(reference_systems_test) {
  auto createDevice = []() { return createDeviceFromFile(STRID(ReferenceSystemDevice), REFERENCE_SYSTEMS_FILE); };

  testAlgorithm(createDevice, 60000);
}

#endif // FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS

BOOST_AUTO_TEST_SUITE_END()

// ****************************** //
// * Helper Methods Definitions * //
// ****************************** //

namespace {

  std::unique_ptr<CDevice> createNonDeterministicExample(CStringDictionary::TStringId paResourceName1,
                                                         CStringDictionary::TStringId paResourceName2,
                                                         CStringDictionary::TStringId paDeviceName) {
    auto device = std::make_unique<CTesterDevice>(paDeviceName);

    BOOST_TEST_INFO("Create Resource 1");
    BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName1, STRID(EMB_RES)));

    BOOST_TEST_INFO("Create Resource 2");
    BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName2, STRID(EMB_RES)));

    BOOST_TEST_INFO("Start Device");
    BOOST_CHECK(device->initialize());

    // resource 1
    {
      auto resource = dynamic_cast<CResource *>(forte::trace::reader::utils::getFB(device.get(), paResourceName1));

      auto cycleName = STRID(E_CYCLE);
      auto ctuName = STRID(E_CTU);
      auto publishName = STRID(PUBLISH_1);

      BOOST_TEST_INFO(CStringDictionary::get(paResourceName1));

      BOOST_TEST_INFO("Create FB Cycle");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(cycleName, STRID(E_CYCLE)));

      BOOST_TEST_INFO("Create FB CTU");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(ctuName, STRID(E_CTU)));

      BOOST_TEST_INFO("Create FB Publish");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(publishName, STRID(PUBLISH_1)));

      forte::core::SManagementCMD command;
      command.mCMD = EMGMCommandType::CreateConnection;
      command.mDestination = CStringDictionary::scmInvalidStringId;

      // Events
      BOOST_TEST_INFO("Event connection: Start.COLD -> PUBLISH.INIT");
      command.mFirstParam.push_back(STRID(START));
      command.mFirstParam.push_back(STRID(COLD));
      command.mSecondParam.push_back(publishName);
      command.mSecondParam.push_back(STRID(INIT));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: Publish.INITO -> Cycle.START");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(publishName);
      command.mFirstParam.push_back(STRID(INITO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(cycleName);
      command.mSecondParam.push_back(STRID(START));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: Cycle.EO -> CTU.CU");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(cycleName);
      command.mFirstParam.push_back(STRID(EO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(ctuName);
      command.mSecondParam.push_back(STRID(CU));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: CTU.CUO -> Publish.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(CUO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(publishName);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      // Data
      BOOST_TEST_INFO("Event connection: CTU.CV -> Publish.SD_1");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(CV));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(publishName);
      command.mSecondParam.push_back(STRID(SD_1));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      // Literals
      command.mCMD = EMGMCommandType::Write;

      BOOST_TEST_INFO("Literal: Cycle.DT -> T#200ms");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(cycleName);
      command.mFirstParam.push_back(STRID(DT));
      command.mAdditionalParams = "T#200ms";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: CTU.PV -> 0");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(PV));
      command.mAdditionalParams = "0";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: Pulbish.QI -> TRUE");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(publishName);
      command.mFirstParam.push_back(STRID(QI));
      command.mAdditionalParams = "TRUE";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: Pulbish.ID -> 239.0.0.1:61000");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(publishName);
      command.mFirstParam.push_back(STRID(ID));
      command.mAdditionalParams = "239.0.0.1:61000";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));
    }

    // resource 2
    {
      auto resource = dynamic_cast<CResource *>(forte::trace::reader::utils::getFB(device.get(), paResourceName2));

      auto cycleName = STRID(E_CYCLE);
      auto ctuName = STRID(E_CTU);
      auto subscribeName = STRID(SUBSCRIBE_1);
      auto addName = STRID(ADD);
      auto mulName = STRID(MUL);
      auto uint2uintFirst = STRID(UINT2UINT);
      auto uint2uintSecond = STRID(UINT2UINT_1);
      auto uint2uintThird = STRID(UINT2UINT_2);

      BOOST_TEST_INFO(CStringDictionary::get(paResourceName2));

      BOOST_TEST_INFO("Create FB Subscribe");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(subscribeName, STRID(SUBSCRIBE_1)));

      BOOST_TEST_INFO("Create FB Cycle");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(cycleName, STRID(E_CYCLE)));

      BOOST_TEST_INFO("Create FB CTU");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(ctuName, STRID(E_CTU)));

      BOOST_TEST_INFO("Create FB ADD");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(addName, STRID(F_ADD)));

      BOOST_TEST_INFO("Create FB MUL");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(mulName, STRID(F_MUL)));

      BOOST_TEST_INFO("Create FB UINT2UINT 1");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(uint2uintFirst, STRID(UINT2UINT)));

      BOOST_TEST_INFO("Create FB UINT2UINT 2");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(uint2uintSecond, STRID(UINT2UINT)));

      BOOST_TEST_INFO("Create FB UINT2UINT 3");
      BOOST_ASSERT(EMGMResponse::Ready == resource->createFB(uint2uintThird, STRID(UINT2UINT)));

      forte::core::SManagementCMD command;
      command.mCMD = EMGMCommandType::CreateConnection;
      command.mDestination = CStringDictionary::scmInvalidStringId;

      // Events
      BOOST_TEST_INFO("Event connection: Start.COLD -> SUBSCRIBE.INIT");
      command.mFirstParam.push_back(STRID(START));
      command.mFirstParam.push_back(STRID(COLD));
      command.mSecondParam.push_back(subscribeName);
      command.mSecondParam.push_back(STRID(INIT));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: SUBSCRIBE.INIT -> Cycle.START");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(subscribeName);
      command.mFirstParam.push_back(STRID(INITO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(cycleName);
      command.mSecondParam.push_back(STRID(START));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: Cycle.EO -> CTU.CU");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(cycleName);
      command.mFirstParam.push_back(STRID(EO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(ctuName);
      command.mSecondParam.push_back(STRID(CU));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: CTU.CUO -> ADD.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(CUO));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(addName);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: ADD.CNF -> UINT2UINT_3.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(addName);
      command.mFirstParam.push_back(STRID(CNF));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintThird);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: SUBSCRIBE.IND -> UINT2UINT_1.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(subscribeName);
      command.mFirstParam.push_back(STRID(IND));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintFirst);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: UINT2UINT_1.CNF -> MUL.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(uint2uintFirst);
      command.mFirstParam.push_back(STRID(CNF));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(mulName);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: MUL.CNF -> UINT2UINT_2.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(mulName);
      command.mFirstParam.push_back(STRID(CNF));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintSecond);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: UINT2UINT_2.CNF -> ADD.REQ");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(uint2uintSecond);
      command.mFirstParam.push_back(STRID(CNF));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(addName);
      command.mSecondParam.push_back(STRID(REQ));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      // Data
      BOOST_TEST_INFO("Event connection: CTU.CV -> ADD.IN1");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(CV));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(addName);
      command.mSecondParam.push_back(STRID(IN1));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: ADD.OUT -> UINT2UINT_3.IN");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(addName);
      command.mFirstParam.push_back(STRID(OUT));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintThird);
      command.mSecondParam.push_back(STRID(IN));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: SUBSCRIBE.RD_1 -> UINT2UINT_1.IN");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(subscribeName);
      command.mFirstParam.push_back(STRID(RD_1));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintFirst);
      command.mSecondParam.push_back(STRID(IN));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: UINT2UINT_1.OUT -> MUL.IN2");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(uint2uintFirst);
      command.mFirstParam.push_back(STRID(OUT));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(mulName);
      command.mSecondParam.push_back(STRID(IN2));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: MUL.OUT -> UINT2UINT_2.IN");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(mulName);
      command.mFirstParam.push_back(STRID(OUT));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(uint2uintSecond);
      command.mSecondParam.push_back(STRID(IN));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Event connection: UINT2UINT_2.OUT -> ADD.IN2");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(uint2uintSecond);
      command.mFirstParam.push_back(STRID(OUT));
      command.mSecondParam.clear();
      command.mSecondParam.push_back(addName);
      command.mSecondParam.push_back(STRID(IN2));
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      // Literals
      command.mCMD = EMGMCommandType::Write;

      BOOST_TEST_INFO("Literal: Cycle.DT -> T#200ms");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(cycleName);
      command.mFirstParam.push_back(STRID(DT));
      command.mAdditionalParams = "T#200ms";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: CTU.PV -> 0");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(ctuName);
      command.mFirstParam.push_back(STRID(PV));
      command.mAdditionalParams = "0";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: SUBSCRIBE.QI -> TRUE");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(subscribeName);
      command.mFirstParam.push_back(STRID(QI));
      command.mAdditionalParams = "TRUE";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: Pulbish.ID -> 239.0.0.1:61000");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(subscribeName);
      command.mFirstParam.push_back(STRID(ID));
      command.mAdditionalParams = "239.0.0.1:61000";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));

      BOOST_TEST_INFO("Literal: MUL.IN1 -> UINT#10");
      command.mFirstParam.clear();
      command.mFirstParam.push_back(mulName);
      command.mFirstParam.push_back(STRID(IN1));
      command.mAdditionalParams = "UINT#10";
      BOOST_ASSERT(EMGMResponse::Ready == resource->executeMGMCommand(command));
    }

    return device;
  }

  std::unique_ptr<CDevice> createDeviceFromFile(CStringDictionary::TStringId paDeviceName,
                                                const std::string &paFilePath) {
    auto device = std::make_unique<CTesterDevice>(paDeviceName);
    device->initialize();
    forte::ita::CommandParser commandParser(*device);

    ForteBootFileLoader fileLoader(
        [&commandParser](const char *paDest, char *paCommand) -> bool {
          return EMGMResponse::Ready == commandParser.parseAndExecuteMGMCommand(paDest, paCommand);
        },
        paFilePath);
    BOOST_ASSERT(LoadBootResult::LOAD_RESULT_OK == fileLoader.loadBootFile());
    return device;
  }

  void testAlgorithm(std::function<std::unique_ptr<CDevice>(void)> paCreateDevice, const std::size_t milliSeconds) {
    forte::tests::traces::prepareTraceTest("metadata");

    forte::trace::reader::utils::setFactoriesSettings({});

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

    auto allTracedEvents = forte::trace::reader::utils::getEventMessages(CTF_OUTPUT_DIR).value();

    forte::trace::reader::utils::setFactoriesSettings({EcetFactory::AvailableEcets::fake,
                                                       TimerHandlerFactory::AvailableTimers::fakeTimer,
                                                       CFlexibleTracer::AvailableTracers::Internal});

    // test with reproduce all
    {
      auto device = paCreateDevice();

      auto allTracedExternalEvents = forte::trace::reader::utils::filterEventsForReplayDevice(allTracedEvents, *device);

      std::unordered_map<std::string, std::vector<EventMessage>> reproducedEvents;

      {
        auto deviceReplayer = CDeviceReplayer(*device, allTracedExternalEvents);
        device->startDevice();
        reproducedEvents = deviceReplayer.reproduceAll();
      }

      killDevice(*device);

      testTraces(*device, allTracedEvents, reproducedEvents);
    }

    // test with reproduce next event
    {
      auto device = paCreateDevice();

      auto allTracedExternalEvents = forte::trace::reader::utils::filterEventsForReplayDevice(allTracedEvents, *device);

      auto deviceReplayer = CDeviceReplayer(*device, allTracedExternalEvents);

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

    auto allInterestingEvents = forte::trace::reader::utils::filterEvents(paAllTracedEvents, isInteretingType);

    allInterestingEvents.erase(paDevice.getInstanceName());

    auto interestingGeneratedMessages =
        forte::trace::reader::utils::filterEvents(paAllGeneratedEvents, isInteretingType);

    forte::tests::traces::checkMessages(allInterestingEvents, interestingGeneratedMessages);
  }

} // namespace
