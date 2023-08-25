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
    ~CFreeRTOSTimerHandler() override;

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

    explicit CFreeRTOSTimerHandler(CDeviceExecution& paDeviceExecution);

    static void vCallbackFunction(TimerHandle_t paTimer);

    TimerHandle_t mTimer;

    friend class CTimerHandler;
};

#endif /* SRC_ARCH_FREERTOS_FREERTOSTIHA_H_ */
