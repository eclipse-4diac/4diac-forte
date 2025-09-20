/*******************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Tarik Terzimehic
 *    - initial API and implementation and/or initial documentation
 *   Martin Erich Jobst
 *    - rework to use self-registration of options
 *******************************************************************************/

#include "forte/util/mainparam_utils.h"

#include "forte/devicefactory.h"
#include "forte/timerhandlerfactory.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <format>

using namespace std::string_literals;

namespace forte::util {
  namespace {
    constinit std::vector<CommandLineParser::Option *> options;

    CommandLineParser::Option *findOption(std::string_view paName) {
      if (paName.starts_with("--")) {
        paName = paName.substr(2);
        for (const auto option : options) {
          if (option->getLongName() == paName) {
            return option;
          }
        }
      } else if (paName.starts_with('-')) {
        paName = paName.substr(1);
        for (const auto option : options) {
          if (option->getShortName() == paName) {
            return option;
          }
        }
      }
      return nullptr;
    }

    void printOption(const std::string_view paShortName,
                     const std::string_view paLongName,
                     const std::string_view paArgumentName,
                     const std::string_view paHelp,
                     std::size_t maxShortNameLength,
                     std::size_t maxLongNameLength) {
      std::cout << std::format("  -{:{}} --{:{}} {} {}", std::string(paShortName) + ",", maxShortNameLength + 1,
                               paLongName, maxLongNameLength, paArgumentName, paHelp)
                << std::endl;
    }
  } // namespace

  void CommandLineParser::registerOption(Option *paOption) {
    options.insert(std::ranges::upper_bound(options, paOption), paOption);
  }

  void CommandLineParser::listHelp() {
    std::size_t maxShortNameLength = std::ranges::max(options | std::views::transform(&Option::getShortName) |
                                                      std::views::transform(&std::string_view::size));
    std::size_t maxLongNameLength = std::ranges::max(options | std::views::transform(&Option::getLongName) |
                                                     std::views::transform(&std::string_view::size));
    std::cout << "Usage: forte [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    printOption("h", "help", "", "Display this information", maxShortNameLength, maxLongNameLength);
    printOption("c", "listen", "<IP>:<port>", "Set the listening IP and port for the incoming connections",
                maxShortNameLength, maxLongNameLength);
    for (const auto option : options) {
      printOption(option->getShortName(), option->getLongName(), option->getArgumentName(), option->getHelp(),
                  maxShortNameLength, maxLongNameLength);
    }
  }

  std::string CommandLineParser::parseCommandLineArguments(const int argc, char *arg[]) {
    std::string result = "localhost:61499"; //! Default Value (localhost:61499)
    for (std::size_t i = 1; i < static_cast<std::size_t>(argc); ++i) {
      std::string_view optionName(arg[i]);
      std::string_view argument;
      if (const std::size_t equalsIndex = optionName.find('='); equalsIndex != std::string_view::npos) {
        argument = optionName.substr(equalsIndex + 1);
        optionName = optionName.substr(0, equalsIndex);
      } else if (i < static_cast<std::size_t>(argc) - 1) {
        argument = arg[i + 1];
        ++i;
      }
      if (optionName == "-c" || optionName == "--listen") {
        result = std::string(argument);
      } else if (Option *option = findOption(optionName); !option || !option->parseOption(argument)) {
        return "";
      }
    }
    return result;
  }
} // namespace forte::util
