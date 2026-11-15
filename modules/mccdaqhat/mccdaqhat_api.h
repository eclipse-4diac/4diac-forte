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

#include <cstdint>
#include <memory>
#include <vector>

namespace forte::eclipse4diac::io::mccdaqhat {

  //! HAT product IDs as reported by hat_list() (values of enum HatIDs in daqhats.h)
  enum class HatType : uint16_t {
    None = 0,
    MCC128 = 0x0146,
    MCC152 = 0x0144
  };

  //! MCC 152 port configuration items (values of enum DIOConfigItem in mcc152.h)
  enum class DioConfigItem : uint8_t {
    Direction = 0,
    PullConfig = 1,
    PullEnable = 2
  };

  struct HatEntry {
      uint8_t mAddress;
      uint16_t mId;
  };

  /*! @brief Seam over the vendor library.
   *
   * Every daqhats call of the module goes through this interface so that configuration logic and board classes can
   * be unit tested without hardware. Results follow the daqhats ResultCode convention: 0 = success, < 0 = error.
   */
  class IMCCDaqHatApi {
    public:
      static constexpr int scmSuccess = 0;

      virtual ~IMCCDaqHatApi() = default;

      virtual std::vector<HatEntry> listHats() = 0;
      virtual const char *errorMessage(int paResult) = 0;

      virtual int mcc128Open(uint8_t paAddress) = 0;
      virtual int mcc128Close(uint8_t paAddress) = 0;
      virtual int mcc128InputModeWrite(uint8_t paAddress, uint8_t paMode) = 0;
      virtual int mcc128InputRangeWrite(uint8_t paAddress, uint8_t paRange) = 0;
      //! One calibrated but unscaled ADC code (0..65535)
      virtual int mcc128InputReadCode(uint8_t paAddress, uint8_t paChannel, uint16_t &paCode) = 0;

      virtual int mcc152Open(uint8_t paAddress) = 0;
      virtual int mcc152Close(uint8_t paAddress) = 0;
      virtual int mcc152DioReset(uint8_t paAddress) = 0;
      virtual int mcc152DioConfigWritePort(uint8_t paAddress, DioConfigItem paItem, uint8_t paValue) = 0;
      virtual int mcc152DioInputReadPort(uint8_t paAddress, uint8_t &paValue) = 0;
      virtual int mcc152DioOutputWritePort(uint8_t paAddress, uint8_t paValue) = 0;
      virtual int mcc152DioOutputWriteBit(uint8_t paAddress, uint8_t paChannel, uint8_t paValue) = 0;
      //! One unscaled DAC code (0..4095)
      virtual int mcc152OutputWriteCode(uint8_t paAddress, uint8_t paChannel, uint16_t paCode) = 0;
  };

  //! Production implementation backed by libdaqhats
  std::unique_ptr<IMCCDaqHatApi> createDaqHatsLibApi();

} // namespace forte::eclipse4diac::io::mccdaqhat
