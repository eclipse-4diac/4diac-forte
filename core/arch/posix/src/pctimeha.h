/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/arch/forte_thread.h"
#include "forte/timerha.h"

namespace forte::arch {
  /*! \ingroup posix_hal
   *\ingroup EXTEVHAND
   *\brief the timer handler for the pc architecture.
   */
  class CPCTimerHandler : public CTimerHandler, private CThread {
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
  };
} // namespace forte::arch
