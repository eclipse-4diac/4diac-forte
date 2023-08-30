/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Christoph Binder - initial implementation
 *******************************************************************************/

#include "forte_thread.h"
#include "timerha.h"
#include "funcbloc.h"

/*! \brief the fake timer handler for testing.
 *
 */
class CFakeTimerHandler : public CTimerHandler, public CThread {

  public:
    explicit CFakeTimerHandler(CDeviceExecution &paDeviceExecution);

    virtual ~CFakeTimerHandler();

    virtual void run();

    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler(void) override;
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void) override;
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    virtual void setPriority(int paPriority) override;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const override;

    void setSleepTime(CIEC_TIME&, CFunctionBlock *fb);

  private:
    CEventChainExecutionThread* getExecThread();
    void startOutputEvent();
    uint_fast64_t sleepTime;
    CFunctionBlock *fakeSleepFb;
};
