/*************************************************************************
 * Copyright (c) 2018 fortiss GmbH
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

#include "../../../core/io/device/io_controller_multi.h"

#define OS_MUST_BE_ARRAY

extern "C" {
#include <dal/adi_application_interface.h>
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>
}

class WagoDeviceController : public forte::core::io::IODeviceMultiController {
  public:
    explicit WagoDeviceController(CDeviceExecution& paDeviceExecution);

    virtual ~WagoDeviceController();

    struct WagoConfig : forte::core::io::IODeviceController::Config {
        unsigned int updateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
    };

    class WagoHandleDescriptor : public forte::core::io::IODeviceMultiController::HandleDescriptor {
      public:
        CIEC_ANY::EDataTypeID mType;
        TForteUInt32 mChannel;

        WagoHandleDescriptor(CIEC_WSTRING const &paId, forte::core::io::IOMapper::Direction paDirection, int paSlaveIndex, CIEC_ANY::EDataTypeID paType,
            TForteUInt32 paChannel) :
            forte::core::io::IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex), mType(paType), mChannel(paChannel) {
        }
    };

    void setConfig(struct forte::core::io::IODeviceController::Config* paConfig) override;

    void addSlaveHandle(int index, forte::core::io::IOHandle* paHandle) override;

    void dropSlaveHandles(int paIndex) override;
  protected:
    const char* init();

    forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) override;

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
    virtual bool isHandleValueEqual(forte::core::io::IOHandle* paHandle) override;

  private:
    /*! @brief Checks if a slave exists at the given index
     *
     * @param index Index/Position of the modular slave
     * @return True in case a slave was found at the given position
     */
    bool isSlaveAvailable(int paIndex);

    /*! @brief Checks if the slave type matches the configured type
     *
     * @param index Index/Position of the modular slave
     * @param type Type identifier which describes the modular slave
     * @return True in case the slave at the index has the given type
     */
    bool checkSlaveType(int paIndex, int paType);

    const char* loadTerminalInformation();

    bool triggerKBusCycle();

    static const tDeviceId scmInvalidDeviceId = -1;
    static const size_t scmNumberOfDevicesToScan = 10;

    static const char * const scmKBusDeviceName;

    static const char * const scmFailedToGetApplicationInterface;
    static const char * const scmFailedToInitializeKBus;
    static const char * const scmFailedToScanDevices;
    static const char * const scmFailedToOpenKBusDevice;
    static const char * const scmFailedToGetDeviceList;

    static const char * const scmFailedToCreateKBusInfo;
    static const char * const scmFailedGetTerminalInfo;
    static const char * const scmFailedGetTerminalList;

};

#endif /* SRC_MODULES_WAGOKBUS_MODULAR_WAGODEVICECONTROLLER_H_ */
