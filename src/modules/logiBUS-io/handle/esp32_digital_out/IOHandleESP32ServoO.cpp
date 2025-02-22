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

#include "IOHandleESP32ServoO.h"

#include <cstdbool>
#include "forte_word.h"

#include "digital_out.h"

#define   ISO_VALID_SIGNAL_W            ( 0xFAFFu )      /**< 16 bit max value of valid signal */
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE

#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
//Conrad Datasheet https://asset.conrad.com/media10/add/160267/c1/-/en/000275465DS00/datenblatt-275465-bluebird-spezial-servo-bms-l530mg-analog-servo-getriebe-material-metall-stecksystem-jr.pdf
#define LEDC_FREQUENCY          (250) // Frequency in Hertz. Set frequency at 250 Hz


IOHandleESP32ServoO::IOHandleESP32ServoO(
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
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ01:
        ledc_channel.channel = LEDC_CHANNEL_0;
        ledc_channel.gpio_num = LEDC_IO_0;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ02:
        ledc_channel.channel = LEDC_CHANNEL_1;
        ledc_channel.gpio_num = LEDC_IO_1;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        break;
    }
  }


void IOHandleESP32ServoO::get(CIEC_ANY &paState) {
}



#define period 4.0

//Working Frequency & Neutral 250Hz & 1520us
#define ServoMsMin 0.80 // 0.80 lowest
#define ServoMsMax 2.18 // 2.18 highest
#define ServoSpan (2.18 - 0.80)

void IOHandleESP32ServoO::set(const CIEC_ANY &paState) {
  setWord(static_cast<const CIEC_WORD&>(paState));
}

void IOHandleESP32ServoO::setWord(const CIEC_WORD &paState) {
  TForteWord dataWord = paState;
  if ( dataWord <= ISO_VALID_SIGNAL_W )
  {
    float percent = (float)dataWord / (float)ISO_VALID_SIGNAL_W;
    int duty = (int)(100.0*((ServoMsMin + (ServoSpan * percent))/period)*81.91);
    ledc_set_duty(LEDC_MODE, ledc_channel.channel, duty);
    ledc_update_duty(LEDC_MODE, ledc_channel.channel);
  }
}


// Prepare and then apply the LEDC PWM timer configuration
ledc_timer_config_t IOHandleESP32ServoO::ledc_timer = {
    .speed_mode       = LEDC_MODE,
    .duty_resolution  = LEDC_DUTY_RES,
    .timer_num        = LEDC_TIMER,
    .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 250 Hz
    .clk_cfg          = LEDC_AUTO_CLK
};

bool IOHandleESP32ServoO::ledc_timer_init = false;





