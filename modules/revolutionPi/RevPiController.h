/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#pragma once

#include "forte/io/device/io_controller_multi.h"
#include "lib/piControlIf.h"
#include "forte/arch/forte_sem.h"

namespace forte::eclipse4diac::io::revpi {

  class RevPiHandle;

  class RevPiController : public ::forte::io::IODeviceMultiController {
    public:
      RevPiController(CDeviceExecution &paDeviceExecution);

      struct Config : IODeviceController::Config {
          unsigned int updateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
      };

      struct HandleDescriptor : IODeviceMultiController::HandleDescriptor {
          CIEC_ANY::EDataTypeID mType;
          uint8_t mOffset;
          uint8_t mPosition;

          HandleDescriptor(std::string const &paID,
                           forte::io::IOMapper::Direction paDirection,
                           size_t paSlaveIndex,
                           CIEC_ANY::EDataTypeID paType,
                           uint8_t paOffset,
                           uint8_t paPosition) :
              IODeviceMultiController::HandleDescriptor(paID, paDirection, paSlaveIndex),
              mType(paType),
              mOffset(paOffset),
              mPosition(paPosition) {
          }
      };

      void setConfig(struct IODeviceController::Config *config);

      /*! @brief Adds a handle for a slave
       *
       * The controller should read and write the given handle.
       * It should keep a list of handles and delete the provided handle in case the #dropSlaveHandles method is called.
       *
       * @param index Index/Position of the modular slave
       * @param handle Handle object which should be updated by the controller.
       */
      void addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> handle) override;

      /*! @brief Drop all handles of a specific slave
       *
       * The method should delete all handles of the indexed slave.
       * All handles which were added by the #addSlaveHandle method should be deleted.
       *
       * @param index Index/Position of the modular slave
       */
      void dropSlaveHandles(size_t paIndex);

    protected:
      const char *init();

      forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &handleDescriptor) override;

      void deInit();

      /*! @brief Contains the blocking run sequence.
       *
       * The method should implement the algorithm, which sets and gets the state of the hardware handles.
       * In case of a runtime error, set the #error property.
       * It should always check if the controller is still active (#isAlive method).
       * If the controller got inactive or an error occurred, the method should return.
       *
       * @attention Never use while(true) loops without any sleep methods. They would consume all processing power and
       * slow down FORTE.
       */
      void runLoop();

      size_t mDeviceCount;
      SDeviceInfoStr mDeviceList[20];

      arch::CSemaphore mTimeoutSemaphore;

      Config mConfig;

      /*! @brief Checks if the value of a handle has changed. Used by the #checkForInputChanges method.
       *
       * @param handle Handle which should be compared to the previous IO state
       * @return True if the current state is equal to the previous IO state. In case it has changed, return false.
       */
      virtual bool isHandleValueEqual(forte::io::IOHandle &handle);

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

      static const char *const scmFailedToOpenControlFile;
      static const char *const scmFailedToResetControllerFile;
      static const char *const scmFailedToGetDeviceList;
  };

} // namespace forte::eclipse4diac::io::revpi
