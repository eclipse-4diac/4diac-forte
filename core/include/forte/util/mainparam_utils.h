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

#pragma once

#include "forte/util/fixed_string.h"

#include <charconv>
#include <concepts>
#include <string_view>

namespace forte::util {
  class CommandLineParser {
    public:
      class Option {
        public:
          virtual ~Option() = default;

          virtual bool parseOption(std::string_view paArgument) = 0;

          [[nodiscard]] virtual std::string_view getShortName() const = 0;

          [[nodiscard]] virtual std::string_view getLongName() const = 0;

          [[nodiscard]] virtual std::string_view getArgumentName() const = 0;

          [[nodiscard]] virtual std::string_view getHelp() const = 0;

          friend bool operator==(const Option &paFirst, const Option &paSecond) {
            return paFirst.getShortName() == paSecond.getShortName() && paFirst.getLongName() == paSecond.getLongName();
          }

          friend auto operator<=>(const Option &paFirst, const Option &paSecond) {
            if (const auto result = paFirst.getShortName() <=> paSecond.getShortName(); result != 0) {
              return result;
            }
            return paFirst.getLongName() <=> paSecond.getLongName();
          }

        protected:
          Option() {
            registerOption(this);
          }
      };

      template<fixed_string ShortName, fixed_string LongName, fixed_string ArgumentName, fixed_string Help>
      class OptionImpl : public Option {
        public:
          ~OptionImpl() override = default;

          [[nodiscard]] std::string_view getShortName() const override {
            return ShortName;
          }

          [[nodiscard]] std::string_view getLongName() const override {
            return LongName;
          }

          [[nodiscard]] std::string_view getArgumentName() const override {
            return ArgumentName;
          }

          [[nodiscard]] std::string_view getHelp() const override {
            return Help;
          }

        protected:
          OptionImpl() = default;
      };

      template<std::integral T,
               fixed_string ShortName,
               fixed_string LongName,
               fixed_string ArgumentName,
               fixed_string Help>
      class IntOptionImpl : public OptionImpl<ShortName, LongName, ArgumentName, Help> {
          bool parseOption(const std::string_view paArgument) final {
            T result{};
            if (auto [ptr, ec] = std::from_chars(paArgument.data(), paArgument.data() + paArgument.size(), result);
                ptr != paArgument.data() + paArgument.size() || ec != std::errc()) {
              return false;
            }
            setOption(result);
            return true;
          }

          virtual bool setOption(T t) = 0;
      };

      CommandLineParser() = delete;

      /*!\brief Lists the help for FORTE
       *
       */
      static void listHelp();

      /*!\brief Parses the command line arguments passed to the main function
       *
       */
      static std::string parseCommandLineArguments(int argc, char *arg[]);

    private:
      static void registerOption(Option *paOption);
  };
} // namespace forte::util
