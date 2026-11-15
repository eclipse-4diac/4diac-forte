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

#include "mccdaqhat_config.h"

#include <cmath>
#include <cstddef>

namespace forte::eclipse4diac::io::mccdaqhat {

  uint16_t toCode(double paValue, uint16_t paMaxCode) {
    // The negated comparison also maps NaN to 0.
    if (!(paValue > 0.0)) {
      return 0;
    }
    const double rounded = std::round(paValue);
    if (rounded >= static_cast<double>(paMaxCode)) {
      return paMaxCode;
    }
    return static_cast<uint16_t>(rounded);
  }

  std::string validateAddress(unsigned int paAddress) {
    if (paAddress > scmMaxHatAddress) {
      return "Address " + std::to_string(paAddress) + " is out of range 0..7";
    }
    return {};
  }

  std::string validateMCC128Config(unsigned int paMode, unsigned int paRange, const TChannelUsage &paUsed) {
    if (paMode > scmMCC128ModeDifferential) {
      return "InputMode " + std::to_string(paMode) + " is invalid (0 = single-ended, 1 = differential)";
    }
    if (paRange > scmMCC128MaxRange) {
      return "InputRange " + std::to_string(paRange) + " is invalid (0..3)";
    }
    if (paMode == scmMCC128ModeDifferential) {
      for (size_t channel = scmMCC128DiffChannels; channel < scmMCC128Channels; ++channel) {
        if (paUsed[channel]) {
          return "AI_" + std::to_string(channel) + " is not available in differential mode";
        }
      }
    }
    return {};
  }

  std::string computeMCC152DirectionMask(const TChannelUsage &paInputs,
                                         const TChannelUsage &paOutputs,
                                         uint8_t &paMask) {
    paMask = 0xFF;
    for (size_t bit = 0; bit < scmMCC152DioChannels; ++bit) {
      if (paInputs[bit] && paOutputs[bit]) {
        const std::string number = std::to_string(bit);
        return "Bit " + number + " is mapped as DI_" + number + " and DO_" + number;
      }
      if (paOutputs[bit]) {
        paMask = static_cast<uint8_t>(paMask & ~(1U << bit));
      }
    }
    return {};
  }

  uint16_t clampMCC152AoCode(uint16_t paCode, bool &paClamped) {
    paClamped = paCode > scmMCC152MaxAoCode;
    return paClamped ? scmMCC152MaxAoCode : paCode;
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
