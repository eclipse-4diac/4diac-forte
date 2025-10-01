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

#include "deviceReplayer.h"
#include "forte/device.h"
#include "../../../core/src/ecetFake.h"

namespace forte::iec61499::system {
  CDeviceReplayer::CResourceInformation::CResourceInformation(
      CResource &paResource, const std::vector<trace::EventMessage> &paExternalEvents) :
      mResourceReplayer(paResource, paExternalEvents),
      mResource{paResource},
      mEcet{*dynamic_cast<CFakeEventExecutionThread *>(mResource.getResourceEventExecution())} {
  }

  CDeviceReplayer::CDeviceReplayer(
      CDevice &paDevice, const std::unordered_map<std::string, std::vector<trace::EventMessage>> &paExternalEvents) :
      mDevice{paDevice} {
    for (auto child : mDevice.getChildren()) {
      auto resource =
          static_cast<CResource *>(child); // the first generation of children under the device are always resources
      if (std::string resourceStringName = resource->getInstanceName();
          paExternalEvents.find(resourceStringName) != paExternalEvents.end()) {

        mResourceInformations.emplace_back(*resource, paExternalEvents.at(resourceStringName));
      }
    }
  }

  std::unordered_map<std::string, std::vector<trace::EventMessage>> CDeviceReplayer::reproduceAll() {

    std::unordered_map<std::string, std::vector<trace::EventMessage>> generatedMessages;

    for (auto &resourceInformation : mResourceInformations) {
      generatedMessages.insert(
          {resourceInformation.mResource.getInstanceName(), resourceInformation.mResourceReplayer.reproduceAll()});
    }

    return generatedMessages;
  }

  std::optional<TEventEntry> CDeviceReplayer::reproduceNextEvent(const std::string &paResourceName) {
    for (auto &resourceInformation : mResourceInformations) {
      if (resourceInformation.mResource.getInstanceName() == paResourceName) {
        return resourceInformation.mResourceReplayer.reproduceNextEvent();
      }
    }
    return std::nullopt;
  }

  std::unordered_map<std::string, std::vector<trace::EventMessage>> CDeviceReplayer::getGeneratedEvents() {
    std::unordered_map<std::string, std::vector<trace::EventMessage>> generatedMessages;
    for (auto &resourceInformation : mResourceInformations) {
      generatedMessages.insert({resourceInformation.mResource.getInstanceName(),
                                resourceInformation.mResourceReplayer.getGeneratedEvents()});
    }

    return generatedMessages;
  }
} // namespace forte::iec61499::system
