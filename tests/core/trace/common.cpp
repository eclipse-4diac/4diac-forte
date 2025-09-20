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

#include "common.h"

#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "config.h"
#include "../../../core/src/trace/barectf_platform_forte.h"

namespace forte::trace {
  namespace test {

    void prepareTraceTest(std::string paDestMetadata) {
      std::filesystem::path destMetadata(cgCTFOutputDir);

      // remove previous trace files
      std::filesystem::remove_all(destMetadata);
      std::filesystem::create_directory(destMetadata);

      std::filesystem::copy_file(cgMetadataFile, destMetadata / std::move(paDestMetadata));

      BarectfPlatformFORTE::setup(cgCTFOutputDir);
    }

    void checkMessages(std::unordered_map<std::string, std::vector<EventMessage>> &paExpected,
                       std::unordered_map<std::string, std::vector<EventMessage>> &paActual) {

      // check that they have the same amount of keys
      BOOST_CHECK(paExpected.size() == paActual.size());

      for (auto &[resource, expectedMessages] : paExpected) {
        BOOST_CHECK(paActual.find(resource) != paActual.end());

        auto &actualMessages = paActual[resource];

        BOOST_TEST_INFO("Resource: " + resource + " Expected vs traced: Same size ");
        BOOST_CHECK_EQUAL(expectedMessages.size(), actualMessages.size());

        // although vectors can be check directly, this granularity helps debugging in case some message is different
        for (size_t i = 0; i < std::min(expectedMessages.size(), actualMessages.size()); i++) {
          BOOST_TEST_INFO("Resource: " + resource + " Expected event number " + std::to_string(i));
          BOOST_CHECK_EQUAL(expectedMessages[i], actualMessages[i]);
        }

        // add extra event to check that the comparison fails
        expectedMessages.emplace_back(
            "sendOutputEvent", std::make_unique<FBInputEventPayload>("iec61499::events::E_RESTART", "START", 2), 0);
        BOOST_CHECK(expectedMessages != actualMessages);

        // remove the recently added message in case is needed again later
        expectedMessages.pop_back();
      }
    }

  } // namespace test

  std::ostream &operator<<(std::ostream &paOs, const EventMessage &paEventMessage) {
    paOs << paEventMessage.getPayloadString();
    return paOs;
  }
} // namespace forte::trace
