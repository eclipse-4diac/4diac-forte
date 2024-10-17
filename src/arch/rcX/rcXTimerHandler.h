/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_RCX_RCXTIMERHANDLER_H_
#define SRC_ARCH_RCX_RCXTIMERHANDLER_H_

#include <rX_Includes.h>
#include "../timerha.h"

class CrcXTimerHandler : public CTimerHandler {
  public:

    explicit CrcXTimerHandler(CDeviceExecution& paDeviceExecution);

    ~CrcXTimerHandler() override;

    /*!\brief Callback function for the timer.
     *
     */
    static void timerCallback(void* argument);

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

    RX_HANDLE mTimer;
    bool mFirstTime;
};



#endif /* SRC_ARCH_RCX_RCXTIMERHANDLER_H_ */
