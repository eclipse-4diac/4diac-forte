/*******************************************************************************
 * Copyright (c) 2019 fotiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include <babeltrace2/babeltrace.h>

#include "EventMessage.h"

#include <optional>
#include <iomanip>

#include <thread>

#include "trace/barectf_platform_forte.h"
#include "../stdfblib/ita/EMB_RES.h"
#include "ecet.h"
#include "ctfTracerTest_gen.cpp"
#include "device.h"
#include "resource.h"




BOOST_AUTO_TEST_SUITE (tracer_test)

std::vector<EventMessage> getEventMessages(const std::string& path);


BOOST_AUTO_TEST_CASE(sequential_events_test) {

  const std::string traceOutputPath = "/home/cochicde/ctf";

  BarectfPlatformFORTE::setup(traceOutputPath);

const  SFBInterfaceSpec scTestDevSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};
  CDevice device(&scTestDevSpec, g_nStringIdMyDevice);
  auto resource = new EMB_RES(g_nStringIdMyResource, device);

  resource->initialize();

  auto startInstanceName = g_nStringIdSTART;
  auto counterInstanceName = g_nStringIdCounter;
  auto switchInstanceName = g_nStringIdSwitch;

  resource->addFB(CTypeLib::createFB(counterInstanceName, g_nStringIdE_CTU, *resource));
  resource->addFB(CTypeLib::createFB(switchInstanceName, g_nStringIdE_SWITCH, *resource));

  forte::core::SManagementCMD command;
  command.mCMD = EMGMCommandType::CreateConnection;
  command.mDestination = CStringDictionary::scmInvalidStringId;

  BOOST_TEST_INFO("Event connection: Start.COLD -> Counter.CU");
  command.mFirstParam.pushBack(startInstanceName);
  command.mFirstParam.pushBack(g_nStringIdCOLD);
  command.mSecondParam.pushBack(counterInstanceName);
  command.mSecondParam.pushBack(g_nStringIdCU);

  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO("Event connection: Counter.CUO -> Switch.EI");
  command.mFirstParam.clear();
  command.mFirstParam.pushBack(counterInstanceName);
  command.mFirstParam.pushBack(g_nStringIdCUO);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(switchInstanceName);
  command.mSecondParam.pushBack(g_nStringIdEI);
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO("Data connection: Counter.Q -> Switch.G ");
  command.mFirstParam.clear();
  command.mFirstParam.pushBack(counterInstanceName);
  command.mFirstParam.pushBack(g_nStringIdQ);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(switchInstanceName);
  command.mSecondParam.pushBack(g_nStringIdG);
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  BOOST_TEST_INFO(" Data constant value: Counter.PV = 1");
  command.mFirstParam.clear();
  command.mFirstParam.pushBack(counterInstanceName);
  command.mFirstParam.pushBack(g_nStringIdPV);
  BOOST_CHECK(EMGMResponse::Ready == resource->writeValue(command.mFirstParam, CIEC_STRING(std::string("1")), false));

  BOOST_TEST_INFO("Event connection: Switch.EO1 -> Counter.R ");
  command.mFirstParam.clear();
  command.mFirstParam.pushBack(switchInstanceName);
  command.mFirstParam.pushBack(g_nStringIdEO1);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(counterInstanceName);
  command.mSecondParam.pushBack(g_nStringIdR);
  BOOST_CHECK(EMGMResponse::Ready == resource->executeMGMCommand(command));

  device.addFB(resource);


  device.startDevice();
  while(resource->getResourceEventExecution()->isProcessingEvents()){
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  device.changeFBExecutionState(EMGMCommandType::Stop);
  resource->getResourceEventExecution()->joinEventChainExecutionThread();

  auto messages = getEventMessages(traceOutputPath);

  auto getTimestampDifference = [](int64_t current, std::optional<int64_t> prev) -> std::string {
    if(prev == std::nullopt){
      return "(+?.\?\?\?\?\?\?\?\?\?)";
    }

    auto diff = current - prev.value();

    auto seconds = diff / 1000000000;
    diff %= 1000000000;


    std::stringstream ss;
    ss << "(" << (diff < 0 ? "-" : "+") << 
      std::setw(1) << std::setfill('0') << seconds << 
      "." << 
      std::setw(9) << std::setfill('0') << diff  << ")"; 
    
    return ss.str();
  };

  std::optional<int64_t> lastTimestamp = std::nullopt;
  for(const auto& message : messages) {
    std::cout << message.getTimestampString() 
      << " " << getTimestampDifference(message.getTimestamp(), lastTimestamp) << " "
      << message.getPayloadString() << std::endl;
    lastTimestamp = message.getTimestamp();
  }

  // disable logging 
  //BarectfPlatformFORTE::setup("");
}

std::vector<EventMessage> getEventMessages(const std::string& path){
  
  BOOST_TEST_INFO("Load ctf plugin");
  const bt_plugin* ctfPlugin;
  BOOST_CHECK_EQUAL(BT_PLUGIN_FIND_STATUS_OK, bt_plugin_find("ctf", BT_FALSE, BT_FALSE, BT_TRUE, BT_FALSE, BT_TRUE, &ctfPlugin));
  auto fileSourceClass = bt_plugin_borrow_source_component_class_by_name_const(ctfPlugin, "fs"); 

  BOOST_TEST_INFO("Load utils plugin");
  const bt_plugin* utilsPlugin;
  BOOST_CHECK_EQUAL(BT_PLUGIN_FIND_STATUS_OK, bt_plugin_find("utils", BT_FALSE, BT_FALSE, BT_TRUE, BT_FALSE, BT_TRUE, &utilsPlugin));
  auto muxerFilterClass = bt_plugin_borrow_filter_component_class_by_name_const(utilsPlugin, "muxer"); 

  BOOST_TEST_INFO("Load forte plugin");
  const bt_plugin* fortePlugin;
  BOOST_CHECK_EQUAL(BT_PLUGIN_FIND_STATUS_OK, bt_plugin_find("forte", BT_FALSE, BT_FALSE, BT_FALSE, BT_TRUE, BT_TRUE, &fortePlugin));
  auto forteReaderClass = bt_plugin_borrow_sink_component_class_by_name_const(fortePlugin, "event_reader"); 

  // create graph
  auto graph = bt_graph_create(0);
  
  // Source component
  // create parameters to file source component
  const bt_component_source*  tracesComponent;
  auto parameters = bt_value_map_create();
  bt_value *dirsArray;

  BOOST_TEST_INFO("Add empty array to map parameter for ctf.source.fs component");
  BOOST_CHECK_EQUAL(BT_VALUE_MAP_INSERT_ENTRY_STATUS_OK, bt_value_map_insert_empty_array_entry(parameters, "inputs", &dirsArray));

  BOOST_TEST_INFO("Add input folder to ctf.source.fs component's input parameter");
  BOOST_CHECK_EQUAL(BT_VALUE_ARRAY_APPEND_ELEMENT_STATUS_OK, bt_value_array_append_string_element(dirsArray, path.c_str()));

  BOOST_TEST_INFO("Create Source component");
  BOOST_CHECK_EQUAL(BT_GRAPH_ADD_COMPONENT_STATUS_OK, bt_graph_add_source_component(graph, fileSourceClass, "traces", parameters, BT_LOGGING_LEVEL_TRACE, &tracesComponent));
  
  const bt_component_filter*  muxerComponent;
  BOOST_CHECK_EQUAL(BT_GRAPH_ADD_COMPONENT_STATUS_OK, bt_graph_add_filter_component(graph, muxerFilterClass, "muxer", nullptr, BT_LOGGING_LEVEL_TRACE, &muxerComponent));

  BOOST_TEST_INFO("Create forte event reader component");
  std::vector<EventMessage> messages;
  const bt_component_sink*  forteReaderComponent;
  BOOST_CHECK_EQUAL(BT_GRAPH_ADD_COMPONENT_STATUS_OK, 
   bt_graph_add_sink_component_with_initialize_method_data(graph, forteReaderClass, "forteReader", nullptr, &messages, BT_LOGGING_LEVEL_TRACE, &forteReaderComponent));

  for(uint64_t i = 0; i < bt_component_source_get_output_port_count(tracesComponent); i++){
    BOOST_TEST_INFO("Add connection " << i << " from source to muxer");
    BOOST_CHECK_EQUAL(BT_GRAPH_CONNECT_PORTS_STATUS_OK,  bt_graph_connect_ports(graph, 
        bt_component_source_borrow_output_port_by_index_const(tracesComponent, i), 
        bt_component_filter_borrow_input_port_by_index_const(muxerComponent, i), 
        nullptr));
  }    

  BOOST_TEST_INFO("Add connection from muxer to forte reader");
  BOOST_CHECK_EQUAL(BT_GRAPH_CONNECT_PORTS_STATUS_OK,  bt_graph_connect_ports(graph, 
    bt_component_filter_borrow_output_port_by_index_const(muxerComponent, 0), 
    bt_component_sink_borrow_input_port_by_index_const(forteReaderComponent, 0), 
    nullptr));

  BOOST_TEST_INFO("Run graph");
  BOOST_CHECK_EQUAL(BT_GRAPH_RUN_STATUS_OK, bt_graph_run(graph));

  return messages;	
}

BOOST_AUTO_TEST_SUITE_END()
