/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include <memory>
#include <vector>

namespace forte {
  class CFunctionBlock;
  class CEventSourceFB;
  class CExternalEventHandler;
  class CTimerHandler;
  class CDevice;

  /**\ingroup CORE
   Handles all the IEC 61499 execution requests and aspects within one device
  @author az
   */
  class CDeviceExecution {
    public:
      explicit CDeviceExecution(CDevice &paDevice);

      ~CDeviceExecution();

      /*!\brief an external event occurred at an ES and a new event source is to start.
       *
       * In this function the CDeviceExecution does all means necessary that the new event chain will be executed
       * correctly.
       * \param paECStartFB The start FB of the event chain
       */
      void startNewEventChain(CEventSourceFB *paECStartFB) const;

      [[nodiscard]] CTimerHandler &getTimer() const;

      template<typename T>
      T &getExtEvHandler() {
        return static_cast<T &>(*getExtEvHandler(T::scmHandlerIdentifier));
      }

      void disableHandlers();

      CDevice &getDevice();

    private:
      [[nodiscard]] CExternalEventHandler *getExtEvHandler(size_t paIdentifer) const;

      CDevice &mDevice;

      /*!\brief List of currently available external event sources.
       *
       * The element 0 is always the timer event source.
       */
      std::vector<std::unique_ptr<CExternalEventHandler>> mRegisteredEventHandlers;
  };
} // namespace forte
