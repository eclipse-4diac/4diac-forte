/*******************************************************************************
 * Copyright (c) 2021, 2025 Jonathan Lainer (kontakt@lainer.co.at), HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *   Franz Höpfinger
 *******************************************************************************/

#include "IOHandleESP32PWMO.h"

#include <cstdbool>
#include "forte_word.h"

#include "digital_out.h"

#define   ISO_VALID_SIGNAL_W            ( 0xFAFFu )      /**< 16 bit max value of valid signal */
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE

#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (400) // Frequency in Hertz. Set frequency at 400 Hz

#define LEDC_TEST_FADE_TIME    (500)


IOHandleESP32PWMO::IOHandleESP32PWMO(
    logiBUSIODeviceController *paDeviceCtrl,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID paDataType,
    logiBUSIOEnums::DigitalOutPinNumber paPin
    )
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
  {

  if (!ledc_timer_init)
  {
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Initialize fade service.
    ledc_fade_func_install(0);


    ledc_timer_init = true;
  }


  // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel.speed_mode     = LEDC_MODE;
    ledc_channel.timer_sel      = LEDC_TIMER;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.duty           = 0; // Set duty to 0%
    ledc_channel.hpoint         = 0;
    ledc_channel.sleep_mode     = LEDC_SLEEP_MODE_KEEP_ALIVE;
    switch (paPin) {
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ00:        ledc_channel.gpio_num = Q00_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ01:        ledc_channel.gpio_num = Q01_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ02:        ledc_channel.gpio_num = Q02_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ03:        ledc_channel.gpio_num = Q03_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ04:        ledc_channel.gpio_num = Q04_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ05:        ledc_channel.gpio_num = Q05_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ06:        ledc_channel.gpio_num = Q06_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ07:        ledc_channel.gpio_num = Q07_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ08:        ledc_channel.gpio_num = Q08_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ09:        ledc_channel.gpio_num = Q09_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ10:        ledc_channel.gpio_num = Q10_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ11:        ledc_channel.gpio_num = Q11_IO;        break;
    }
    if(ledc_channel.gpio_num > GPIO_NUM_NC)
    {
      ledc_channel.channel = static_cast<ledc_channel_t>(ledc_channel_num);     /*!< LEDC channel */
      ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
      ledc_channel_num++;
    }
  }


void IOHandleESP32PWMO::get(CIEC_ANY &paState) {
}



void IOHandleESP32PWMO::set(const CIEC_ANY &paState) {
  setWord(static_cast<const CIEC_WORD&>(paState));
}

void IOHandleESP32PWMO::setWord(const CIEC_WORD &paState) {
  TForteWord dataWord = paState;


  ledc_set_fade_with_time(ledc_channel.speed_mode,
                          ledc_channel.channel, dataWord, LEDC_TEST_FADE_TIME);
  ledc_fade_start(ledc_channel.speed_mode,
                  ledc_channel.channel, LEDC_FADE_NO_WAIT);


}


// Prepare and then apply the LEDC PWM timer configuration
ledc_timer_config_t IOHandleESP32PWMO::ledc_timer = {
    .speed_mode       = LEDC_MODE,
    .duty_resolution  = LEDC_DUTY_RES,
    .timer_num        = LEDC_TIMER,
    .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 400 Hz
    .clk_cfg          = LEDC_AUTO_CLK
};

bool IOHandleESP32PWMO::ledc_timer_init = false;
int32_t IOHandleESP32PWMO::ledc_channel_num = LEDC_CHANNEL_0;            /*!< LEDC channel */




