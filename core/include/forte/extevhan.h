/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>
#include <concepts>

namespace forte {
  class CDeviceExecution;
  class CEventSourceFB;
  class CFunctionBlock;

  /**  \defgroup FORTE_HAL FORTE Hardware Abstraction Layer - FORTE-HAL
   * \brief The FORTE-HAL is the abstraction of HW dependent features important
   * and needed in each port of FORTE.
   */
  /*@{*/

  /**\defgroup EXTEVHAND External Event Handling
   */
  /*@{*/

  template<typename T>
  class RegisterExternalEventHandler;

  /*! \brief Baseclass for handling incoming interrupts and similar external events.
   *
   * Implementations of such classes should provide the following functions for ES-FBs:
   *   - registerFB(CFunctionBlock *paESFB, ...) allows the ES-FB to register to this ExternalEventHandler for recieving
   * external events (e.g. INIT+). the specific parameters depend on the ExternalEventHandler.
   *   - unregisterFB(CFunctionBlock *paESFB) the ES-FB doesn't want to receive any external events any more (e.g.
   * INIT-).
   */

  class CExternalEventHandler {
      template<typename T>
      friend class RegisterExternalEventHandler;

    public:
      explicit CExternalEventHandler(CDeviceExecution &paDeviceExecution);

      virtual ~CExternalEventHandler() = default;
      /*!\brief Enables this event source
       *
       */
      virtual void enableHandler() = 0;
      /*!\brief Disable this event source
       */
      virtual void disableHandler() = 0;

      [[nodiscard]] static const std::vector<
          std::function<std::unique_ptr<CExternalEventHandler>(CDeviceExecution &)>> &
      getFactories() {
        return smEventHandlerFactories;
      }

    protected:
      /*!\brief register event source at device execution for starting a new event chain
       *
       * this function checks if the external event handler is allowed to start new event chains and if yes performs the
       * necessary actions.
       *
       * @param paECStartFB the event source function block which starts the new event chain
       */
      void startNewEventChain(CEventSourceFB *paECStartFB);

      CDeviceExecution &mDeviceExecution;

    private:
      template<typename T>
        requires std::derived_from<T, CExternalEventHandler> && std::derived_from<T, RegisterExternalEventHandler<T>>
      static std::size_t registerExtEvHandler() {
        smEventHandlerFactories.emplace_back(T::create);
        // return values start at 1, since index 0 is the timer handler
        return smEventHandlerFactories.size();
      }

      constinit static std::vector<std::function<std::unique_ptr<CExternalEventHandler>(CDeviceExecution &)>>
          smEventHandlerFactories;
  };

  template<typename T>
  class RegisterExternalEventHandler {
      friend CExternalEventHandler;

    public:
      RegisterExternalEventHandler() {
        (void) &scmHandlerIdentifier;
      }

      static const std::size_t scmHandlerIdentifier;

    private:
      static std::unique_ptr<CExternalEventHandler> create(CDeviceExecution &paDeviceExecution) {
        return std::make_unique<T>(paDeviceExecution);
      }
  };

  template<typename T>
  const size_t RegisterExternalEventHandler<T>::scmHandlerIdentifier = CExternalEventHandler::registerExtEvHandler<T>();

  /*@}*/
  /*@}*/
} // namespace forte
