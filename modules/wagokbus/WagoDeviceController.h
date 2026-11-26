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

#pragma once

#include "forte/io/device/io_controller_multi.h"
#include "types/WagoRegComCmd_dtp.h"

#define OS_MUST_BE_ARRAY

extern "C" {
#include <dal/adi_application_interface.h>
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>
}

namespace forte::eclipse4diac::io::wago {

  class WagoRegComDevice;

  class WagoDeviceController : public ::forte::io::IODeviceMultiController {
    public:
      explicit WagoDeviceController(CDeviceExecution &paDeviceExecution);

      ~WagoDeviceController() override;

      struct WagoConfig : IODeviceController::Config {
          unsigned int updateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
      };

      class WagoHandleDescriptor : public IODeviceMultiController::HandleDescriptor {
        public:
          CIEC_ANY::EDataTypeID mType;
          TForteUInt32 mChannel;

          WagoHandleDescriptor(std::string const &paId,
                               forte::io::IOMapper::Direction paDirection,
                               size_t paSlaveIndex,
                               CIEC_ANY::EDataTypeID paType,
                               TForteUInt32 paChannel) :
              IODeviceMultiController::HandleDescriptor(paId, paDirection, paSlaveIndex),
              mType(paType),
              mChannel(paChannel) {
          }
      };

      void setConfig(struct IODeviceController::Config *paConfig) override;
      void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) override;
      void dropSlaveHandles(size_t paIndex) override;

      bool enableRegCom(WagoRegComDevice *paECStartFB);
      bool disableRegCom();
      bool writeRegComRequest(const CIEC_WagoRegComCmd &paCmd);
      bool readRegComRequest(const CIEC_WagoRegComCmd &paCmd);
      bool readRegComResult(CIEC_BYTE &paD0, CIEC_BYTE &paD1);
      void initRegComOffsets(WagoRegComDevice *paECStartFB);
      bool regComInitFailed();

    protected:
      const char *init();
      void deInit() override;
      void runLoop() override;

      forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) override;

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

      arch::CSyncObject mRegComMutex;

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

      void checkForRegComChanges();
      TForteByte mREG_C = 0x80;
      bool isRegComOn = false;
      WagoRegComDevice *mRegComDevice = nullptr;
      int mReadCounter = 0;

      static constexpr tDeviceId scmInvalidDeviceId = -1;
      static constexpr size_t scmNumberOfDevicesToScan = 10;
      static constexpr int scmRegComInitFail = -1;

      static constexpr const char* scmKBusDeviceName = "libpackbus";

      static constexpr const char* scmFailedToGetApplicationInterface = "Failed to get the application Interface";
      static constexpr const char* scmFailedToInitializeKBus =
            "Failed to initialize the KBus device. Probably there's another program that is using the bus";
      static constexpr const char* scmFailedToScanDevices = "Failed to scan devices on the KBus";
      static constexpr const char* scmFailedToOpenKBusDevice = "Failed to open KBus device.";
      static constexpr const char* scmFailedToCreateKBusInfo = "Call to ldkc_KbusInfo_Create() failed";
      static constexpr const char* scmFailedGetTerminalInfo = "Call to ldkc_KbusInfo_GetTerminalInfo() failed";
      static constexpr const char* scmFailedGetTerminalList = "Call to ldkc_KbusInfo_GetTerminalList() failed";
      static constexpr const char* scmFailedToGetDeviceList = "Failed to get device list";
  };

} // namespace forte::eclipse4diac::io::wago
