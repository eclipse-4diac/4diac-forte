/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *               2020 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Monika Wenger, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *   Alois Zoitl - worked on reducing the jitter and overhead of timer handler
 *                 Bug #568902
 *******************************************************************************/
#ifndef _TIMERHA_H_
#define _TIMERHA_H_

#include <forte_config.h>
#include "../core/extevhan.h"
#include <forte_sync.h>
#include <vector>

class CEventSourceFB;
class CIEC_TIME;

enum ETimerActivationType{
  e_SingleShot, e_Periodic
};

//! Data stored for each FB that is registered to the timer handler
struct STimedFBListEntry{
    uint_fast64_t mTimeOut; //!< absolut time when the next trigger of the FB should occure
    CEventSourceFB *mTimedFB; //!< Functionblock to be triggered
    TForteUInt32 mInterval; //!< relative time between FB trigger points (mainly needed for the different periodic timed FBs)
    ETimerActivationType mType; //!< type of activation. e.g. singleshot, periodic, ...
    STimedFBListEntry *mNext; //!< pointer to the next entry in the list
};

/*! \brief External event handler for the Timer.
 *  \ingroup EXTEVHAND
 */
class CTimerHandler : public CExternalEventHandler{
  DECLARE_HANDLER(CTimerHandler)
    ;
  public:

    /*!\brief create the timer handler and set the parameter pointer with the the new timer handler.
     *
     * This function is not implemented in the standardtimerhandler and has to be implemented in the specific implementation.
     * implementations should check that not two timerhanlders can be created.
     */
    static CTimerHandler* createTimerHandler(CDeviceExecution &paDeviceExecution);

    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int paPriority) = 0;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const = 0;

    /*! \brief Get the time base of the runtime
     *
     * \return internal runtime ticks per second
     */
    static TForteUInt32 getTicksPerSecond(void){
      return cg_nForteTicksPerSecond;
    }

    /*!\brief Register an Event source Functionblock to this external event source.
     *
     * \param paTimerListEntry   TimerListEntry data
     * \param paTimeInterval      time interval to next event
     */
    void registerTimedFB(STimedFBListEntry *paTimerListEntry, const CIEC_TIME &paTimeInterval);

    /*!\brief  Unregister an FB from an the timmer
     *
     * \param paTimedFB FB to unregister from this external event handler
     */
    void unregisterTimedFB(CEventSourceFB *paTimedFB);

    //! one tick of time elapsed. Implementations should call this function on each tick.
    void nextTick(void);

    //! returns the time since startup of FORTE
    uint_fast64_t getForteTime() const{
      return mForteTime;
    }

  private:
    //!Add an entry to the timed list.
    void addTimedFBEntry(STimedFBListEntry *paTimerListEntry);

    void processTimedFBList();
    void processAddList();
    void processRemoveList();

    //!Remove an entry from the timed list.
    void removeTimedFB(CEventSourceFB *paTimedFB);

    //! process one timed FB entry, trigger the external event and if needed readd into the list.
    void triggerTimedFB(STimedFBListEntry *paTimerListEntry);

    //!The runtime time in ticks till the start of FORTE.
    uint_fast64_t mForteTime;

    //! List of function blocks currently registered to the timer handler
    STimedFBListEntry *mTimedFBList;

    //! List of function blocks to be added to the timer handler
    STimedFBListEntry *mAddFBList;
    CSyncObject mAddListSync;

    //! List of function blocks to be removed from the timer handler
    std::vector<CEventSourceFB *> mRemoveFBList;
    CSyncObject mRemoveListSync;

};

#endif /*TIMERHA_H_*/
