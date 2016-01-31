/*******************************************************************************
  * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Gunnar Grabmaier, Thomas Strasser, Rene Smodic, Ingo Hegny
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ECET_H_
#define _ECET_H_

#include "eventconn.h"
#include "event.h"
#include "datatypes/forte_time.h"
#include <thread.h>
#include <sync.h>

class CEventSourceFB;

/*! \ingroup CORE\brief Class for executing one event chain.
 *
 */
class CEventChainExecutionThread : public CThread{
  public:
    CEventChainExecutionThread();
    virtual ~CEventChainExecutionThread();

    /*!\brief Start the a new event chain with the given event.
     *
     *
     * \param pa_poEventToAdd event of the EC to start
     */
    void startEventChain(SEventEntry *pa_poEventToAdd);

    /*!\brief Add an new event entry to the event chain
     *
     * \param pa_poEventToAdd new event entry
     */
    void addEventEntry(SEventEntry *pa_poEventToAdd);

    /*!\brief allow to start, stop, and kill the execution of the event chain execution thread
     *
     * @param pa_unCommand the management command to be executed
     */
    void changeExecutionState(EMGMCommandType pa_unCommand);

    void joinEventChainExecutionThread(){
      CThread::join();
    }

    void setDeadline(const CIEC_TIME &pa_roVal){
      CThread::setDeadline(pa_roVal);
    }

    bool isProcessingEvents() const {
      return mProcessingEvents;
    }

  protected:
    //@{
    /*! \brief List of input events to deliver.
     *
     * This list stores the necessary information for all events to deliver that occurred within this event chain.
     */

    TEventEntryPtr m_astEventList[cg_nEventChainEventListSize];
    TEventEntryPtr* m_pstEventListStart;
    TEventEntryPtr* m_pstEventListEnd;
    //@}

  private:
    /*! \brief The thread run()-method where the events are sent to the FBs and the FBs are executed in.
     *
     * If there is an entry in the Event List the event will be delivered and the FB executed.
     * If there is no entry in this list the CEventChainExecutionThread will suspend itself and remove itself from the
     * active EventChainExecutionlist in CFBNExecution.
     * \return true if thread execution ended successfully
     */
    virtual void run(void);

    /*! \brief Clear the event chain.
     */
    void clear(void);

    bool externalEventOccured(){
      /* we should not need a protection here as we are just comparing the both
       * the only value that could change during the compare is m_pstExternalEventListEnd. In the worst case
       * this results in a to early or to late detection. However as the transfer is protected this should be no
       * big issue.
       * TODO perform test to verify this assumption
       */
      return (m_pstExternalEventListStart != m_pstExternalEventListEnd);
    }

    //! Transfer elements stored in the external event list to the main event list
    void transferExternalEvents();

    //@{
    /*! \brief List of external events that occurred during one FB's execution
     *
     * This list stores external events that may have occurred during the execution of a FB or during when the
     * Event-Chain execution was sleeping. with this second list we omit the need for a mutex protection of the event
     * list. This is a great performance gain.
     */
    TEventEntryPtr m_astExternalEventList[cg_nEventChainExternalEventListSize];
    TEventEntryPtr* m_pstExternalEventListStart;
    TEventEntryPtr* m_pstExternalEventListEnd;
    //@}

    //! SyncObject for protecting the list in regard to several accesses
    CSyncObject m_oExternalEventListSync;

    /*! \brief Flag indicating if this event chain execution thread is currently processing any events
     *
     * Initially this flag is false.
     * This flag is activated when a new event chain is started and deactivated when the event queue is empty.
     *
     * Currently this flag is only needed for the FB tester.
     * TODO consider surrounding the usage points of this flag with #defines such that it is only used for testing.
     */
    bool mProcessingEvents;
};

#endif /*ECET_H_*/
