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

#include "logiBUSIODeviceController.h"

#if defined(FORTE_MODULE_ESP32_DIGITAL_OUT)
#include "IOHandleESP32DigitalO.h"
#include "IOHandleESP32PWMO.h"
#include "IOHandleESP32ServoO.h"
#include "IOHandleESP32LEDO_GPIO_MODE.h"
#include "IOHandleESP32LEDO_LEDC_MODE.h"
#endif //FORTE_MODULE_ESP32_DIGITAL_OUT

#if defined(FORTE_MODULE_ESP_ADDONS)
#include "IOHandleESP32LEDO_STRIPS_MODE.h"
#endif //FORTE_MODULE_ESP_ADDONS

#if defined(FORTE_MODULE_ESP32_ANALOG_IN)
#include "IOHandleESP32AnalogIN.h"
#endif //FORTE_MODULE_ESP32_ANALOG_IN

#if defined(FORTE_MODULE_ESP32_DIGITAL_IN)
#include "IOHandleESP32Button.h"
#include "IOHandleESP32Button_DOWN.h"
#include "IOHandleESP32Button_UP.h"
#include "IOHandleESP32Button_REPEAT.h"
#include "IOHandleESP32Button_SINGLE_CLICK.h"
#include "IOHandleESP32Button_DOUBLE_CLICK.h"
#include "IOHandleESP32Button_LONG_PRESS_START.h"
#include "IOHandleESP32Button_LONG_PRESS_HOLD.h"
#endif //FORTE_MODULE_ESP32_DIGITAL_IN


DEFINE_HANDLER(logiBUSIODeviceController);

logiBUSIODeviceController::logiBUSIODeviceController(CDeviceExecution& paDeviceExecution) 
: forte::core::io::IODeviceController(paDeviceExecution){}

logiBUSIODeviceController::~logiBUSIODeviceController() = default;

forte::core::io::IOHandle *
logiBUSIODeviceController::createIOHandle(forte::core::io::IODeviceController::HandleDescriptor &paHandleDescriptor) {
  logiBUSIOHandleDescriptor &desc = static_cast<logiBUSIOHandleDescriptor &>(paHandleDescriptor);
  forte::core::io::IOHandle *handle = nullptr;

  switch (desc.mHandleType) {

#if defined(FORTE_MODULE_ESP32_DIGITAL_OUT)
  case HandleType::ESP32DigitalO:                handle = new IOHandleESP32DigitalO               (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalOutPinNumber>(desc.mPin)); break;
  case HandleType::ESP32PWMO:                    handle = new IOHandleESP32PWMO                   (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalOutPinNumber>(desc.mPin)); break;
  case HandleType::ESP32ServoO:                  handle = new IOHandleESP32ServoO                 (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalOutPinNumber>(desc.mPin)); break;
  case HandleType::ESP32LED_PWM_5HZ:
  case HandleType::ESP32LED_PWM_2HZ:
  case HandleType::ESP32LED_PWM_1HZ:
  case HandleType::ESP32LED_PWM_05HZ:
  case HandleType::ESP32LED_PWM_breathing_05HZ:
  case HandleType::ESP32LED_PWM_ON:              handle = new IOHandleESP32LEDO_LEDC_MODE         (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalOutPinNumber>(desc.mPin), desc.mHandleType); break;
  case HandleType::ESP32LED_DO_5HZ:
  case HandleType::ESP32LED_DO_2HZ:
  case HandleType::ESP32LED_DO_1HZ:
  case HandleType::ESP32LED_DO_05HZ:
  case HandleType::ESP32LED_DO_ON:               handle = new IOHandleESP32LEDO_GPIO_MODE         (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalOutPinNumber>(desc.mPin), desc.mHandleType); break;
#endif //FORTE_MODULE_ESP32_DIGITAL_OUT

#if defined(FORTE_MODULE_ESP_ADDONS)
  case HandleType::ESP32LED_STRIP_5HZ:
  case HandleType::ESP32LED_STRIP_2HZ:
  case HandleType::ESP32LED_STRIP_1HZ:
  case HandleType::ESP32LED_STRIP_05HZ:
  case HandleType::ESP32LED_STRIP_ON:            handle = new IOHandleESP32LEDO_STRIPS_MODE       (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::LEDStripsPinNumber>(desc.mPin), desc.mHandleType, desc.mColour); break;
#endif //FORTE_MODULE_ESP_ADDONS

#if defined(FORTE_MODULE_ESP32_ANALOG_IN)
  case HandleType::ESP32AnalogI:                 handle = new IOHandleESP32AnalogIN               (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::AnalogInPinNumber>(desc.mPin), desc.mAnalogInputHysteresis); break;
#endif //FORTE_MODULE_ESP32_ANALOG_IN

#if defined(FORTE_MODULE_ESP32_DIGITAL_IN)
  case HandleType::ESP32Button:                  handle = new IOHandleESP32Button                 (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_DOWN:             handle = new IOHandleESP32Button_DOWN            (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_UP:               handle = new IOHandleESP32Button_UP              (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_REPEAT:           handle = new IOHandleESP32Button_REPEAT          (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_SINGLE_CLICK:     handle = new IOHandleESP32Button_SINGLE_CLICK    (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_DOUBLE_CLICK:     handle = new IOHandleESP32Button_DOUBLE_CLICK    (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_LONG_PRESS_START: handle = new IOHandleESP32Button_LONG_PRESS_START(this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
  case HandleType::ESP32Button_LONG_PRESS_HOLD:  handle = new IOHandleESP32Button_LONG_PRESS_HOLD (this, desc.mDirection, desc.mDataType, static_cast<logiBUSIOEnums::DigitalInPinNumber>(desc.mPin)); break;
#endif //FORTE_MODULE_ESP32_DIGITAL_IN


  }
  return static_cast<forte::core::io::IOHandle *>(handle);
}

logiBUSIODeviceController::logiBUSIOHandleDescriptor::logiBUSIOHandleDescriptor(
    std::string                   const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    int32_t                              paPin,
    uint32_t                             paAnalogInputHysteresis,
    uint16_t                             paColour
    )
: forte::core::io::IODeviceController::HandleDescriptor(paId, paDirection)
, mDataType(paDataType)
, mHandleType(paHandleType)
, mPin(paPin)
, mAnalogInputHysteresis(paAnalogInputHysteresis)
, mColour(paColour)
{}


