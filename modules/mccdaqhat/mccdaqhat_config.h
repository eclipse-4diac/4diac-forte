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

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>

namespace forte::eclipse4diac::io::mccdaqhat {

  constexpr unsigned int scmMaxHatAddress = 7;
  constexpr size_t scmMCC128Channels = 8;
  constexpr size_t scmMCC128DiffChannels = 4;
  constexpr unsigned int scmMCC128ModeSingleEnded = 0;
  constexpr unsigned int scmMCC128ModeDifferential = 1;
  constexpr unsigned int scmMCC128MaxRange = 3;
  constexpr uint16_t scmMCC128MaxCode = 65535;
  constexpr size_t scmMCC152DioChannels = 8;
  constexpr size_t scmMCC152AoChannels = 2;
  constexpr uint16_t scmMCC152MaxAoCode = 4095;

  //! One flag per channel: true if the channel has a non-empty IO id
  using TChannelUsage = std::array<bool, 8>;

  // TChannelUsage serves both board types, so its fixed size has to cover every channel set.
  static_assert(scmMCC128Channels == std::tuple_size_v<TChannelUsage>, "TChannelUsage must match the MCC 128");
  static_assert(scmMCC152DioChannels == std::tuple_size_v<TChannelUsage>, "TChannelUsage must match the MCC 152");

  //! Rounds a code delivered as double by daqhats and clamps it to [0, paMaxCode]
  uint16_t toCode(double paValue, uint16_t paMaxCode);

  // The validators return an empty string if the configuration is valid, otherwise the STATUS text.
  std::string validateAddress(unsigned int paAddress);
  std::string validateMCC128Config(unsigned int paMode, unsigned int paRange, const TChannelUsage &paUsed);

  /*! @brief Derives the DIO_DIRECTION mask from the channel mapping (bit = 1: input, 0: output).
   *
   * The direction is not a separate parameter: a bit with a DO id is an output, everything else an input.
   */
  std::string computeMCC152DirectionMask(const TChannelUsage &paInputs,
                                         const TChannelUsage &paOutputs,
                                         uint8_t &paMask);

  uint16_t clampMCC152AoCode(uint16_t paCode, bool &paClamped);

} // namespace forte::eclipse4diac::io::mccdaqhat
