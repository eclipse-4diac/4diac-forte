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

#ifndef SRC_MODULES_LOGIBUS_IO_HANDLER_LOGIBUSIODEVICECONTROLLER_H_
#define SRC_MODULES_LOGIBUS_IO_HANDLER_LOGIBUSIODEVICECONTROLLER_H_

#include <cstdint>
#include "core/io/device/io_controller.h"
#include "core/io/mapper/io_handle.h"
#include "extevhan.h"
#include "logiBUSIOEnums.h"


class logiBUSIODeviceController : public forte::core::io::IODeviceController {
public:

  DECLARE_HANDLER(logiBUSIODeviceController);




  //TODO add Analog, PWM etc..
    enum HandleType {
      Invalid = -1,
      ESP32DigitalO,
      ESP32PWMO,
      ESP32AnalogI,
      ESP32LED_PWM_5HZ,
      ESP32LED_PWM_2HZ,
      ESP32LED_PWM_1HZ,
      ESP32LED_PWM_05HZ,
      ESP32LED_PWM_breathing_05HZ,
      ESP32LED_PWM_ON,
      ESP32LED_DO_5HZ,
      ESP32LED_DO_2HZ,
      ESP32LED_DO_1HZ,
      ESP32LED_DO_05HZ,
      ESP32LED_DO_ON,
      ESP32LED_STRIP_5HZ,
      ESP32LED_STRIP_2HZ,
      ESP32LED_STRIP_1HZ,
      ESP32LED_STRIP_05HZ,
      ESP32LED_STRIP_ON,
      ESP32ServoO,
      ESP32Button,
      ESP32Button_DOWN,
      ESP32Button_UP,
      ESP32Button_REPEAT,
      ESP32Button_REPEAT_DONE,
      ESP32Button_SINGLE_CLICK,
      ESP32Button_DOUBLE_CLICK,
      ESP32Button_MULTIPLE_CLICK,
      ESP32Button_LONG_PRESS_START,
      ESP32Button_LONG_PRESS_HOLD,
      ESP32Button_LONG_PRESS_UP,
      ESP32Button_END,
    };



  class logiBUSIOHandleDescriptor
      : public forte::core::io::IODeviceController::HandleDescriptor {
  public:
    CIEC_ANY::EDataTypeID                mDataType;
    logiBUSIODeviceController::HandleType mHandleType;
    int32_t                              mPin;
    uint32_t                             mAnalogInputHysteresis;
    uint16_t                             mColour;


    logiBUSIOHandleDescriptor(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        int32_t                              paPin,
        uint32_t                             paAnalogInputHysteresis,
        uint16_t                             paColour
        );
  };


  forte::core::io::IOHandle* createIOHandle(forte::core::io::IODeviceController::HandleDescriptor &paHandleDescriptor);

  void setConfig(Config *paConfig) {}

  const char *init() {
    const char *x = "";
    return x;
  }


  void runLoop() {}

  void deInit() {}



};

#endif /* SRC_MODULES_LOGIBUS_IO_HANDLER_LOGIBUSIODEVICECONTROLLER_H_ */
