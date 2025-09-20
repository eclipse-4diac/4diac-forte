/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/stringid.h"

#include "forte/ecetfactory.h"
#include "forte/timerhandlerfactory.h"
#include "../internal/flexibleTracer.h"

#include <string>
#include <set>
#include <unordered_map>
#include <functional>
#include <optional>

using namespace forte::literals;

class bt_port_output;

namespace forte {
  class CFunctionBlock;
  class CDevice;
  class CFBContainer;

  namespace trace::reader::utils {

    /**
     * @brief  get the function block instance out of the container. This is just a shorthand for creating the needed
     * parameters for the CFBContainer::getFB
     *
     * @param paContainer Device where the to look for the resource
     * @param paFunctionBlockName name of the function block
     * @return a pointer to the resource with the provided name, nullptr if a resource with the provided name does not
     * exist
     */
    CFunctionBlock *getFB(CFBContainer *paContainer, const std::string &paFunctionBlockName);

    /**
     * @brief  Same as the previous function, but using other type of input parameter
     *
     * @param paContainer Device where the to look for the resource
     * @param paFunctionBlockName name of the function block
     * @return a pointer to the resource with the provided name, nullptr if a resource with the provided name does not
     * exist
     */
    CFunctionBlock *getFB(CFBContainer *paContainer, StringId paFunctionBlockName);

    struct FactoriesSettings {
        StringId mEcet{"default"_STRID};
        StringId mTimer{"default"_STRID};
        CFlexibleTracer::AvailableTracers mTracer{CFlexibleTracer::AvailableTracers::BareCtf};
    };

    /**
     * @brief Set the desired factories settings
     *
     * @param paFactoriesSettings settings to be set
     */
    void setFactoriesSettings(FactoriesSettings paFactoriesSettings);

    /**
     * @brief Get the list of service function block types
     *
     * @param paContainer container where to look for the types
     * @return list of of service function block types
     */
    std::set<StringId> getServiceFunctionBlockTypes(CFBContainer &paContainer);

    /**
     * @brief Get the list of message from a directory containing CTF traces separated by resource
     *
     * @param path directory containing ctf traces
     * @return events mapped to the resource they belong to if there was no issue, nullopt otherwise
     */
    std::optional<std::unordered_map<std::string, std::vector<EventMessage>>> getEventMessages(std::string path);

    /**
     * @brief Get the resource name from an output port of the source port of a file source component
     *
     * When creating a source file component, the output ports are based on the files which are based on each resource
     *
     * @param paPort the port where to read the name from
     * @return the name of the resource
     */
    std::string getResourceNameFromTraceOutputPort(const bt_port_output *paPort);

    /**
     * @brief Filter a list of events based on a given function
     *
     * @param paEvents list of events to be filtered, separated by a string key (usually resource name)
     * @param paFilterIn function to check if the event should be kept
     * @return list of filtered events
     */
    std::unordered_map<std::string, std::vector<EventMessage>>
    filterEvents(const std::unordered_map<std::string, std::vector<EventMessage>> &paEvents,
                 std::function<bool(const EventMessage &)> paFilterIn);

    /**
     * @brief Filter a list of events returning only the interesting ones needed for the replay device algorithm
     *
     * @param paEvents list of events to be filtered, separated by a string key (usually resource name)
     * @param paDevice device which is being replayed
     * @return list of filtered events
     */
    std::unordered_map<std::string, std::vector<EventMessage>>
    filterEventsForReplayDevice(const std::unordered_map<std::string, std::vector<EventMessage>> &paEvents,
                                CDevice &paDevice);

  } // namespace trace::reader::utils
} // namespace forte
