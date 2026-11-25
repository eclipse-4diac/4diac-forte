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

#include "mccdaqhat_api.h"
#include "mccdaqhat_config.h"

#include <cstddef>
#include <daqhats/daqhats.h>

namespace forte::eclipse4diac::io::mccdaqhat {

  namespace {
    class DaqHatsLibApi final : public IMCCDaqHatApi {
      public:
        std::vector<HatEntry> listHats() override {
          std::vector<HatEntry> result;
          const int count = hat_list(HAT_ID_ANY, nullptr);
          if (count <= 0) {
            return result;
          }
          std::vector<HatInfo> infos(static_cast<size_t>(count));
          const int filled = hat_list(HAT_ID_ANY, infos.data());
          for (int i = 0; i < filled && i < count; ++i) {
            result.push_back({infos[static_cast<size_t>(i)].address, infos[static_cast<size_t>(i)].id});
          }
          return result;
        }

        const char *errorMessage(int paResult) override {
          return hat_error_message(paResult);
        }

        int mcc128Open(uint8_t paAddress) override {
          return mcc128_open(paAddress);
        }

        int mcc128Close(uint8_t paAddress) override {
          return mcc128_close(paAddress);
        }

        int mcc128InputModeWrite(uint8_t paAddress, uint8_t paMode) override {
          return mcc128_a_in_mode_write(paAddress, paMode);
        }

        int mcc128InputRangeWrite(uint8_t paAddress, uint8_t paRange) override {
          return mcc128_a_in_range_write(paAddress, paRange);
        }

        int mcc128InputReadCode(uint8_t paAddress, uint8_t paChannel, uint16_t &paCode) override {
          double value = 0.0;
          const int result = mcc128_a_in_read(paAddress, paChannel, OPTS_NOSCALEDATA, &value);
          if (result == RESULT_SUCCESS) {
            paCode = toCode(value, scmMCC128MaxCode);
          }
          return result;
        }

        int mcc152Open(uint8_t paAddress) override {
          return mcc152_open(paAddress);
        }

        int mcc152Close(uint8_t paAddress) override {
          return mcc152_close(paAddress);
        }

        int mcc152DioReset(uint8_t paAddress) override {
          return mcc152_dio_reset(paAddress);
        }

        int mcc152DioConfigWritePort(uint8_t paAddress, DioConfigItem paItem, uint8_t paValue) override {
          return mcc152_dio_config_write_port(paAddress, static_cast<uint8_t>(paItem), paValue);
        }

        int mcc152DioInputReadPort(uint8_t paAddress, uint8_t &paValue) override {
          return mcc152_dio_input_read_port(paAddress, &paValue);
        }

        int mcc152DioOutputWritePort(uint8_t paAddress, uint8_t paValue) override {
          return mcc152_dio_output_write_port(paAddress, paValue);
        }

        int mcc152DioOutputWriteBit(uint8_t paAddress, uint8_t paChannel, uint8_t paValue) override {
          return mcc152_dio_output_write_bit(paAddress, paChannel, paValue);
        }

        int mcc152OutputWriteCode(uint8_t paAddress, uint8_t paChannel, uint16_t paCode) override {
          return mcc152_a_out_write(paAddress, paChannel, OPTS_NOSCALEDATA, static_cast<double>(paCode));
        }
    };
  } // namespace

  std::unique_ptr<IMCCDaqHatApi> createDaqHatsLibApi() {
    return std::make_unique<DaqHatsLibApi>();
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
