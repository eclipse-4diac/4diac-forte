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

#ifndef SRC_CORE_IO_DEVICE_CONTROLLER_H_
#define SRC_CORE_IO_DEVICE_CONTROLLER_H_

#include <extevhan.h>
#include <forte_sync.h>
#include <forte_thread.h>
#include <devlog.h>
#include <fortelist.h>

#include <io/mapper/io_handle.h>

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBController;

      /*! @brief Abstract Device Controller
       *
       * Base class of the device controller implementation.
       * All hardware specific IO implementations should inherit from this class.
       *
       * Features:
       * - runs Device Controller in separate thread
       * - thread safe communication with corresponding controller configuration fb (see #notifyConfigFB)
       * - base structure for safe startups and resets (see #init, #runLoop, and #deInit)
       * - automatic error handling and restart (see #IOConfigFBController)
       */
      class IODeviceController : protected CExternalEventHandler, protected CThread {
          friend class IOConfigFBController;

        public:
          /*! @brief Abstract configuration struct.
           *
           * The child controller should implement an own configuration struct, which inherits from this struct and is initialized with default parameters.
           * The controller configuration fb uses the #setConfig method to set the runtime configuration.
           */
          struct Config {

          };

          /*! @brief Abstract descriptor of device handles.
           *
           * Used to exchange information about handles between the device controller and the corresponding configuration fb.
           * The device implementation should extend the struct with properties, which uniquely identify the handle.
           * The #initHandle method then creates an #IOHandle instance based on the handle descriptor.
           */
          class HandleDescriptor {
            public:
              CIEC_WSTRING const &mId;
              IOMapper::Direction mDirection;

              HandleDescriptor(CIEC_WSTRING const &paId, IOMapper::Direction paDirection) :
                  mId(paId), mDirection(paDirection) {

              }
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
          NotificationType mNotificationType;
          const void* mNotificationAttachment;
          bool mNotificationHandled;

          /*! @brief Sets the configuration of the controller
           *
           * @param paConfig Pointer to the configuration struct provided by the corresponding controller configuration fb. Use *static_cast<CustomConfig*>(config) to retrieve the struct instance.
           */
          virtual void setConfig(Config* paConfig) = 0;

          void fireIndicationEvent(IOObserver *paObserver);

          virtual void handleChangeEvent(IOHandle *paHandle);

          //TODO: adapt this properly to the new handler model. This mockup is just to avoid the classes below to be abstract
          virtual size_t getIdentifier() const {
            return 0;
          }

        protected:
          explicit IODeviceController(CDeviceExecution& paDeviceExecution);

          /*! @brief Initializes the controller.
           *
           * The method should initialize and startup the controller.
           * All required hardware libraries should be started and checked for errors.
           * If the system requires an initialization sequence which is executed once at startup, place it here.
           * In case of an error, return an error description.
           *
           * @return 0/NULL if the initialization was successful. Otherwise return an error description.
           */
          virtual const char* init() = 0;

          /*! @brief Contains the blocking run sequence.
           *
           * The method should implement the algorithm, which sets and gets the state of the hardware handles.
           * In case of a runtime error, set the #error property.
           * It should always check if the controller is still active (#isAlive method).
           * If the controller got inactive or an error occurred, the method should return.
           *
           * @attention Never use While(true) loops without any sleep methods. They would consume all processing power and slow down FORTE.
           */
          virtual void runLoop() = 0;

          /*! @brief Deinitializes the controller.
           *
           * Stops all initialized hardware libraries.
           * Frees the allocated resources of the controller.
           */
          virtual void deInit() = 0;

          bool hasError() const;
          const char* mError;

          /*! @brief Notifies the corresponding configuration fb about a startup or runtime event.
           *
           * The method should not be used for time critical operations, such as a write operation of an output fb.
           * You should avoid multiple messages in a short time period, as notifications are not yet queued but dropped.
           *
           * @param paType Type of notification
           * @param paAttachment Attachment of notification. E.g. a const* char pointer should be attached to an Error notification.
           */
          void notifyConfigFB(NotificationType paType, const void* paAttachment = 0);

          typedef CSinglyLinkedList<IOHandle *> THandleList;

          /*! @brief Adds an IO handle to the controller
           *
           * This method is called by the corresponding configuration function block during the initialization.
           * The handle is automatically added to the correct list (#inputHandles and #outputHandles).
           * The handle lists should be accessed by the controller to read and write IOs.
           *
           * @param paHandleDescriptor Descriptor of the handle
           */
          virtual void addHandle(HandleDescriptor *paHandleDescriptor);

          /*! @brief Initializer for all IO handles.
           *
           * The method is called when a configuration fb adds an handle via the #addHandle method.
           * It should return an handle instance based on the descriptor information.
           *
           * @param paHandleDescriptor Descriptor of the handle
           */
          virtual IOHandle* initHandle(HandleDescriptor *paHandleDescriptor) = 0;

          /*! @brief Iterates over all input handles and fires an indication event in case of a change.
           *
           * The method iterates over the #inputHandles list.
           * It checks if the input handle has a bound observer and then calls the #isHandleValueEqual method.
           * If the #isHandleValueEqual returns true, the indication event is fired for the bound observer.
           * @attention The method does only work if the #isHandleValueEqual is overridden.
           */
          void checkForInputChanges();

          /*! @brief Checks if the value of a handle has changed. Used by the #checkForInputChanges method.
           *
           * @param paHandle IOHandle which should be compared to the previous IO state
           * @return True if the current state is equal to the previous IO state. In case it has changed, return false.
           */
          virtual bool isHandleValueEqual(IOHandle* paHandle);

          /*! @brief Synchronizes the access to the #inputHandles and #outputHandles. Use it for iterations over the lists. */
          CSyncObject mHandleMutex;

          /*! @brief All input handles of the main controller
           * The list should only contain input handles of the main controller.
           * The list is managed by the #addHandle and #dropHandles method.
           * #dropHandles is called automatically during deinitialization.
           */
          THandleList mInputHandles;

          /*! @brief All output handles of the main controller
           * The list should only contain output handles of the main controller.
           * The list is managed by the #addHandle and #dropHandles method.
           * #dropHandles is called automatically during deinitialization.
           */
          THandleList mOutputHandles;

        private:

          IOConfigFBController *mDelegate;

          /*! @brief Drops all handle instances which were previously added by the #addHandle method
           *
           * The method is automatically called during the deinitialization of the corresponding configuration function block.
           */
          void dropHandles();

          void run();

          void setInitDelay(int paDelay);

          int mInitDelay;

          void addHandle(THandleList* paList, CIEC_WSTRING const &paId, IOHandle* paHandle);

          // Functions needed for the external event handler interface
          void enableHandler(void) {
            //do nothing
          }
          void disableHandler(void) {
            //do nothing
          }
          void setPriority(int) {
            //do nothing
          }
          int getPriority(void) const {
            return 0;
          }
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_DEVICE_CONTROLLER_H_ */
