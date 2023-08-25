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
#ifndef _PCTIMEHA_H_
#define _PCTIMEHA_H_

#include <forte_thread.h>
#include "../timerha.h"

/*! \ingroup posix_hal
 *\ingroup EXTEVHAND
 *\brief the timer handler for the pc architecture.
 */
class CPCTimerHandler : public CTimerHandler, private CThread {
  public:
    ~CPCTimerHandler() override;

    void run() override;

    /*!\brief Enables this event source
     *
     */
    void enableHandler() override;
    /*!\brief Disable this event source
     */
    void disableHandler() override;
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    void setPriority(int paPriority) override;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    int getPriority() const override;
  private:
    explicit CPCTimerHandler(CDeviceExecution& paDeviceExecution);

    friend class CTimerHandler;

};

#endif /*PCTIMEHA_H_*/
