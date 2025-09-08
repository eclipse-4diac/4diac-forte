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

#pragma once

#include "../../../../core/src/trace/internal/EventMessage.h"
#include "forte/stringid.h"
#include "resourceReplayer.h"

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class CFakeEventExecutionThread;
class CFunctionBlock;
class CDevice;
class CResource;

namespace forte::core {
  class CFBContainer;
}

/**
 * @brief Algorithm to generate the full series of event in a device from the subset of output events of Service
 * Function Blocks
 *
 */
class CDeviceReplayer {
  public:
    /**
     * @brief Constructor
     *
     * @param paCreateDevice a function that creates the device for which the generation of events must be done
     * @param paExternalEvents list of output events of Service Function Blocks separated by resource name
     */
    CDeviceReplayer(CDevice &paDevice,
                    const std::unordered_map<std::string, std::vector<EventMessage>> &paExternalEvents);

    ~CDeviceReplayer() = default;

    /**
     * @brief Executes the algorithm tha generates the full set of events of the device
     *
     * @return list of full events of the device separated by resource name
     */
    std::unordered_map<std::string, std::vector<EventMessage>> reproduceAll();

    /**
     * @brief Reproduce the next event
     *
     * @param paResourceName name of the resource where to reproduce the next event
     *
     * @return the event that was executed, std::nullopt if nothing was executed
     */
    std::optional<TEventEntry> reproduceNextEvent(const std::string &paResourceName);

    /**
     * @brief Get all generated events in the device
     *
     * @return list of full events of the device separated by resource name
     */
    std::unordered_map<std::string, std::vector<EventMessage>> getGeneratedEvents();

  private:
    CDevice &mDevice;

    /**
     * @brief Helper class that contains all needed information of a resource by the algorithm
     *
     */
    class CResourceInformation {
      public:
        CResourceInformation(CResource &paResource, const std::vector<EventMessage> &paEvents);

        CResourceReplayer mResourceReplayer;
        CResource &mResource;
        CFakeEventExecutionThread &mEcet;
    };

    std::vector<CResourceInformation> mResourceInformations;
};
