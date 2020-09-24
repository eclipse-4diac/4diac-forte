/*******************************************************************************
 * Copyright (c) 2019, 2020 TU Wien ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - inital API for virtual interface for several types of
 *       event chain thread execution mechanisms
 *    Martin Melik Merkumians - move shared methods to abstract base class
 *******************************************************************************/

#ifndef SRC_CORE_ECETA_H_
#define SRC_CORE_ECETA_H_

#include "event.h"
#include "mgmcmd.h"
#include "forte_thread.h"

class CEventChainExecutionThreadAbstract : public CThread {
  public:
    CEventChainExecutionThreadAbstract() :
        CThread(), mSuspendSemaphore(0), mProcessingEvents(false) {
      clearEventList();
    }
    ;
    virtual ~CEventChainExecutionThreadAbstract() {
    }
    ;

    /*!\brief Add an new event entry to the event chain
     *
     * \param paEventToAdd new event entry
     */
    void addEventEntry(SEventEntry *paEventToAdd);
    virtual void changeExecutionState(EMGMCommandType paCommand) = 0;
    virtual void startEventChain(SEventEntry *paEventToAdd) = 0;
    void joinEventChainExecutionThread();
    void resumeSelfSuspend();

    bool isProcessingEvents() const;

    void setDeadline(const CIEC_TIME &paVal);
    void end();

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

    /**
     * @brief self suspend the Event-Chain Execution Thread
     */
    void selfSuspend();

    /**
     * @brief clears the event list
     */
    void clearEventList();

    /**
     * @brief clear all event lists available
     */
    virtual void clear(void) = 0;

    /*! \brief The thread run()-method where the events are sent to the FBs and the FBs are executed in.
     *
     * If there is an entry in the Event List the event will be delivered and the FB executed.
     * If there is no entry in this list the CEventChainExecutionThread61131 will suspend itself and remove itself from the
     * active EventChainExecutionlist in CFBNExecution.
     * \return true if thread execution ended successfully
     */
    virtual void run(void) = 0;

};

#endif /* SRC_CORE_ECETA_H_ */
