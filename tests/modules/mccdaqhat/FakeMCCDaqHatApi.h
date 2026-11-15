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

#include "../../../modules/mccdaqhat/mccdaqhat_api.h"

#include <array>
#include <string>
#include <vector>

namespace forte::eclipse4diac::io::mccdaqhat::test {

  //! Records every call and returns mNextResult, so board logic can be checked without hardware.
  class FakeMCCDaqHatApi final : public IMCCDaqHatApi {
    public:
      std::vector<HatEntry> mHats;
      int mNextResult = scmSuccess;
      std::vector<std::string> mCalls;
      std::array<uint16_t, 8> mAnalogCodes{};
      uint8_t mInputPort = 0;

      std::vector<HatEntry> listHats() override {
        return mHats;
      }

      const char *errorMessage(int) override {
        return "fake error";
      }

      int mcc128Open(uint8_t paAddress) override {
        return record("mcc128Open", paAddress);
      }
      int mcc128Close(uint8_t paAddress) override {
        return record("mcc128Close", paAddress);
      }
      int mcc128InputModeWrite(uint8_t paAddress, uint8_t paMode) override {
        return record("mcc128InputModeWrite", paAddress, paMode);
      }
      int mcc128InputRangeWrite(uint8_t paAddress, uint8_t paRange) override {
        return record("mcc128InputRangeWrite", paAddress, paRange);
      }
      int mcc128InputReadCode(uint8_t paAddress, uint8_t paChannel, uint16_t &paCode) override {
        paCode = mAnalogCodes[paChannel];
        return record("mcc128InputReadCode", paAddress, paChannel);
      }
      int mcc152Open(uint8_t paAddress) override {
        return record("mcc152Open", paAddress);
      }
      int mcc152Close(uint8_t paAddress) override {
        return record("mcc152Close", paAddress);
      }
      int mcc152DioReset(uint8_t paAddress) override {
        return record("mcc152DioReset", paAddress);
      }
      int mcc152DioConfigWritePort(uint8_t paAddress, DioConfigItem paItem, uint8_t paValue) override {
        return record("mcc152DioConfigWritePort", paAddress, static_cast<unsigned int>(paItem), paValue);
      }
      int mcc152DioInputReadPort(uint8_t paAddress, uint8_t &paValue) override {
        paValue = mInputPort;
        return record("mcc152DioInputReadPort", paAddress);
      }
      int mcc152DioOutputWritePort(uint8_t paAddress, uint8_t paValue) override {
        return record("mcc152DioOutputWritePort", paAddress, paValue);
      }
      int mcc152DioOutputWriteBit(uint8_t paAddress, uint8_t paChannel, uint8_t paValue) override {
        return record("mcc152DioOutputWriteBit", paAddress, paChannel, paValue);
      }
      int mcc152OutputWriteCode(uint8_t paAddress, uint8_t paChannel, uint16_t paCode) override {
        return record("mcc152OutputWriteCode", paAddress, paChannel, paCode);
      }

    private:
      int record(const std::string &paName, unsigned int paArg) {
        mCalls.push_back(paName + "(" + std::to_string(paArg) + ")");
        return mNextResult;
      }
      int record(const std::string &paName, unsigned int paArg1, unsigned int paArg2) {
        mCalls.push_back(paName + "(" + std::to_string(paArg1) + "," + std::to_string(paArg2) + ")");
        return mNextResult;
      }
      int record(const std::string &paName, unsigned int paArg1, unsigned int paArg2, unsigned int paArg3) {
        mCalls.push_back(paName + "(" + std::to_string(paArg1) + "," + std::to_string(paArg2) + "," +
                         std::to_string(paArg3) + ")");
        return mNextResult;
      }
  };

} // namespace forte::eclipse4diac::io::mccdaqhat::test
