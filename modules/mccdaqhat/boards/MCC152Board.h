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

namespace forte::eclipse4diac::io::mccdaqhat {

  class MCC152Board final : public MCCBoard {
    public:
      MCC152Board(IMCCDaqHatApi &paApi, uint8_t paAddress);
      ~MCC152Board() override;

      HatType getType() const override {
        return HatType::MCC152;
      }

      //! Opens and configures the board; returns an empty string on success, otherwise the STATUS text
      std::string open(uint8_t paDirectionMask, uint8_t paPullEnable, uint8_t paPullConfig);

      void close() override;

      bool getInputBit(uint8_t paChannel);
      int writeOutputBit(uint8_t paChannel, bool paValue);
      int writeAnalogCode(uint8_t paChannel, uint16_t paCode);

    protected:
      int readInputs() override;

    private:
      uint8_t mInputPort = 0;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
