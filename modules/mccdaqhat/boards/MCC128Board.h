/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#pragma once

#include "MCCBoard.h"
#include "../mccdaqhat_config.h"

#include <array>

namespace forte::eclipse4diac::io::mccdaqhat {

  class MCC128Board final : public MCCBoard {
    public:
      MCC128Board(IMCCDaqHatApi &paApi, uint8_t paAddress);
      ~MCC128Board() override;

      HatType getType() const override {
        return HatType::MCC128;
      }

      //! Opens and configures the board; returns an empty string on success, otherwise the STATUS text
      std::string open(uint8_t paMode, uint8_t paRange, const TChannelUsage &paUsed);

      void close() override;

      uint16_t getCode(uint8_t paChannel);

    protected:
      int readInputs() override;

    private:
      TChannelUsage mUsed{};
      std::array<uint16_t, scmMCC128Channels> mCodes{};
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
