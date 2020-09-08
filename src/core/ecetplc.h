/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *******************************************************************************/
#ifndef _ECET61131_H_
#define _ECET61131_H_

#include "eceta.h"
#include "eventconn.h"
#include "datatypes/forte_time.h"
#include <forte_sync.h>
#include <forte_sem.h>
#include <forte_thread.h>

class CCycleTimer;

/*! \ingroup CORE\brief Class for executing one event chain.
 *
 */
class CEventChainExecutionThread61131 : public CEventChainExecutionThreadAbstract, public CThread {
  public:
    CEventChainExecutionThread61131();
    virtual ~CEventChainExecutionThread61131();

    /*!\brief Start the a new event chain with the given event.
     *
     *
     * \param pa_poEventToAdd event of the EC to start
     */
    void startEventChain(SEventEntry *paEventToAdd);

    /*!\brief Add an new event entry to the event chain
     *
     * \param pa_poEventToAdd new event entry
     */
    void addEventEntry(SEventEntry *paEventToAdd);

    void addInitEventEntry(SEventEntry *paEventToAdd);

    void setDeadline(const CIEC_TIME &paVal);

    bool isProcessingEvents() const;

    virtual void end(void);

    /*!\brief allow to start, stop, and kill the execution of the event chain execution thread
     *
     * @param pa_unCommand the management command to be executed
     */
    void changeExecutionState(EMGMCommandType paCommand);

    void joinEventChainExecutionThread();

    bool getExeDone();

    void setCycleTime(TForteUInt16 time_ms);

    void resumeSelfSuspend();

  protected:
    //@{
    /*! \brief List of input events to deliver.
     *
     * This list stores the necessary information for all events to deliver that occurred within this event chain.
     */

    TEventEntryPtr mEventList[cg_nEventChainEventListSize];
    TEventEntryPtr *mEventListStart;
    TEventEntryPtr *mEventListEnd;
    //@}

    //@{
    /*! \brief List of events for initialization to deliver.
     *
     * This list stores the necessary information for all events to deliver that are needed for initialization.
     */

    TEventEntryPtr mInitEventList[cg_nEventChainEventListSize];
    TEventEntryPtr *mInitEventListStart;
    TEventEntryPtr *mInitEventListEnd;
    //@}

  private:
    /*! \brief The thread run()-method where the events are sent to the FBs and the FBs are executed in.
     *
     * If there is an entry in the Event List the event will be delivered and the FB executed.
     * If there is no entry in this list the CEventChainExecutionThread61131 will suspend itself and remove itself from the
     * active EventChainExecutionlist in CFBNExecution.
     * \return true if thread execution ended successfully
     */
    virtual void run(void);

    TForteUInt16 cycle_time;
    /*! \brief Clear the event chain.
     */
    void clear(void);

    void selfSuspend();

    //@{
    /*! \brief List of external events that occurred during one FB's execution
     *
     * This list stores external events that may have occurred during the execution of a FB or during when the
     * Event-Chain execution was sleeping. with this second list we omit the need for a mutex protection of the event
     * list. This is a great performance gain.
     */
    TEventEntryPtr mExternalEventList[cg_nEventChainExternalEventListSize];
    TEventEntryPtr *mExternalEventListStart;
    TEventEntryPtr *mExternalEventListEnd;
    //@}

    //! SyncObject for protecting the list in regard to several accesses
    CSyncObject mExternalEventListSync;

    forte::arch::CSemaphore mSuspendSemaphore;

    /*! \brief Flag indicating if this event chain execution thread is currently processing any events
     *
     * Initially this flag is false.
     * This flag is activated when a new event chain is started and deactivated when the event queue is empty.
     *
     * Currently this flag is only needed for the FB tester.
     * TODO consider surrounding the usage points of this flag with #defines such that it is only used for testing.
     */
    bool mProcessingEvents;

    bool exeDone;
};

#endif /*ECET61131_H_*/
