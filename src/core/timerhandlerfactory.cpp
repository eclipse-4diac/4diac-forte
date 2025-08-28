/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "timerhandlerfactory.h"

#include "core/util/mainparam_utils.h"

namespace {
  class TimerHandlerOption final
      : public forte::core::util::CommandLineParser::OptionImpl<"T", "timer", "<timer>", "Set the timer to be used"> {
    public:
      bool parseOption(const std::string_view paArgument) override {
        if (forte::core::TimerHandlerFactory::setDefaultImpl(forte::core::StringId::lookup(paArgument))) {
          return true;
        }
        printf("The selected timer '%s' is not valid. Select one of the following:\n", paArgument.data());
        for (const auto name : forte::core::TimerHandlerFactory::getNames()) {
          printf("  %s\n", name.data());
        }
        return false;
      }
  };

  [[maybe_unused]] TimerHandlerOption gTimerHandlerOption;
} // namespace
