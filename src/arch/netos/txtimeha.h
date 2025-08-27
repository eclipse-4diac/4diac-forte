/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _TXTIMEHA_H_
#define _TXTIMEHA_H_

#include <threadx/tx_api.h>
#include "forte/timerha.h"
#include "../../core/devlog.h"

/*! \ingroup netos_hal
 * \ingroup EXTEVHAND
 * \brief Timerhandler for NET+OS.
 *
 */
class CTXTimerHandler : public CTimerHandler {
  public:
    explicit CTXTimerHandler(CDeviceExecution &paDeviceExecution);

    ~CTXTimerHandler() override;

    /*!\brief Enables this event source
     *
     */
    void enableHandler() override;
    /*!\brief Disable this event source
     */
    void disableHandler() override;

  private:
    /*!\brief callback function for the system timer
     */
    static void timerHandlerFunc(ULONG data) {
      if (data) {
        static_cast<CTXTimerHandler *>(data)->nextTick();
      }
    }

    TX_TIMER mTimer;
};

#endif /*PCTIMEHA_H_*/
