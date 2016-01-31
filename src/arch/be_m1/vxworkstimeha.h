/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _PCTIMEHA_H_
#define _PCTIMEHA_H_

#include "thread.h"
#include "../timerha.h"

/*! \ingroup bem1_hal
 *\ingroup EXTEVHAND
 *\brief the timer handler for the Bachmann electronic M1 architecture.
 */
class CPCTimerHandler : public CTimerHandler, private CThread{
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
    CPCTimerHandler();

    friend class CTimerHandler;

};

#endif /*PCTIMEHA_H_*/
