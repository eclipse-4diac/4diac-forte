/*******************************************************************************
 * Copyright (c) 2010, 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/arch/forte_thread.h"
#include "forte/timerha.h"

namespace forte::arch {
  /*! \ingroup win32_hal
   *\ingroup EXTEVHAND
   *\brief the timer handler for the pc architecture.
   */
  class CPCTimerHandler : public CTimerHandler, public CThread {
    private:
      /*!\brief default time base of the used timer. in useconds.
       */
      static const TForteInt32 csmTicksPerSecond;
      /*!\brief callback function for the system timer
       */

    public:
      explicit CPCTimerHandler(CDeviceExecution &paDeviceExecution);

      ~CPCTimerHandler() override;

      void run() override;

      /*!\brief Enables this event source
       *
       */
      void enableHandler() override;
      /*!\brief Disable this event source
       */
      void disableHandler() override;

      /*! \brief Get the time base of the runtime
       *
       * \return internal runtime ticks per millisecond
       */
      virtual TForteUInt32 getTicksPerSecond() {
        return csmTicksPerSecond;
      };
  };
} // namespace forte::arch
