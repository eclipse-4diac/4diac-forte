/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/arch/forte_fileio.h"

#include <string>
#include <functional>
#include <optional>

namespace forte::iec61499::system {
  enum LoadBootResult {
    LOAD_RESULT_OK,
    MISSING_COLON,
    FILE_NOT_OPENED,
    EXTERNAL_ERROR,
  };

  class ForteBootFileLoader {
    public:
      using BootFileCallback = std::function<bool(const char *, char *)>;

      /**
       * Constructor which uses the the default values for the boot file location
       * @param paCallback Object to be called for each command
       */
      explicit ForteBootFileLoader(BootFileCallback paCallback,
                                   const std::optional<std::string> &paPathToFile = std::nullopt);

      ~ForteBootFileLoader();

      [[nodiscard]] LoadBootResult loadBootFile() const;

      [[nodiscard]] bool isOpen() const {
        return (nullptr != mBootfile);
      }

      [[nodiscard]] bool needsExit() const {
        return mNeedsExit;
      }

    private:
      decltype(forte_fopen(nullptr, nullptr)) mBootfile{nullptr};
      BootFileCallback mCallback; // for now with one callback is enough for all cases
      bool mNeedsExit{false};

      bool openBootFile(const std::optional<std::string> &paPathToFile);
      bool readLine(std::string &line) const;
      static bool hasCommandEnded(const std::string &line);
  };
} // namespace forte::iec61499::system
