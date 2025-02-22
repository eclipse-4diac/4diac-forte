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

#include "IOHandleESP32Button_LONG_PRESS_START.h"

#include <cstdbool>

#include "button_handle.h"
#include "forte_usint.h"
#include "forte_uint.h"

// for Buttons (input)
#include "iot_button.h"
#include "button_gpio.h"

#include "digital_in.h" //Pin Definitions

IOHandleESP32Button_LONG_PRESS_START::IOHandleESP32Button_LONG_PRESS_START(logiBUSIODeviceController *paDeviceCtrl, forte::core::io::IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paDataType, logiBUSIOEnums::DigitalInPinNumber paPin)
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
    , g_btn(nullptr)
  {
    switch (paPin) {
      case logiBUSIOEnums::DigitalInPinNumber::PinI0:
        register_GPIO_BUTTON(paPin, I0_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI1:
        register_GPIO_BUTTON(paPin, I1_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI2:
        register_GPIO_BUTTON(paPin, I2_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI3:
        register_GPIO_BUTTON(paPin, I3_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI4:
        register_GPIO_BUTTON(paPin, I4_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI5:
        register_GPIO_BUTTON(paPin, I5_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI6:
        register_GPIO_BUTTON(paPin, I6_IO);
        break;      
      case logiBUSIOEnums::DigitalInPinNumber::PinI7:
        register_GPIO_BUTTON(paPin, I7_IO);
        break;
      case logiBUSIOEnums::DigitalInPinNumber::PinI8:
        register_GPIO_BUTTON(paPin, I8_IO);
        break;
      default:
        break;
    }
  }

static const button_event_t EVENT = BUTTON_LONG_PRESS_START;

void IOHandleESP32Button_LONG_PRESS_START::register_GPIO_BUTTON(logiBUSIOEnums::DigitalInPinNumber paPin, int32_t gpio_num) {
  //Check if button was already created
  if(g_btns[static_cast<int32_t>(paPin)] == nullptr) //button was not created before.
  {
    button_config_t cfg = {0};
    button_gpio_config_t gpio_cfg = {
        .gpio_num = gpio_num,
        .active_level = ACTIVE_LEVEL,
    };
    esp_err_t ret = iot_button_new_gpio_device(&cfg, &gpio_cfg, &g_btns[static_cast<int32_t>(paPin)]);
    assert(ret == ESP_OK);
  }
  g_btn = g_btns[static_cast<int32_t>(paPin)];
  iot_button_register_cb(g_btns[static_cast<int32_t>(paPin)], EVENT, nullptr, button_cb, this);
}

void IOHandleESP32Button_LONG_PRESS_START::button_cb(void *button_handle, void *usr_data) {
  if(usr_data) {
    IOHandleESP32Button_LONG_PRESS_START *p = static_cast<IOHandleESP32Button_LONG_PRESS_START *>(usr_data);
    p->button_cb(button_handle);
  }
}

void IOHandleESP32Button_LONG_PRESS_START::button_cb(void *button_handle) {
  this->onChange();
}


void IOHandleESP32Button_LONG_PRESS_START::get(CIEC_ANY &paState) {
}

IOHandleESP32Button_LONG_PRESS_START::~IOHandleESP32Button_LONG_PRESS_START() {
  iot_button_unregister_cb(g_btn, EVENT, nullptr);
  DEVLOG_DEBUG("IOHandleESP32Button unregistered");
  size_t count = iot_button_count_cb(g_btn);
  DEVLOG_DEBUG("count  %i", count);
  if (0 == count) {
    DEVLOG_DEBUG("iot_button_delete unregistered");
    size_t i = get_btn_index(g_btn);
    iot_button_delete(g_btn);
    g_btns[i] = nullptr;
    g_btn = nullptr;
  }
}

void IOHandleESP32Button_LONG_PRESS_START::set(const CIEC_ANY &paState) {
}
