/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FMITIMERHANDLER_H_
#define _FMITIMERHANDLER_H_

#include "../../arch/timerha.h"
#include "../ecet.h"
#include "../datatypes/forte_lreal.h"
#include <vector>

/*!\brief Timer handler for FMI/FMU
 *
 */
class fmiTimerHandler : public CTimerHandler{
  public:

    explicit fmiTimerHandler(CDeviceExecution& pa_poDeviceExecution);

    virtual ~fmiTimerHandler();

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

    /*!\brief Advance the ticks according of the time
     */
    bool advanceTicks(CIEC_LREAL& time);

    void addExecutionThread(CEventChainExecutionThread* pa_executionThread){
      eventChainExecutions.push_back(pa_executionThread);
    }

  private:
    std::vector<CEventChainExecutionThread*> eventChainExecutions;

#ifdef FMU_DEBUG
    SForteTime m_stForteTimeFMI;
#endif
};

#endif /* _FMITIMERHANDLER_H_ */
