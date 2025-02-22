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

#ifndef SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32LEDO_STRIPS_MODE_H_
#define SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32LEDO_STRIPS_MODE_H_

#include <core/io/mapper/io_handle.h>
#include <core/io/mapper/io_observer.h>
#include <logiBUSIODeviceController.h>
#include <stdint.h>

// for LED
#include "led_indicator.h"

class logiBUSIODeviceController;

class IOHandleESP32LEDO_STRIPS_MODE : public forte::core::io::IOHandle {

public:
    IOHandleESP32LEDO_STRIPS_MODE(
      logiBUSIODeviceController            *paDeviceCtrl,
      forte::core::io::IOMapper::Direction paDirection,
      CIEC_ANY::EDataTypeID                paDataType,
      logiBUSIOEnums::LEDStripsPinNumber    paPin,
      logiBUSIODeviceController::HandleType paHandleType,
      uint16_t                             paColour
      );
  ~IOHandleESP32LEDO_STRIPS_MODE();
  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;



private:
  logiBUSIOEnums::LEDStripsPinNumber    mPin;
  logiBUSIODeviceController::HandleType mHandleType;
  int32_t                              gpio_num;   /*!< num of gpio */
  uint16_t                             mColour;
  static led_indicator_handle_t        led_handles[static_cast<int32_t>(logiBUSIOEnums::LEDStripsPinNumber::LEDStripsNumberOfPins)];

    int set_red    (int blink_type);
    int set_yellow (int blink_type);
    int set_orange (int blink_type);
    int set_green  (int blink_type);
    int set_blue   (int blink_type);
    int set_white  (int blink_type);
    int set_magenta(int blink_type);
    int set_cyan   (int blink_type);
};

#endif /* SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32LEDO_STRIPS_MODE_H_ */
