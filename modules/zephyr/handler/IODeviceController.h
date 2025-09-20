/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0

 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef ZEPHYRIO_DEVICE_CONTROLLER_H
#define ZEPHYRIO_DEVICE_CONTROLLER_H

#include "forte/io/device/io_controller.h"
#include "forte/io/mapper/io_handle.h"
#include "forte/extevhan.h"

#include <handler/IOHandleDescriptor.h>

class IODeviceController : public forte::io::IODeviceController,
                           public RegisterExternalEventHandler<IODeviceController> {
  public:
    using HandleDescriptor = forte::io::IODeviceController::HandleDescriptor;
    using IOMapper = forte::io::IOMapper;
    using IOHandle = forte::io::IOHandle;

  public:
    explicit IODeviceController(CDeviceExecution &paDeviceExecution);
    ~IODeviceController() override;

    struct Config : forte::io::IODeviceController::Config {
        unsigned int updateInterval = 0; // Sets the period for the data update cycle, default 0 means infinite.
    };

    IOHandle *createIOHandle(HandleDescriptor &paHandleDescriptor) override;

    void setConfig(struct forte::io::IODeviceController::Config *paConfig) override;

    void handleChangeEvent(IOHandle *paHandle) override;

  protected:
    void onAliveChanged(bool paNewValue) override {
      mUpdateSema.inc();
    }
    const char *init() override {
      DEVLOG_INFO("IODeviceController::init\n");
      return nullptr;
    }
    void runLoop() override;
    void deInit() override {
      DEVLOG_INFO("IODeviceController::deInit\n");
      mUpdateSema.inc();
    }

    bool isHandleValueEqual(IOHandle &paHandle) override;

    struct Config mConfig;
    CSemaphore mUpdateSema;
};

#endif /* ifndef ZEPHYRIO_DEVICE_CONTROLLER_H */
