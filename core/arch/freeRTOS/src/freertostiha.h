/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include "forte/timerha.h"

namespace forte::arch {
  /*! \brief External event handler for the FreeRTOS Timer.
   * \ingroup FreeRTOS-HAL
   *
   */
  class CFreeRTOSTimerHandler : public CTimerHandler {
    public:
      explicit CFreeRTOSTimerHandler(CDeviceExecution &paDeviceExecution);

      ~CFreeRTOSTimerHandler() override;

      /*!\brief Enables this event source
       *
       */
      void enableHandler() override;
      /*!\brief Disable this event source
       */
      void disableHandler() override;

    private:
      static void vCallbackFunction(TimerHandle_t paTimer);

      TimerHandle_t mTimer;
  };
} // namespace forte::arch
