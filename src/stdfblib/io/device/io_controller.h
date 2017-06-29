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

#ifndef SRC_STDFBLIB_IO_DEVICE_CONTROLLER_H_
#define SRC_STDFBLIB_IO_DEVICE_CONTROLLER_H_

#include <extevhan.h>
#include <forte_sync.h>
#include <forte_thread.h>
#include <devlog.h>

namespace IO {

namespace ConfigurationFB {
class Controller;
}

namespace Device {

/*! @brief Abstract Device Controller
 *
 * Base class of the device controller implementation.
 * All hardware specific IO implementations should inherit from this class.
 *
 * Features:
 * - runs Device Controller in separate thread
 * - thread safe communication with corresponding controller configuration fb (see #notifyConfigFB)
 * - base structure for safe startups and resets (see #init, #runLoop, and #deInit)
 * - automatic error handling and restart (see #IO::ConfigurationFB::Controller)
 */
class Controller: protected CExternalEventHandler, protected CThread {
  friend class ConfigurationFB::Controller;

public:
  /*! @brief Abstract configuration struct.
   *
   * The child controller should implement an own configuration struct, which inherits from this struct and is initialized with default parameters.
   * The controller configuration fb uses the #setConfig method to set the runtime configuration.
   */
  struct Config {

  };

  enum NotificationType {
    UnknownNotificationType,
    //! Notifies the configuration fb about a successful operation e.g. a successful startup.
    Success,
    //! Notifies the configuration about an error. It should be emitted during the init method or the runLoop method.
    Error,
    //! Placeholder for custom hardware specific communication between the device and configuration fb controller.
    Custom
  };
  NotificationType notificationType;
  const void* notificationAttachment;
  bool notificationHandled;

  /*! @brief Sets the configuration of the controller
   *
   * @param config Pointer to the configuration struct provided by the corresponding controller configuration fb. Use *static_cast<CustomConfig*>(config) to retrieve the struct instance.
   */
  virtual void setConfig(Config* config) = 0;

protected:
  Controller();

  /*! @brief Initializes the controller.
   *
   * The method should initialize and startup the controller.
   * All required hardware libraries should be started and checked for errors.
   * If the system requires an initialization sequence which is executed once at startup, place it here.
   * In case of an error, set the #error property to provide debugging information.
   *
   * @return true if the initialization was successful, false if not.
   */
  virtual bool init() = 0;

  /*! @brief Contains the blocking run sequence.
   *
   * The method should implement the algorithm, which sets and gets the state of the hardware handles.
   * In case of a runtime error, set the #error property.
   * It should always check if the controller is still active (#isAlive method).
   * If the controller got inactive or an error occurred, the method should return.
   *
   * @attention Never use while(true) loops without any sleep methods. They would consume all processing power and slow down FORTE.
   */
  virtual void runLoop() = 0;

  /*! @brief Deinitializes the controller.
   *
   * Stops all initialized hardware libraries.
   * Frees the allocated resources of the controller.
   */
  virtual void deInit() = 0;

  ConfigurationFB::Controller *delegate;

  bool hasError();
  const char* error;

  /*! @brief Notifies the corresponding configuration fb about a startup or runtime event.
   *
   * The method should not be used for time critical operations, such as a write operation of an output fb.
   * You should avoid multiple messages in a short time period, as notifications are not yet queued but dropped.
   *
   * @param type Type of notification
   * @param attachment Attachment of notification. E.g. a const* char pointer should be attached to an Error notification.
   */
  void notifyConfigFB(NotificationType type, const void* attachment = 0);

private:
  void run();

  void setInitDelay(int delay);

  int initDelay;

  // Functions needed for the external event handler interface
  void enableHandler(void) {
  }
  void disableHandler(void) {
  }
  void setPriority(int) {
  }
  int getPriority(void) const {
  }
};

} /* namespace Device */
} /* namespace IO */

#endif /* SRC_STDFBLIB_IO_DEVICE_CONTROLLER_H_ */
