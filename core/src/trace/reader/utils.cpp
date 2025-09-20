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

#include "utils.h"
#include "forte/fbcontainer.h"
#include "forte/cfb.h"
#include "forte/basicfb.h"
#include "forte/util/parameterParser.h"
#include "forte/device.h"
#include "forte/util/devlog.h"
#include "forte/timerhandlerfactory.h"
#include "forte/ecetfactory.h"

#include <babeltrace2/babeltrace.h>

#include <functional>
#include <iostream>

namespace forte::trace::reader::utils {

  CFunctionBlock *getFB(CFBContainer *paContainer, const std::string &paFunctionBlockName) {
    if (paContainer == nullptr) {
      return nullptr;
    }

    TNameIdentifier id;

    // copy from OPCUA_MGR with some modifications
    std::string fbNamePart(paFunctionBlockName);
    size_t index = fbNamePart.find_first_of(".");
    while (index != std::string::npos) {
      id.push_back(StringId::insert(fbNamePart.substr(0, index)));
      fbNamePart = fbNamePart.substr(index + 1);
      index = fbNamePart.find_first_of(".");
    }
    id.push_back(StringId::insert(fbNamePart.substr(0, index)));

    auto it = id.cbegin();
    return paContainer->getFB(it, id.cend());
  }

  CFunctionBlock *getFB(CFBContainer *paContainer, StringId paFunctionBlockName) {
    if (paContainer == nullptr) {
      return nullptr;
    }
    TNameIdentifier id;
    id.push_back(paFunctionBlockName);
    auto it = id.cbegin();
    return paContainer->getFB(it, id.cend());
  }

  void setFactoriesSettings(FactoriesSettings paFactoriesSettings) {
    EcetFactory::setDefaultImpl(paFactoriesSettings.mEcet);
    TimerHandlerFactory::setDefaultImpl(paFactoriesSettings.mTimer);
    CFlexibleTracer::setTracer(paFactoriesSettings.mTracer);
  }

  std::set<StringId> getServiceFunctionBlockTypes(CFBContainer &paContainer) {

    std::set<StringId> result;

    // Get a list of all types that are not service FB (either Composite or Basic)
    std::function<void(CFBContainer *)> iterateContainers;

    iterateContainers = [&iterateContainers, &result](CFBContainer *paSubcontainer) {
      for (const auto child : paSubcontainer->getChildren()) {
        if (child == nullptr) {
          continue;
        }
        if (child->isDynamicContainer()) {
          iterateContainers(child);
          continue;
        }
        if (dynamic_cast<CCompositeFB *>(child) == nullptr && // no composite
            dynamic_cast<CBasicFB *>(child) == nullptr && // neither basic
            dynamic_cast<CFunctionBlock *>(child) != nullptr) { // neither just containers
          result.insert(dynamic_cast<CFunctionBlock *>(child)->getFBTypeId());
        }
      }
    };

    iterateContainers(&paContainer);

    return result;
  }

  std::optional<std::unordered_map<std::string, std::vector<EventMessage>>> getEventMessages(std::string path) {

    // create graph
    auto graph = bt_graph_create(0);

    // Source file Component
    const bt_plugin *ctfPlugin;
    if (BT_PLUGIN_FIND_STATUS_OK != bt_plugin_find("ctf", BT_FALSE, BT_FALSE, BT_TRUE, BT_FALSE, BT_TRUE, &ctfPlugin)) {
      DEVLOG_ERROR("Could not load ctf plugin\n");
      return std::nullopt;
    }
    auto fileSourceClass = bt_plugin_borrow_source_component_class_by_name_const(ctfPlugin, "fs");

    // create parameters for the source file component
    const bt_component_source *tracesComponent;
    auto parameters = bt_value_map_create();
    bt_value *dirsArray;

    if (BT_VALUE_MAP_INSERT_ENTRY_STATUS_OK !=
        bt_value_map_insert_empty_array_entry(parameters, "inputs", &dirsArray)) {
      DEVLOG_ERROR("Could not add empty array to map parameter for ctf.source.fs component\n");
      return std::nullopt;
    }

    if (BT_VALUE_ARRAY_APPEND_ELEMENT_STATUS_OK != bt_value_array_append_string_element(dirsArray, path.c_str())) {
      DEVLOG_ERROR("Could not add input folder to ctf.source.fs component's input parameter\n");
      return std::nullopt;
    }

    if (BT_GRAPH_ADD_COMPONENT_STATUS_OK != bt_graph_add_source_component(graph, fileSourceClass, "traces", parameters,
                                                                          BT_LOGGING_LEVEL_TRACE, &tracesComponent)) {
      DEVLOG_ERROR("Could not create Source component\n");
      return std::nullopt;
    }

    // Forte event reader component

    const bt_plugin *fortePlugin;
    if (BT_PLUGIN_FIND_STATUS_OK !=
        bt_plugin_find("forte", BT_FALSE, BT_FALSE, BT_FALSE, BT_TRUE, BT_TRUE, &fortePlugin)) {
      DEVLOG_ERROR("Could not load forte plugin\n");
      return std::nullopt;
    }
    auto forteReaderClass = bt_plugin_borrow_sink_component_class_by_name_const(fortePlugin, "event_reader");

    std::unordered_map<std::string, std::vector<EventMessage>> messages;

    // create a sink forte event reader component for each resource
    for (uint64_t i = 0; i < bt_component_source_get_output_port_count(tracesComponent); i++) {
      auto port = bt_component_source_borrow_output_port_by_index_const(tracesComponent, i);
      auto resourceName = getResourceNameFromTraceOutputPort(port);
      messages.insert({resourceName, {}});

      const bt_component_sink *forteReaderComponent;
      auto componentName = std::string("forteReader_") + std::to_string(i);

      if (BT_GRAPH_ADD_COMPONENT_STATUS_OK !=
          bt_graph_add_sink_component_with_initialize_method_data(graph, forteReaderClass, componentName.c_str(),
                                                                  nullptr, &messages[resourceName],
                                                                  BT_LOGGING_LEVEL_TRACE, &forteReaderComponent)) {
        DEVLOG_ERROR("Could not create forte event reader component number %d\n", i);
        return std::nullopt;
      }

      if (BT_GRAPH_CONNECT_PORTS_STATUS_OK !=
          bt_graph_connect_ports(graph, bt_component_source_borrow_output_port_by_index_const(tracesComponent, i),
                                 bt_component_sink_borrow_input_port_by_index_const(forteReaderComponent, 0),
                                 nullptr)) {
        DEVLOG_ERROR("Could not add connection %d from source to forte\n", i);
        return std::nullopt;
      }
    }

    if (BT_GRAPH_RUN_STATUS_OK != bt_graph_run(graph)) {
      DEVLOG_ERROR("Could not run graph\n");
      return std::nullopt;
    }

    return messages;
  }

  std::string getResourceNameFromTraceOutputPort(const bt_port_output *paPort) {
    auto outputPortBase = bt_port_output_as_port_const(paPort);
    auto portName = bt_port_get_name(outputPortBase);

    // Port name has the following pattern: TRACE-ID | STREAM-CLASS-ID | STREAM-ID
    // where STREAM-ID contains the absolut path to the trace file.
    // The file name is given in the CTF tracer inside forte as
    // "trace_"  + INSTANCE_NAME + "_" + DATE_TIME + ".ctf"),

    util::CParameterParser portNameParser(portName, '|', 3);
    portNameParser.parseParameters();
    util::CParameterParser fileNameParser(std::filesystem::path(portNameParser[2]).filename().c_str(), '_', 4);
    auto totalParameters = fileNameParser.parseParameters();

    std::string result = fileNameParser[1];

    // in case the instance name has underscore in it
    for (std::size_t i = 2; i < totalParameters - 2; i++) {
      result += std::string("_") + fileNameParser[i];
    }

    return result;
  }

  std::unordered_map<std::string, std::vector<EventMessage>>
  filterEvents(const std::unordered_map<std::string, std::vector<EventMessage>> &paEvents,
               std::function<bool(const EventMessage &)> paFilterIn) {

    std::unordered_map<std::string, std::vector<EventMessage>> result;

    for (const auto &[resourceName, messages] : paEvents) {
      result.insert({resourceName, {}});
      auto &resultMessages = result[resourceName];

      for (auto &message : messages) {
        if (paFilterIn(message)) {
          resultMessages.push_back(message);
        }
      }
    }

    return result;
  }

  std::unordered_map<std::string, std::vector<EventMessage>>
  filterEventsForReplayDevice(const std::unordered_map<std::string, std::vector<EventMessage>> &paEvents,
                              CDevice &paDevice) {

    // function to filter events which are interesting for the replay algorithm, i.e. output events from service FBs
    auto isValidType = [validTypes = getServiceFunctionBlockTypes(paDevice)](const EventMessage &paMessage) {
      if (paMessage.getEventType() != "sendOutputEvent") {
        return false;
      }
      auto type = StringId::lookup(paMessage.getPayload<AbstractPayload>()->getTypeName());
      return validTypes.find(type) != validTypes.end();
    };

    return filterEvents(paEvents, isValidType);
  }

} // namespace forte::trace::reader::utils
