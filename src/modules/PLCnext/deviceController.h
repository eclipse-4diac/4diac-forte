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

#include "../../../core/io/device/io_controller_multi.h"
#include <syslog.h>
#include <unistd.h>
#include <libgen.h>
#include <thread>
#include <pthread.h>
#include <fortelist.h>
#include "forte_wstring.h"

class PLCnextSlaveHandler;

class PLCnextDeviceController : public forte::core::io::IODeviceMultiController {
  public:

    enum HandleType {
        Bit = 1,             // Digital
        BitString16 = 16     // Analog
    };

    PLCnextDeviceController(CDeviceExecution& paDeviceExecution) : forte::core::io::IODeviceMultiController(paDeviceExecution) {
        mConfig.updateInterval = 25;  // set default
    };

    ~PLCnextDeviceController();

    struct PLCnextConfig : forte::core::io::IODeviceController::Config {
        unsigned int updateInterval;
    };

    class HandleDescriptor : public forte::core::io::IODeviceMultiController::HandleDescriptor {
        public:
            HandleType mType;
            uint16_t mPosition;

            HandleDescriptor(CIEC_WSTRING const& paId, forte::core::io::IOMapper::Direction paDirection, int paSlaveIndex, uint16_t position, HandleType paType) :
                forte::core::io::IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex), mPosition(position), mType(paType) {
            }
    };

    void setConfig(struct forte::core::io::IODeviceController::Config* paConfig) override;
    void registerSlaveHandler(PLCnextSlaveHandler* slave);

    PLCnextSlaveHandler* getSlave(int paIndex);
    void addSlaveHandle(int index, forte::core::io::IOHandle* paHandle) override;
    void dropSlaveHandles(int paIndex) override;
    
  protected:
    const char* init() override;

    forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) override;

    void deInit() override;
    void runLoop() override;
    
    typedef CSinglyLinkedList<PLCnextSlaveHandler*> TSlaveList;
    TSlaveList* mSlaves = new TSlaveList();;

    PLCnextConfig mConfig;
    
  private:
    bool isSlaveAvailable(int paIndex) override;
    bool checkSlaveType(int paIndex, int paType) override;
};

#endif /* SRC_MODULES_PLCNEXT_PLCNEXTDEVICECONTROLLER_H_ */
