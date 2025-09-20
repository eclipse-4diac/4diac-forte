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
 *   Maximilian Scharf - Adaptations for ADC, DAC and event-triggered IOs.
 *******************************************************************************/

#pragma once

#include "forte/io/device/io_controller_poll.h"
#include "forte/io/mapper/io_handle.h"
#include "forte/extevhan.h"

#include "stm32h743xx.h"
#include <string>
#include "forte/io/eIO/CeSpec.h"

#include "stm32h7xx_hal_gpio.h"

class EliteBoardDeviceController : public IODevicePollController,
                                   public RegisterExternalEventHandler<EliteBoardDeviceController> {
  public:
    explicit EliteBoardDeviceController(CDeviceExecution &paDeviceExecution);
    ~EliteBoardDeviceController() override;

    using HandleDescriptor = HandleDescriptor;
    using IOMapper = forte::io::IOMapper;
    using IOHandle = forte::io::IOHandle;

    enum EIOType { GPIO = 10, ADC = 20, DAC = 30 };

    void addHandle(HandleDescriptor &paHandleDescriptor) override {
      IODeviceController::addHandle(paHandleDescriptor);
    }

    void addHandle(std::string const &paId, std::unique_ptr<IOHandle> paHandle) override {
      IODeviceController::addHandle(paId, std::move(paHandle));
    }

    void updateHandleList(std::string const &paId, IOHandle *paHandle) override {
      IODeviceController::updateHandleList(paId, paHandle);
    }

    void setConfig(Config *paConfig) override {
    }

    const char *init() override {
      /* nullptr indicates no error */
      return nullptr;
    }

    IOHandle *createIOHandle(HandleDescriptor &paHandleDescriptor) override;
    bool isHandleValueEqual(IOHandle &paHandle) override;
    void deInit() override {
    }
    void poll() override;
    void startThread();

    class EliteBoardDescriptor : public HandleDescriptor {
      public:
        EIOType mIOType;
        EliteBoardDescriptor(std::string paId, IOMapper::Direction paDirection, EIOType paIOType) :
            HandleDescriptor(std::string(paId), paDirection),
            mIOType(paIOType) {
        }
    };

    class GPIODescriptor : public EliteBoardDescriptor {
      public:
        GPIO_TypeDef *mGPIO_Port;
        uint16_t mPin;

        GPIODescriptor(CIEC_STRING const &paId,
                       IOMapper::Direction paDirection,
                       GPIO_TypeDef *paGPIO_Port,
                       uint16_t paPin) :
            EliteBoardDescriptor(std::string(paId), paDirection, GPIO),
            mGPIO_Port(paGPIO_Port),
            mPin(paPin) {
        }

        GPIODescriptor(CIEC_STRING const &paId, GPIO_TypeDef *paGPIO_Port, uint16_t paPin) :
            EliteBoardDescriptor(std::string(paId), IOMapper::InOut, GPIO),
            mGPIO_Port(paGPIO_Port),
            mPin(paPin) {
        }
    };
};
