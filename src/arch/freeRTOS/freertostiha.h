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

#ifndef SRC_ARCH_FREERTOS_FREERTOSTIHA_H_
#define SRC_ARCH_FREERTOS_FREERTOSTIHA_H_

#include <FreeRTOS.h>
#include <timers.h>

#include "../timerha.h"

/*! \brief External event handler for the FreeRTOS Timer.
 * \ingroup FreeRTOS-HAL
 *
 */
class CFreeRTOSTimerHandler : public CTimerHandler {
  public:
    virtual ~CFreeRTOSTimerHandler();

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
    virtual void setPriority(int paPriority);
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const;

  private:

    explicit CFreeRTOSTimerHandler(CDeviceExecution& paDeviceExecution);

    static void vCallbackFunction(TimerHandle_t paTimer);

    TimerHandle_t mTimer;

    friend class CTimerHandler;
};

#endif /* SRC_ARCH_FREERTOS_FREERTOSTIHA_H_ */
