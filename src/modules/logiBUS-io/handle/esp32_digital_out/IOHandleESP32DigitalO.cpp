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

#include "IOHandleESP32DigitalO.h"

#include <cstdbool>

// for Outputs only
#include "driver/gpio.h"


#include "digital_out.h"



IOHandleESP32DigitalO::IOHandleESP32DigitalO(
    logiBUSIODeviceController *paDeviceCtrl,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID paDataType,
    logiBUSIOEnums::DigitalOutPinNumber paPin
    )
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
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
    if(gpio_num > GPIO_NUM_NC)
    {
      gpio_reset_pin((gpio_num_t)gpio_num);
      /* Set the GPIO as a push/pull output */
      gpio_set_direction((gpio_num_t)gpio_num, GPIO_MODE_OUTPUT);
    }
  }


void IOHandleESP32DigitalO::get(CIEC_ANY &paState) {
}

void IOHandleESP32DigitalO::set(const CIEC_ANY &paState) {
  int targetState = (true == static_cast<const CIEC_BOOL &>(paState)) ? 1 : 0;
  gpio_set_level((gpio_num_t)gpio_num, targetState);
}
