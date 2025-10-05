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

#include "forte/util/devlog.h"
#include "forte/util/mainparam_utils.h"

#include <windows.h>

namespace {
  class LoadOption final : public forte::util::CommandLineParser::
                               OptionImpl<"l", "load", "<module>", "Load a module from a shared library"> {
    public:
      bool parseOption(const std::string_view paArgument) override {
        if (LoadLibraryA(paArgument.data())) {
          DEVLOG_INFO("Loaded module '%s'\n", paArgument.data());
          return true;
        }
        DEVLOG_ERROR("Could not load module '%s'\n", paArgument.data());
        return false;
      }
  };

  [[maybe_unused]] LoadOption gLoadOption;
} // namespace
