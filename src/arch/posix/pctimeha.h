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
    virtual ~CPCTimerHandler();

    virtual void run();

    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler(void);
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void);
    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int pa_nPriority);
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const;
  private:
    explicit CPCTimerHandler(CDeviceExecution& pa_poDeviceExecution);

    friend class CTimerHandler;

};

#endif /*PCTIMEHA_H_*/
