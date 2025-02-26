/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#ifndef PIN_HANDLE_H
#define PIN_HANDLE_H

#include <core/io/mapper/io_handle.h>
#include <core/io/mapper/io_observer.h>
#include "../handler/EliteBoardDeviceController.h"
#include <stdint.h>

#include "stm32h743xx.h"
#include "stm32h7xx_hal_gpio.h"

class EliteBoardDeviceController;

class IOHandleGPIO : public forte::core::io::IOHandle {
  using IOObserver = forte::core::io::IOObserver;
  using IOHandle = forte::core::io::IOHandle;
  using IOMapper = forte::core::io::IOMapper;

public:
  IOHandleGPIO(EliteBoardDeviceController *paDeviceCtrl,
               GPIO_TypeDef *paGPIO_Port, uint16_t paGPIO_Pin);
  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;

protected:
  void onObserver(IOObserver *paObserver) override;
  void dropObserver() override;

private:
  GPIO_TypeDef *mGPIO_Port;
  uint16_t mGPIO_Pin;
};

#endif
