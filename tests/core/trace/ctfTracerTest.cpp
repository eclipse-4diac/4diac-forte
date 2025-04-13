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

#include <thread>
#include <functional>

#include <babeltrace2/babeltrace.h>
#include <boost/test/unit_test.hpp>

#include "config.h"
#include "common.h"
#include "core/trace/reader/utils.h"
#include "device.h"
#include "ecet.h"
#include "core/trace/internal/EventMessage.h"
#include "trace/barectf_platform_forte.h"
#include "../fbtests/fbtesterglobalfixture.h"

USE_STRING_ID(COLD)
USE_STRING_ID(Counter)
USE_STRING_ID(CU)
USE_STRING_ID(CUO)
USE_STRING_ID(E_CTU)
USE_STRING_ID(E_SWITCH)
USE_STRING_ID(EI)
USE_STRING_ID(EMB_RES)
USE_STRING_ID(EO1)
USE_STRING_ID(G)
USE_STRING_ID(MyDevice)
USE_STRING_ID(MyResource)
USE_STRING_ID(PV)
USE_STRING_ID(Q)
USE_STRING_ID(R)
USE_STRING_ID(START)
USE_STRING_ID(Switch)

// ******************************* //
// * Helper Methods Declarations * //
// ******************************* //

namespace {

  /**
   * @brief create a FB network with a E_SWITCH and E_CTU  with some connections 
   * 
   * @param paResourceName name for the resource where the network is located
   * @param paDeviceName name for the device
   * 
   * @return the created device wuth the network of FB in it
  */
  std::unique_ptr<CDevice> createExampleDevice(CStringDictionary::TStringId paResourceName, CStringDictionary::TStringId paDeviceName = STRID(MyDevice));
}

BOOST_AUTO_TEST_SUITE (tracer_test)

BOOST_AUTO_TEST_CASE(sequential_events_test) {
  forte::tests::traces::prepareTraceTest("metadata");

  auto resourceName = STRID(MyResource);
  auto deviceName = STRID(MyDevice);

  // The inner scope is to make sure the destructors of the resources are 
  // called which flushes the output
  {
    auto device = createExampleDevice(resourceName, deviceName);

    auto resource = dynamic_cast<CResource*>(forte::trace::reader::utils::getFB(device.get(), resourceName));

    device->startDevice();
    // wait for all events to be triggered
    while(resource->getResourceEventExecution()->isProcessingEvents()){
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    device->changeExecutionState(EMGMCommandType::Stop);
    resource->getResourceEventExecution()->joinEventChainExecutionThread();
  }

  // disable logging 
  BarectfPlatformFORTE::setup("");

  std::unordered_map<std::string, std::vector<EventMessage>> expectedMessages;

  auto addInitiaOrFinalEvent = [](std::vector<EventMessage>& paMessages){
     paMessages.emplace_back("receiveInputEvent", std::make_unique<FBInputEventPayload>("E_RESTART", "START", 65534), 0);
  };

  // device resource has no events
  expectedMessages[CStringDictionary::get(deviceName)] = {}; 

  // default resource in the test device
  expectedMessages[CStringDictionary::get(STRID(EMB_RES))] = {}; 

  auto& embResMessages = expectedMessages[CStringDictionary::get(STRID(EMB_RES))];
  addInitiaOrFinalEvent(embResMessages);
  addInitiaOrFinalEvent(embResMessages);

  // resource with example FBs
  expectedMessages[CStringDictionary::get(resourceName)] = {};

  auto& resourceMessages = expectedMessages[CStringDictionary::get(resourceName)];

  auto eventCounter = 0;

  // timestamp cannot properly be tested, so setting everythin to zero
  addInitiaOrFinalEvent(resourceMessages);
  resourceMessages.emplace_back("sendOutputEvent", std::make_unique<FBOutputEventPayload>("E_RESTART", "START", 0
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
  , eventCounter, std::vector<std::string>{}), 0
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
  );
  eventCounter++;

  resourceMessages.emplace_back("receiveInputEvent", std::make_unique<FBInputEventPayload>("E_CTU", "Counter", 0),0);
  resourceMessages.emplace_back("instanceData", std::make_unique<FBInstanceDataPayload>("E_CTU", "Counter", std::vector<std::string>{"1"}, std::vector<std::string>{"FALSE", "0"}, std::vector<std::string>{}, std::vector<std::string>{}), 0);

  resourceMessages.emplace_back("sendOutputEvent", std::make_unique<FBOutputEventPayload>("E_CTU", "Counter", 0
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
  , eventCounter, std::vector<std::string>{"TRUE", "1"}),0
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
  );
  resourceMessages.emplace_back("outputData", std::make_unique<FBDataPayload>("E_CTU", "Counter", 0, "TRUE"), 0);
  resourceMessages.emplace_back("outputData", std::make_unique<FBDataPayload>("E_CTU", "Counter", 1, "1"), 0);
  eventCounter++;

  resourceMessages.emplace_back("receiveInputEvent", std::make_unique<FBInputEventPayload>("E_SWITCH", "Switch", 0),0);
  resourceMessages.emplace_back("instanceData", std::make_unique<FBInstanceDataPayload>("E_SWITCH", "Switch", std::vector<std::string>{"FALSE"}, std::vector<std::string>{}, std::vector<std::string>{}, std::vector<std::string>{}), 0);
  resourceMessages.emplace_back("inputData", std::make_unique<FBDataPayload>("E_SWITCH", "Switch", 0, "TRUE"), 0);


  resourceMessages.emplace_back("sendOutputEvent", std::make_unique<FBOutputEventPayload>("E_SWITCH", "Switch", 1
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
  , eventCounter, std::vector<std::string>{}),0
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
  );  
  eventCounter++;

  resourceMessages.emplace_back("receiveInputEvent", std::make_unique<FBInputEventPayload>("E_CTU", "Counter", 1),0);
  resourceMessages.emplace_back("instanceData", std::make_unique<FBInstanceDataPayload>("E_CTU", "Counter", std::vector<std::string>{"1"}, std::vector<std::string>{"TRUE", "1"}, std::vector<std::string>{}, std::vector<std::string>{}),0);
  resourceMessages.emplace_back("outputData", std::make_unique<FBDataPayload>("E_CTU", "Counter", 0, "FALSE"), 0);
  resourceMessages.emplace_back("outputData", std::make_unique<FBDataPayload>("E_CTU", "Counter", 1, "0"), 0);
  
  addInitiaOrFinalEvent(resourceMessages);

  auto ctfMessages = forte::trace::reader::utils::getEventMessages(CTF_OUTPUT_DIR).value();

  forte::tests::traces::checkMessages(expectedMessages, ctfMessages);
}

BOOST_AUTO_TEST_SUITE_END()

// ****************************** //
// * Helper Methods Definitions * //
// ****************************** //

namespace {

std::unique_ptr<CDevice> createExampleDevice(CStringDictionary::TStringId paResourceName, CStringDictionary::TStringId paDeviceName){
  auto device = std::make_unique<CTesterDevice>(paDeviceName);

  BOOST_TEST_INFO("Create Resource");
  BOOST_CHECK(EMGMResponse::Ready == device->createFB(paResourceName, STRID(EMB_RES)));

  BOOST_TEST_INFO("Start Device");
  BOOST_CHECK(device->initialize());
  
  auto resource = dynamic_cast<CResource*>(forte::trace::reader::utils::getFB(device.get(), paResourceName));

  auto startInstanceName = STRID(START);
  auto counterInstanceName = STRID(Counter);
  auto switchInstanceName = STRID(Switch);

  BOOST_TEST_INFO("Create E_CTU");
  BOOST_CHECK(EMGMResponse::Ready == resource->createFB(counterInstanceName, STRID(E_CTU)));

  BOOST_TEST_INFO("Create E_SWITCH");
  BOOST_CHECK(EMGMResponse::Ready == resource->createFB(switchInstanceName, STRID(E_SWITCH)));

  forte::core::SManagementCMD command;
  command.mCMD = EMGMCommandType::CreateConnection;
  command.mDestination = CStringDictionary::scmInvalidStringId;

  BOOST_TEST_INFO("Event connection: Start.COLD -> Counter.CU");
  command.mFirstParam.push_back(startInstanceName);
  command.mFirstParam.push_back(STRID(COLD));
  command.mSecondParam.push_back(counterInstanceName);
  command.mSecondParam.push_back(STRID(CU));

  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO("Event connection: Counter.CUO -> Switch.EI");
  command.mFirstParam.clear();
  command.mFirstParam.push_back(counterInstanceName);
  command.mFirstParam.push_back(STRID(CUO));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(switchInstanceName);
  command.mSecondParam.push_back(STRID(EI));
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO("Data connection: Counter.Q -> Switch.G ");
  command.mFirstParam.clear();
  command.mFirstParam.push_back(counterInstanceName);
  command.mFirstParam.push_back(STRID(Q));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(switchInstanceName);
  command.mSecondParam.push_back(STRID(G));
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO(" Data constant value: Counter.PV = 1");
  command.mFirstParam.clear();
  command.mFirstParam.push_back(counterInstanceName);
  command.mFirstParam.push_back(STRID(PV));
  BOOST_CHECK(EMGMResponse::Ready == resource->writeValue(command.mFirstParam, std::string("1"), false));

  BOOST_TEST_INFO("Event connection: Switch.EO1 -> Counter.R ");
  command.mFirstParam.clear();
  command.mFirstParam.push_back(switchInstanceName);
  command.mFirstParam.push_back(STRID(EO1));
  command.mSecondParam.clear();
  command.mSecondParam.push_back(counterInstanceName);
  command.mSecondParam.push_back(STRID(R));
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  return device;
}

}

