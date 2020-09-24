/*******************************************************************************
 * Copyright (c) 2020 TU Wien ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *   Martin Melik Merkumians - move shared methods to abstract base class
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
class CEventChainExecutionThread61131 : public CEventChainExecutionThreadAbstract {
  public:
    CEventChainExecutionThread61131();
    virtual ~CEventChainExecutionThread61131();

    /*!\brief Start the a new event chain with the given event.
     *
     *
     * \param pa_poEventToAdd event of the EC to start
     */
    virtual void startEventChain(SEventEntry *paEventToAdd);

    void addInitEventEntry(SEventEntry *paEventToAdd);

    /*!\brief allow to start, stop, and kill the execution of the event chain execution thread
     *
     * @param pa_unCommand the management command to be executed
     */
    virtual void changeExecutionState(EMGMCommandType paCommand);

    bool getExeDone();

    void setCycleTime(TForteUInt16 time_ms);

  protected:

    //@{
    /*! \brief List of events for initialization to deliver.
     *
     * This list stores the necessary information for all events to deliver that are needed for initialization.
     */

    TEventEntryPtr mInitEventList[cg_nEventChainEventListSize];
    TEventEntryPtr *mInitEventListStart;
    TEventEntryPtr *mInitEventListEnd;
    //@}

    virtual void run(void);

  private:

    TForteUInt16 cycle_time;
    /*! \brief Clear the event chain.
     */
    void clear(void);

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

    bool exeDone;
};

#endif /*ECET61131_H_*/
