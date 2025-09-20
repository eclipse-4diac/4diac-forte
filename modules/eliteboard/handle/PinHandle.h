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

#include "forte/io/mapper/io_handle.h"
#include "forte/io/mapper/io_observer.h"
#include "EliteBoardDeviceController.h"

#include "stm32h743xx.h"
#include "stm32h7xx_hal_gpio.h"

class EliteBoardDeviceController;

class IOHandleGPIO : public forte::io::IOHandle {
    using IOObserver = forte::io::IOObserver;
    using IOHandle = forte::io::IOHandle;
    using IOMapper = forte::io::IOMapper;

  public:
    IOHandleGPIO(EliteBoardDeviceController *paDeviceCtrl, EliteBoardDeviceController::GPIODescriptor desc);
    void get(CIEC_ANY &) override;
    void set(const CIEC_ANY &) override;

  protected:
    void onObserver(IOObserver *paObserver) override;
    void dropObserver() override;

  private:
    GPIO_TypeDef *mGPIO_Port;
    uint16_t mGPIO_Pin;
    std::string const mId;
};

#endif
