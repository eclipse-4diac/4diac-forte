/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (messmer@fortiss.org)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_STDFBLIB_IO_CONFIGFB_CONTROLLER_H_
#define SRC_STDFBLIB_IO_CONFIGFB_CONTROLLER_H_

#include <io/device/io_controller.h>
#include "io_base.h"

namespace IO {
namespace ConfigurationFB {

/*! @brief Abstract controller configuration fb
 *
 * Base class of the controller configuration fb implementation.
 * All master or board IO configuration fb should inherit from this class or from its subclasses.
 *
 * Features:
 * - handles the basic events of a controller configuration fb (INIT, INITO with the corresponding data outputs QI, QO, and STATUS)
 * - thread safe communication with corresponding device controller (see #handleNotification)
 * - automatic error handling and restart of the Device Controller (see #IO::Device::Controller)
 */
class Controller: public Base {
public:
  Controller(CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~Controller();

protected:

  static const TEventID scm_nEventINITID = 0;

  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventINDID = 1;

  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  }

  virtual void executeEvent(int pa_nEIID);

  //! Describes the current state of the initialization. Use to detect startup or runtime errors.
  bool starting;

  //! Maximum value of the #errorCounter. It sets the amount of retries after an error.
  static int MaxErrors;

  //! Counter for received errors
  int errorCounter;

  //! Instance of the corresponding Device Controller
  Device::Controller *controller;

  /*! @brief Creates an instance of the corresponding Device Controller
   *
   * The method should create an instance of the hardware specific Device Controller.
   * This instance should be set as delegate in the constructor call.
   *
   * @return Instance of the corresponding Device Controller
   */
  virtual Device::Controller* createDeviceController() = 0;

  /*! @brief Sets the configuration of the Device Controller
   *
   * The method should create a local struct of the Device Controller Config and assign the corresponding global data inputs to it.
   * It should call the #IO::Device::Controller::setConfig with a reference to the created config struct.
   */
  virtual void setConfig() = 0;

  /*! @brief Handles the notifications of the Device Controller
   *
   * The method can be overwritten to handle custom notifications between the configuration fb and the Device Controller.
   * @attention Notifications should not be used for time critical operations, such as a write operation of an output fb.
   *
   * @param type Type of the notification
   * @param attachment Reference to the attachment (e.g. a string of an error message)
   * @return True if the notification has been handled. In case it is not handled, a warning message is logged.
   */
  virtual bool handleNotification(Device::Controller::NotificationType type,
      const void* attachment);

  /*! @brief Initializes the configuration fb
   *
   * It is usually called by the INIT+ event.
   * It may also be called after an error and a following recovery attempt.
   *
   * @return True if the initialization was successful
   */
  bool init();

  /*! @brief Deinitializes the configuration fb
   *
   * It is usually called by the INIT- event and after an error.
   *
   * @return True if the deinitialization was successful
   */
  bool deInit();

  /*! @brief Used for further asynchronous initialization operations
   *
   * The method is called after a successful #init call.
   * It is, for example, used for the initialization of attached configuration fbs.
   *
   * @attention The #started method should be called after the asynchronous initialization operations
   */
  virtual void onStartup();

  /*! @brief Confirmation method of the #onStartup method
   *
   * @param error Forward error message in case the startup was not successful. Leads to a re-initialization.
   */
  void started(const char* error = 0);

private:
  void onError(bool isFatal = true);

  static const char * const scmOK;
  static const char * const scmInitializing;
  static const char * const scmFailedToInit;
  static const char * const scmStopped;
};

} /* namespace ConfigurationFB */
} /* namespace IO */

#endif /* SRC_STDFBLIB_IO_CONFIGFB_CONTROLLER_H_ */
