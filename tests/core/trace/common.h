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

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../../core/src/trace/internal/EventMessage.h"

namespace forte::trace {
  namespace test {

    /**
     * @brief Cleanup the output directory of traces, copy the metadata file to it and setups the bareCTF tracer to it
     *
     * @param paDestMetadata metada destination file inside the CTF_OUTPUT_DIR
     */
    void prepareTraceTest(std::string paDestMetadata);

    /**
     * @brief Compares two maps of expected messages from resources to the actual ones
     *
     * @param paExpected expected messages
     * @param paActual actual messages
     */
    void checkMessages(std::unordered_map<std::string, std::vector<EventMessage>> &paExpected,
                       std::unordered_map<std::string, std::vector<EventMessage>> &paActual);

  } // namespace test

  /**
   * @brief Helper operator for BOOST_TEST to print
   *
   * @param paOs Stream to print to
   * @param paEventMessage Message to print
   * @return std::ostream& stream with the printed message
   */
  std::ostream &operator<<(std::ostream &paOs, const EventMessage &paEventMessage);
} // namespace forte::trace
