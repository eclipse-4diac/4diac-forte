/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_CORE_IO_DEVICE_IO_CONTROLLER_POLL_H_
#define SRC_CORE_IO_DEVICE_IO_CONTROLLER_POLL_H_

#include "io_controller.h"
#include <forte_sem.h>

namespace forte {
  namespace core {
    namespace io {

      /*! @brief Abstract Device Controller for devices which require cyclic poll operations
       *
       * IO device controller for devices which require an implementation of IOs using poll operations.
       * Offers a #poll method which performs an IO update in a configured #PollInterval.
       * Allows to force a polling routine with the #forcePoll method (e.g. can be used to set an output immediately).
       */
      class IODevicePollController : public IODeviceController {
        public:

          virtual void handleChangeEvent(IOHandle *paHandle);

        protected:
          /*! @brief Constructor
           *
           * @param paDeviceExecution Device execution where the controller runs
           * @param paPollInterval Default poll routines per seconds (Hz). Must be greater than 0. Call #setPollInterval in the #setConfig method.
           */
          IODevicePollController(CDeviceExecution& paDeviceExecution, float paPollInterval);

          /*! @brief Poll routine which updates the IO state of the device
           *
           * The IO states of all handles should be written/read in this method.
           * The method should call the #checkForInputChanges method after receiving new input states.
           *
           * @attention The method should perform a single update routine and not a blocking loop.
           */
          virtual void poll() = 0;

          /*! @brief Forces an execution of the #poll routine
           *
           * Should be called by the corresponding device #IOHandle implementation after setting/changing an output handle.
           */
          void forcePoll();

          /*! @brief Sets the polling interval in Hz (poll routines per second)
           *
           * The poll interval should not be set to high as the poll operations may consume too much processing power and consequently block other control operations.
           *
           * @param paPollInterval Poll routines per seconds (Hz). Must be greater than 0.
           */
          void setPollInterval(float paPollInterval);

        private:
          virtual void runLoop();

          float mPollInterval;

          CSemaphore mForceLoop;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_DEVICE_IO_CONTROLLER_POLL_H_ */
