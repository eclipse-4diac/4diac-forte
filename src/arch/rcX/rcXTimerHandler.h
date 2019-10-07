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
    virtual ~CrcXTimerHandler();

    /*!\brief Callback function for the timer.
     *
     */
    static void timerCallback(void* argument);

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
    CrcXTimerHandler();

    RX_HANDLE mTimer;
    bool mFirstTime;

    friend class CTimerHandler;
};



#endif /* SRC_ARCH_RCX_RCXTIMERHANDLER_H_ */
