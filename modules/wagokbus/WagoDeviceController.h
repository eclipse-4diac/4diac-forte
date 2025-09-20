/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#ifndef SRC_MODULES_WAGOKBUS_MODULAR_WAGODEVICECONTROLLER_H_
#define SRC_MODULES_WAGOKBUS_MODULAR_WAGODEVICECONTROLLER_H_

#include "forte/io/device/io_controller_multi.h"

#define OS_MUST_BE_ARRAY

extern "C" {
#include <dal/adi_application_interface.h>
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>
}

class WagoDeviceController : public forte::io::IODeviceMultiController {
  public:
    explicit WagoDeviceController(CDeviceExecution &paDeviceExecution);

    ~WagoDeviceController() override;

    struct WagoConfig : forte::io::IODeviceController::Config {
        unsigned int updateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
    };

    class WagoHandleDescriptor : public forte::io::IODeviceMultiController::HandleDescriptor {
      public:
        CIEC_ANY::EDataTypeID mType;
        TForteUInt32 mChannel;

        WagoHandleDescriptor(std::string const &paId,
                             forte::io::IOMapper::Direction paDirection,
                             size_t paSlaveIndex,
                             CIEC_ANY::EDataTypeID paType,
                             TForteUInt32 paChannel) :
            forte::io::IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex),
            mType(paType),
            mChannel(paChannel) {
        }
    };

    void setConfig(struct forte::io::IODeviceController::Config *paConfig) override;

    void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;

    void dropSlaveHandles(size_t paIndex) override;

  protected:
    const char *init();

    forte::io::IOHandle *createIOHandle(forte::io::IODeviceController::HandleDescriptor &paHandleDescriptor) override;

    void deInit() override;

    void runLoop() override;

    tApplicationDeviceInterface *mAppDevInterface;
    uint32_t mTaskId;
    tDeviceId mKBusDeviceId;

    /*KBus Terminal information */
    size_t mTerminalCount;
    u16 mTerminalIds[LDKC_KBUS_TERMINAL_COUNT_MAX];
    tldkc_KbusInfo_TerminalInfo mTerminalInfos[LDKC_KBUS_TERMINAL_COUNT_MAX];

    WagoConfig mConfig;

    /*! @brief Checks if the value of a handle has changed. Used by the #checkForInputChanges method.
     *
     * @param handle Handle which should be compared to the previous IO state
     * @return True if the current state is equal to the previous IO state. In case it has changed, return false.
     */
    virtual bool isHandleValueEqual(forte::io::IOHandle &paHandle) override;

  private:
    /*! @brief Checks if a slave exists at the given index
     *
     * @param index Index/Position of the modular slave
     * @return True in case a slave was found at the given position
     */
    bool isSlaveAvailable(size_t paIndex);

    /*! @brief Checks if the slave type matches the configured type
     *
     * @param index Index/Position of the modular slave
     * @param type Type identifier which describes the modular slave
     * @return True in case the slave at the index has the given type
     */
    bool checkSlaveType(size_t paIndex, int paType);

    const char *loadTerminalInformation();

    bool triggerKBusCycle();

    static const tDeviceId scmInvalidDeviceId = -1;
    static const size_t scmNumberOfDevicesToScan = 10;

    static const char *const scmKBusDeviceName;

    static const char *const scmFailedToGetApplicationInterface;
    static const char *const scmFailedToInitializeKBus;
    static const char *const scmFailedToScanDevices;
    static const char *const scmFailedToOpenKBusDevice;
    static const char *const scmFailedToGetDeviceList;

    static const char *const scmFailedToCreateKBusInfo;
    static const char *const scmFailedGetTerminalInfo;
    static const char *const scmFailedGetTerminalList;
};

#endif /* SRC_MODULES_WAGOKBUS_MODULAR_WAGODEVICECONTROLLER_H_ */
