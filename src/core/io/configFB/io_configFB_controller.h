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

#ifndef SRC_CORE_IO_CONFIGFB_CONTROLLER_H_
#define SRC_CORE_IO_CONFIGFB_CONTROLLER_H_

#include <io/device/io_controller.h>
#include "io_base.h"

namespace forte {
  namespace core {
    namespace io {

      /*! @brief Abstract controller configuration fb
       *
       * Base class of the controller configuration fb implementation.
       * All master or board IO configuration fb should inherit from this class or from its subclasses.
       *
       * Features:
       * - handles the basic events of a controller configuration fb (INIT, INITO with the corresponding data outputs QI, QO, and STATUS)
       * - thread safe communication with corresponding device controller (see #handleNotification)
       * - automatic error handling and restart of the Device Controller (see #IODeviceController)
       */
      class IOConfigFBController : public IOConfigFBBase {
        public:
          IOConfigFBController(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
              TForteByte *paFBConnData, TForteByte *paFBVarsData);
          virtual ~IOConfigFBController();

          IODeviceController* getDeviceController() {
            return mController;
          }

        protected:

          static const TEventID scmEventINITID = 0;

          CIEC_BOOL &QI() {
            return *static_cast<CIEC_BOOL*>(getDI(0));
          }

          static const TEventID scmEventINITOID = 0;
          static const TEventID scmEventINDID = 1;

          CIEC_BOOL &QO() {
            return *static_cast<CIEC_BOOL*>(getDO(0));
          }

          CIEC_WSTRING &STATUS() {
            return *static_cast<CIEC_WSTRING*>(getDO(1));
          }

          virtual void executeEvent(int paEIID);

          /*! @brief Creates an instance of the corresponding Device Controller
           *
           * The method should create an instance of the hardware specific Device Controller.
           * This instance should be set as delegate in the constructor call.
           *
           * @param paDeviceExecution Device execution needed for the external handler
           * @return Instance of the corresponding Device Controller
           */
          virtual IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution) = 0;

          /*! @brief Sets the configuration of the Device Controller
           *
           * The method should create a local struct of the Device Controller Config and assign the corresponding global data inputs to it.
           * It should call the #IODeviceController::setConfig with a reference to the created config struct.
           */
          virtual void setConfig() = 0;

          /*! @brief Handles the notifications of the Device Controller
           *
           * The method can be overwritten to handle custom notifications between the configuration fb and the Device Controller.
           * @attention Notifications should not be used for time critical operations, such as a write operation of an output fb.
           *
           * @param paType Type of the notification
           * @param paAttachment Reference to the attachment (e.g. a string of an error message)
           * @return True if the notification has been handled. In case it is not handled, a warning message is logged.
           */
          virtual bool handleNotification(IODeviceController::NotificationType paType, const void* paAttachment);

          /*! @brief Initializes the configuration fb
           *
           * It is usually called by the INIT+ event.
           * It may also be called after an error and a following recovery attempt.
           *
           * @param paDelay Delay the initialization in seconds
           * @return True if the initialization was successful
           */
          bool init(int paDelay = 0);

          /*! @brief Initializes an IO handle
           *
           * This method is used to forward handle descriptors to the device controller.
           * The #IODeviceController::addHandle method is called and in case the #IODeviceController::HandleDescriptor::id
           * is not empty, the handle is initialized with the #IODeviceController::initHandle method.
           *
           * @param paHandleDescriptor Descriptor of the handle
           */
          void initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor);

          /*! @brief Deinitializes the configuration fb
           *
           * It is usually called by the INIT- event and after an error.
           *
           * @param paIsDestructing True if the deInit is called inside the destructor
           * @return True if the deinitialization was successful
           */
          bool deInit(bool paIsDestructing = false);

          /*! @brief Used for asynchronous initialization operations
           *
           * The method is called after a successful #init call.
           * It is, for example, used for the initialization of attached configuration fbs.
           *
           * @attention The #started method should be called after the asynchronous initialization operations
           */
          virtual void onStartup();

          /*! @brief Confirmation method of the #onStartup method
           *
           * @param paError Forward error message in case the startup was not successful. Leads to a re-initialization.
           */
          void started(const char* paError = 0);

          /*! @brief Used for asynchronous deinitialization operations
           *
           * The method is called after the #deInit function, in case the fb is not destructed.
           * It is, for example, used for the deinitialization of attached configuration fbs.
           *
           * @attention The #stopped method should be called after the deinitialization operations.
           */
          virtual void onStop();

          /*! @brief Confirmation method of the #onStop method */
          void stopped();

        private:

          //! Describes the current state of the initialization. Use to detect startup or runtime errors.
          bool mStarting;

          //! Maximum value of the #errorCounter. It sets the amount of retries after an error.
          static int smMaxErrors;

          //! Counter for received errors
          int mErrorCounter;

          //! Instance of the corresponding Device Controller
          IODeviceController *mController;

          void onError(bool paIsFatal = true);

          bool mPerformRestart;

          static const char * const scmOK;
          static const char * const scmInitializing;
          static const char * const scmFailedToInit;
          static const char * const scmStopped;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_CONTROLLER_H_ */
