/*******************************************************************************
 * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 *   2020 TU Wien ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Gunnar Grabmaier, Thomas Strasser, Rene Smodic, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *    Peter Gsellmann - change superclass
 *    Martin Melik Merkumians - move shared methods to abstract base class
 *******************************************************************************/
#ifndef _ECET_H_
#define _ECET_H_

#include "eceta.h"
#include "event.h"
#include "datatypes/forte_time.h"
#include <forte_thread.h>
#include <forte_sync.h>
#include <forte_sem.h>

/*! \ingroup CORE\brief Class for executing one event chain.
 *
 */
class CEventChainExecutionThread : public CEventChainExecutionThreadAbstract {
  public:
    CEventChainExecutionThread();
    virtual ~CEventChainExecutionThread();

    /*!\brief Start the a new event chain with the given event.
     *
     *
     * \param pa_poEventToAdd event of the EC to start
     */
    void startEventChain(SEventEntry *paEventToAdd);

    /*!\brief allow to start, stop, and kill the execution of the event chain execution thread
     *
     * @param pa_unCommand the management command to be executed
     */
    void changeExecutionState(EMGMCommandType paCommand);

    static CEventChainExecutionThread* createEcet();

  protected:

    void mainRun();
    virtual void run(void);

  private:
    /*! \brief Clear the event chain.
     */
    void clear(void);

    bool externalEventOccured() const;

    //! Transfer elements stored in the external event list to the main event list
    void transferExternalEvents();

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
};

#endif /*ECET_H_*/
