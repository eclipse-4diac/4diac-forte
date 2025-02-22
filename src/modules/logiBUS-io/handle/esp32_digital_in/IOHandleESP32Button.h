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

#pragma once

#include <core/io/mapper/io_handle.h>
#include <core/io/mapper/io_observer.h>
#include <logiBUSIODeviceController.h>
#include <stdint.h>
#include "iot_button.h"

class logiBUSIODeviceController;

class IOHandleESP32Button : public forte::core::io::IOHandle {

public:
    IOHandleESP32Button(logiBUSIODeviceController *paDeviceCtrl, forte::core::io::IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paDataType, logiBUSIOEnums::DigitalInPinNumber paPin);
    ~IOHandleESP32Button();
    void register_GPIO_BUTTON(logiBUSIOEnums::DigitalInPinNumber paPin, int32_t gpio_num);
    static void button_cb(void *button_handle, void *usr_data);
    void button_cb(void *button_handle);

  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;

private:
  bool newData;
  button_handle_t g_btn;
};

