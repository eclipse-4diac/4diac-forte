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

#ifndef SRC_CORE_IO_DEVICE_IO_CONTROLLER_MULTI_H_
#define SRC_CORE_IO_DEVICE_IO_CONTROLLER_MULTI_H_

#include "io_controller.h"
#include <io/mapper/io_handle.h>

class IOConfigFBMultiSlave;

/*! @brief Abstract Device Controller for modular devices
 *
 * The IODeviceMultiController extends the abstract #IODeviceController.
 * It integrates additional functionality for modular devices.
 * The controller should be used with the #IOConfigFBMulti configuration fbs.
 */
class IODeviceMultiController: public IODeviceController {
  friend class IOConfigFBMultiSlave;

public:

  struct HandleDescriptor: IODeviceController::HandleDescriptor {
    int slaveIndex;

    HandleDescriptor(CIEC_WSTRING const &id, IOMapper::Direction direction,
        int slaveIndex) :
        IODeviceController::HandleDescriptor(id, direction), slaveIndex(slaveIndex) {

    }
  };

  /*! @brief Adds a handle for a slave
   *
   * The controller should read and write the given handle.
   * It should keep a list of handles and delete the provided handle in case the #dropSlaveHandles method is called.
   *
   * @param index Index/Position of the modular slave
   * @param handle IOHandle object which should be updated by the controller.
   */
  virtual void addSlaveHandle(int index, IOHandle* handle) = 0;

  /*! @brief Drop all handles of a specific slave
   *
   * The method should delete all handles of the indexed slave.
   * All handles which were added by the #addSlaveHandle method should be deleted.
   *
   * @param index Index/Position of the modular slave
   */
  virtual void dropSlaveHandles(int index) = 0;

protected:
  explicit IODeviceMultiController(CDeviceExecution& paDeviceExecution);

  virtual void addHandle(IODeviceController::HandleDescriptor *handleDescriptor);

  virtual IOHandle* initHandle(HandleDescriptor *handleDescriptor) = 0;

private:

  IOHandle* initHandle(IODeviceController::HandleDescriptor *handleDescriptor) {
    return initHandle(static_cast<HandleDescriptor*>(handleDescriptor));
  }

  /*! @brief Checks if a slave exists at the given index
   *
   * @param index Index/Position of the modular slave
   * @return True in case a slave was found at the given position
   */
  virtual bool isSlaveAvailable(int index) = 0;

  /*! @brief Checks if the slave type matches the configured type
   *
   * @param index Index/Position of the modular slave
   * @param type Type identifier which describes the modular slave
   * @return True in case the slave at the index has the given type
   */
  virtual bool checkSlaveType(int index, int type) = 0;

};

#endif /* SRC_CORE_IO_DEVICE_IO_CONTROLLER_MULTI_H_ */
