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

#include "IOHandleESP32LEDO_LEDC_MODE.h"

#include <cstdbool>
#include <cstdint>


#include "driver/ledc.h"
// for LED
#include "led_indicator.h"


#include "digital_out.h"


typedef enum {
  BLINK_INVALID,
  BLINK_5HZ,  //highest Priority
  BLINK_2HZ,
  BLINK_1HZ,
  BLINK_05HZ,
  BREATHING_05HZ,
  LED_ON,
  BLINK_OFF, //lowest Priority
  BLINK_NUM,
} led_blink_type_t;



/**
 * off
 *
 */
static const blink_step_t blink_off[] = {
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_5HZ[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 100},
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_2HZ[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 250},
    {LED_BLINK_HOLD, LED_STATE_OFF, 250},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_1HZ[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 500},
    {LED_BLINK_HOLD, LED_STATE_OFF, 500},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_05HZ[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 1000},
    {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Breathe with 0.5 Hz
 * means a T of 2s
 * so every quarter is 0.5s
 *
 */
static const blink_step_t breathing_05HZ[] = {
  {LED_BLINK_BREATHE, LED_STATE_ON, 500},
  {LED_BLINK_BRIGHTNESS, LED_STATE_ON, 500},
  {LED_BLINK_BREATHE, LED_STATE_OFF, 500},
  {LED_BLINK_BRIGHTNESS, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Just ON !
 *
 */
static const blink_step_t just_ON[] = {
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};



//Priorities:


static blink_step_t const *led_blink_lst[] = {
  [BLINK_INVALID]  = nullptr,
  [BLINK_5HZ]      = blink_5HZ,  //highest Priority
  [BLINK_2HZ]      = blink_2HZ,
  [BLINK_1HZ]      = blink_1HZ,
  [BLINK_05HZ]     = blink_05HZ,
  [BREATHING_05HZ] = breathing_05HZ,
  [LED_ON]         = just_ON,
  [BLINK_OFF]      = blink_off,  //lowest Priority
  [BLINK_NUM]      = nullptr,
};


led_indicator_handle_t IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(logiBUSIOEnums::DigitalOutPinNumber::DigitalOutNumberOfPins)] = {nullptr};
int32_t IOHandleESP32LEDO_LEDC_MODE::ledc_channel_num = LEDC_CHANNEL_0;            /*!< LEDC channel */


IOHandleESP32LEDO_LEDC_MODE::~IOHandleESP32LEDO_LEDC_MODE(){
  //TODO led_indicator_delete
  IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)] = nullptr;

}





IOHandleESP32LEDO_LEDC_MODE::IOHandleESP32LEDO_LEDC_MODE(
    logiBUSIODeviceController *paDeviceCtrl,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID paDataType,
    logiBUSIOEnums::DigitalOutPinNumber paPin,
    logiBUSIODeviceController::HandleType paHandleType
    )
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
    , mPin(paPin)
    , mHandleType(paHandleType)
    , gpio_num(GPIO_NUM_NC)
  {
    switch (paPin) {
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ00:        gpio_num = Q00_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ01:        gpio_num = Q01_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ02:        gpio_num = Q02_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ03:        gpio_num = Q03_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ04:        gpio_num = Q04_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ05:        gpio_num = Q05_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ06:        gpio_num = Q06_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ07:        gpio_num = Q07_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ08:        gpio_num = Q08_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ09:        gpio_num = Q09_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ10:        gpio_num = Q10_IO;        break;
      case logiBUSIOEnums::DigitalOutPinNumber::PinQ11:        gpio_num = Q11_IO;        break;
    }
  if(gpio_num > GPIO_NUM_NC) {
    if(IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)] == nullptr) {

      led_indicator_ledc_config_t led_indicator_ledc_config = {
        .is_active_level_high = 1,
        .timer_inited = false,
        .timer_num = LEDC_TIMER_0, /*!< The timer source of channel */
        .gpio_num = gpio_num,   /*!< num of gpio */
        .channel = static_cast<ledc_channel_t>(ledc_channel_num),     /*!< LEDC channel */
      };
      ledc_channel_num++;

      led_indicator_config_t config = {
        .mode = LED_LEDC_MODE,
        .led_indicator_ledc_config = &led_indicator_ledc_config,
        .blink_lists = led_blink_lst,
        .blink_list_num = BLINK_NUM,
      };

      IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)] = led_indicator_create(&config);
      led_indicator_start(IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)], BLINK_OFF);
    }
  }
}


void IOHandleESP32LEDO_LEDC_MODE::get(CIEC_ANY &paState) {
}




void IOHandleESP32LEDO_LEDC_MODE::set(const CIEC_ANY &paState) {
  int targetState = (true == static_cast<const CIEC_BOOL &>(paState)) ? 1 : 0;
  int blink_type = BLINK_INVALID;
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_5HZ:                 blink_type = BLINK_5HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_2HZ:                 blink_type = BLINK_2HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_1HZ:                 blink_type = BLINK_1HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_05HZ:                blink_type = BLINK_05HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_breathing_05HZ:      blink_type = BREATHING_05HZ;  break;
    case logiBUSIODeviceController::HandleType::ESP32LED_PWM_ON:                  blink_type = LED_ON;          break;
    default:                                                                     blink_type = BLINK_INVALID;   break;
  }

  if(blink_type > BLINK_INVALID) {
    switch(targetState){
      case 1:
        led_indicator_start (IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)], blink_type);
        break;
      case 0:
        led_indicator_stop (IOHandleESP32LEDO_LEDC_MODE::led_handles[static_cast<int32_t>(this->mPin)], blink_type);
        break;
      default:
        break;
    }
  }
}
