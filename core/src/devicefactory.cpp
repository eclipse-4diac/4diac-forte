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

#include "forte/devicefactory.h"

#include "forte/util/mainparam_utils.h"

namespace {
  class DeviceOption final : public forte::core::util::CommandLineParser::
                                 OptionImpl<"d", "device", "<device>", "Set the device to be used"> {
    public:
      bool parseOption(const std::string_view paArgument) override {
        if (forte::core::DeviceFactory::setDefaultImpl(forte::core::StringId::lookup(paArgument))) {
          return true;
        }
        printf("The selected device '%s' is not valid. Select one of the following:\n", paArgument.data());
        for (const auto name : forte::core::DeviceFactory::getNames()) {
          printf("  %s\n", name.data());
        }
        return false;
      }
  };

  [[maybe_unused]] DeviceOption gDeviceOption;
} // namespace
