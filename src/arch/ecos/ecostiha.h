/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _ECOSTIMEHA_H_
#define _ECOSTIMEHA_H_

#include <cyg/kernel/kapi.h>
#include "../timerha.h"
#include "forte_thread.h"

/*! \brief External event handler for the eCos Timer.
 * \ingroup ECOS-HAL
 *  
 */
class CECOSTimerHandler : public CTimerHandler, private CThread{
  public:

    explicit CECOSTimerHandler(CDeviceExecution& paDeviceExecution);

    ~CECOSTimerHandler() override;

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

    static cyg_sem_t mSemaphore;

    /*!\brief callback function for the system timer
     */
    static void timerHandlerFunc(cyg_handle_t , cyg_addrword_t ){
      cyg_semaphore_post(&mSemaphore);
    }

    virtual void run();

    cyg_handle_t mAlarmHandle;
    cyg_handle_t mCounterHandle, mSystemclockHandle;
    cyg_alarm mAlarm;
};

#endif /*_ECOSTIMEHA_H_*/
