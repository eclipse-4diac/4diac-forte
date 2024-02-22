/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <io/device/io_controller_multi.h>
#include "lib/piControlIf.h"
#include <forte_sem.h>

class RevPiHandle;

class RevPiController: public forte::core::io::IODeviceMultiController {
public:
  RevPiController(CDeviceExecution& paDeviceExecution);

  struct Config: forte::core::io::IODeviceController::Config {
    unsigned int updateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
  };

  struct HandleDescriptor: forte::core::io::IODeviceMultiController::HandleDescriptor {
    CIEC_ANY::EDataTypeID mType;
    uint8_t mOffset;
    uint8_t mPosition;

    HandleDescriptor(std::string const &paID, forte::core::io::IOMapper::Direction paDirection,
        int paSlaveIndex, CIEC_ANY::EDataTypeID paType, uint8_t paOffset, uint8_t paPosition) :
          forte::core::io::IODeviceMultiController::HandleDescriptor(paID, paDirection, paSlaveIndex),
          mType(paType),
          mOffset(paOffset),
          mPosition(paPosition) {

    }
  };

  void setConfig(struct forte::core::io::IODeviceController::Config* config);

  /*! @brief Adds a handle for a slave
   *
   * The controller should read and write the given handle.
   * It should keep a list of handles and delete the provided handle in case the #dropSlaveHandles method is called.
   *
   * @param index Index/Position of the modular slave
   * @param handle Handle object which should be updated by the controller.
   */
  void addSlaveHandle(int index, forte::core::io::IOHandle* handle);

  /*! @brief Drop all handles of a specific slave
   *
   * The method should delete all handles of the indexed slave.
   * All handles which were added by the #addSlaveHandle method should be deleted.
   *
   * @param index Index/Position of the modular slave
   */
  void dropSlaveHandles(int index);

protected:
  const char* init();

  forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *handleDescriptor) override;

  void deInit();

  /*! @brief Contains the blocking run sequence.
   *
   * The method should implement the algorithm, which sets and gets the state of the hardware handles.
   * In case of a runtime error, set the #error property.
   * It should always check if the controller is still active (#isAlive method).
   * If the controller got inactive or an error occurred, the method should return.
   *
   * @attention Never use while(true) loops without any sleep methods. They would consume all processing power and slow down FORTE.
   */
  void runLoop();

  int deviceCount;
  SDeviceInfoStr deviceList[20];

  CSemaphore mTimeoutSemaphore;

  Config config;

  /*! @brief Checks if the value of a handle has changed. Used by the #checkForInputChanges method.
   *
   * @param handle Handle which should be compared to the previous IO state
   * @return True if the current state is equal to the previous IO state. In case it has changed, return false.
   */
  virtual bool isHandleValueEqual(forte::core::io::IOHandle* handle);

private:
  /*! @brief Checks if a slave exists at the given index
   *
   * @param index Index/Position of the modular slave
   * @return True in case a slave was found at the given position
   */
  bool isSlaveAvailable(int index);

  /*! @brief Checks if the slave type matches the configured type
   *
   * @param index Index/Position of the modular slave
   * @param type Type identifier which describes the modular slave
   * @return True in case the slave at the index has the given type
   */
  bool checkSlaveType(int index, int type);

  static const char * const scmFailedToOpenControlFile;
  static const char * const scmFailedToResetControllerFile;
  static const char * const scmFailedToGetDeviceList;
};
