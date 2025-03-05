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

#ifndef ELITEBOARD_DEVICE_CONTROLLER_H
#define ELITEBOARD_DEVICE_CONTROLLER_H

#include "core/io/device/io_controller.h"
#include "core/io/mapper/io_handle.h"
#include "extevhan.h"

#include "stm32h743xx.h"
#include "stm32h7xx_hal_gpio.h"

#include <string>

class EliteBoardDeviceController : public forte::core::io::IODeviceController {
public:
  using HandleDescriptor = forte::core::io::IODeviceController::HandleDescriptor;
  using IOMapper = forte::core::io::IOMapper;
  using IOHandle = forte::core::io::IOHandle;

  DECLARE_HANDLER(EliteBoardDeviceController);

  class EliteBoardHandleDescriptor
    : public forte::core::io::IODeviceController::HandleDescriptor {
  public:
    GPIO_TypeDef *mGPIO_Port;
    uint16_t mPin;

    EliteBoardHandleDescriptor(CIEC_STRING const &paId, forte::core::io::IOMapper::Direction paDirection, GPIO_TypeDef* paGPIO_Port, uint16_t paPin)
      : HandleDescriptor(std::string(paId), IOMapper::UnknownDirection), mGPIO_Port(paGPIO_Port), mPin(paPin){}

    EliteBoardHandleDescriptor(CIEC_STRING const &paId, GPIO_TypeDef* paGPIO_Port, uint16_t paPin)
      : HandleDescriptor(std::string(paId), IOMapper::UnknownDirection), mGPIO_Port(paGPIO_Port), mPin(paPin){}
  };


  IOHandle* createIOHandle(HandleDescriptor& paHandleDescriptor);

  void setConfig(Config *paConfig) {}
  const char *init() {
    const char *x = "";
    return x;
  }
  void runLoop() {}
  void deInit() {}
};

#endif /* ifndef ELITEBOARD_DEVICE_CONTROLLER_H */
