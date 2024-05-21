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

BOOST_AUTO_TEST_SUITE (ctfTracer_test)

BOOST_AUTO_TEST_CASE(library_test) {

  // load ctf plugin
  const bt_plugin* ctfPlugin;
  BOOST_TEST(BT_PLUGIN_FIND_STATUS_OK == bt_plugin_find("ctf", BT_FALSE, BT_FALSE, BT_TRUE, BT_FALSE, BT_TRUE, &ctfPlugin));
  auto fileSourceClass = bt_plugin_borrow_source_component_class_by_name_const(ctfPlugin, "fs"); 

  // load utils plugin
  const bt_plugin* utilsPlugin;
  BOOST_TEST(BT_PLUGIN_FIND_STATUS_OK == bt_plugin_find("utils", BT_FALSE, BT_FALSE, BT_TRUE, BT_FALSE, BT_TRUE, &utilsPlugin));

  auto muxerFilterClass = bt_plugin_borrow_filter_component_class_by_name_const(utilsPlugin, "muxer"); 

  // look on static plugins only
  const bt_plugin* fortePlugin;
  BOOST_TEST(BT_PLUGIN_FIND_STATUS_OK == bt_plugin_find("forte", BT_FALSE, BT_FALSE, BT_FALSE, BT_TRUE, BT_TRUE, &fortePlugin));
  auto forteReaderClass = bt_plugin_borrow_sink_component_class_by_name_const(fortePlugin, "event_reader"); 

  // create graph
  auto graph = bt_graph_create(0);
  
  // Source component
  // create parameters to file source component
  const bt_component_source*  tracesComponent;
  auto parameters = bt_value_map_create();
  bt_value *dirsArray;

  BOOST_TEST(BT_VALUE_MAP_INSERT_ENTRY_STATUS_OK == bt_value_map_insert_empty_array_entry(parameters, "inputs", &dirsArray));

  BOOST_TEST(BT_VALUE_ARRAY_APPEND_ELEMENT_STATUS_OK ==
     	bt_value_array_append_string_element(dirsArray, "/home/cochicde/ctf"));

  // create Source component
  BOOST_TEST(BT_GRAPH_ADD_COMPONENT_STATUS_OK == 
    bt_graph_add_source_component(graph, fileSourceClass, "traces", parameters, BT_LOGGING_LEVEL_TRACE, &tracesComponent));
  
  // create muxer component
  const bt_component_filter*  muxerComponent;
  BOOST_TEST(BT_GRAPH_ADD_COMPONENT_STATUS_OK == 
    bt_graph_add_filter_component(graph, muxerFilterClass, "muxer", nullptr, BT_LOGGING_LEVEL_TRACE, &muxerComponent));

  // create pretty component
  std::vector<EventMessage> messages;
  const bt_component_sink*  forteReaderComponent;
  BOOST_TEST(BT_GRAPH_ADD_COMPONENT_STATUS_OK == 
   bt_graph_add_sink_component_with_initialize_method_data(graph, forteReaderClass, "forteReader", nullptr, &messages, BT_LOGGING_LEVEL_TRACE, &forteReaderComponent));

  // Connections
  for(uint64_t i = 0; i < bt_component_source_get_output_port_count(tracesComponent); i++){
    BOOST_TEST(BT_GRAPH_CONNECT_PORTS_STATUS_OK ==  bt_graph_connect_ports(graph, 
        bt_component_source_borrow_output_port_by_index_const(tracesComponent, i), 
        bt_component_filter_borrow_input_port_by_index_const(muxerComponent, i), 
        nullptr));
  }    

  BOOST_TEST(BT_GRAPH_CONNECT_PORTS_STATUS_OK ==  bt_graph_connect_ports(graph, 
    bt_component_filter_borrow_output_port_by_index_const(muxerComponent, 0), 
    bt_component_sink_borrow_input_port_by_index_const(forteReaderComponent, 0), 
    nullptr));

  BOOST_TEST(BT_GRAPH_RUN_STATUS_OK == bt_graph_run(graph));	

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
}

BOOST_AUTO_TEST_SUITE_END()
