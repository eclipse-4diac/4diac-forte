/*******************************************************************************
 * Copyright (c) 2022 Peirlberger Juergen
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Peirlberger Juergen - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_PLCNEXT_PLCNEXTDEVICECONTROLLER_H_
#define SRC_MODULES_PLCNEXT_PLCNEXTDEVICECONTROLLER_H_

#include "forte/io/device/io_controller_multi.h"
#include <syslog.h>
#include <unistd.h>
#include <libgen.h>
#include "forte/datatypes/forte_wstring.h"

class PLCnextSlaveHandler;

class PLCnextDeviceController : public forte::io::IODeviceMultiController {
  public:
    enum HandleType {
      Bit = 1, // Digital
      BitString16 = 16 // Analog
    };

    PLCnextDeviceController(CDeviceExecution &paDeviceExecution) : IODeviceMultiController(paDeviceExecution) {
      mConfig.updateInterval = 25; // set default
    };

    ~PLCnextDeviceController() override = default;

    struct PLCnextConfig : Config {
        unsigned int updateInterval;
    };

    class HandleDescriptor : public IODeviceMultiController::HandleDescriptor {
      public:
        HandleType mType;
        uint16_t mPosition;

        HandleDescriptor(CIEC_WSTRING const &paId,
                         forte::io::IOMapper::Direction paDirection,
                         size_t paSlaveIndex,
                         uint16_t position,
                         HandleType paType) :
            IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex),
            mType(paType),
            mPosition(position) {
        }
    };

    void setConfig(struct Config *paConfig) override;
    void registerSlaveHandler(PLCnextSlaveHandler *slave);

    PLCnextSlaveHandler *getSlave(size_t paIndex);
    void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;
    void dropSlaveHandles(size_t paIndex) override;

  protected:
    const char *init() override;

    forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) override;

    void deInit() override;
    void runLoop() override;

    std::vector<PLCnextSlaveHandler *> mDevices;

    PLCnextConfig mConfig;

  private:
    bool isSlaveAvailable(size_t paIndex) override;
    bool checkSlaveType(size_t paIndex, int paType) override;
};

#endif /* SRC_MODULES_PLCNEXT_PLCNEXTDEVICECONTROLLER_H_ */
