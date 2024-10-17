/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Johannes Kepler University Linz,
 *                          OFFIS e.V.
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
#include "forte_time.h"
#include <vector>

class CEventSourceFB;
class CIEC_TIME;

/*! \brief External event handler for the Timer.
 *  \ingroup EXTEVHAND
 */
class CTimerHandler : public CExternalEventHandler{
  DECLARE_HANDLER(CTimerHandler)
    ;
  public:
    
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    void setPriority(int paPriority) override = 0;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    int getPriority() const override = 0;
    /*! \brief Get the time base of the runtime
     *
     * \return internal runtime ticks per second
     */
    static TForteUInt32 getTicksPerSecond(){
      return cgForteTicksPerSecond;
    }

    /*!\brief Register an Event source Function Block for receiving a single timed event.
     *
     * \param paTimedFB         the event source FB to be registered
     * \param paTimeInterval    delta time to next event
     */
    void registerOneShotTimedFB(CEventSourceFB *const paTimedFB, const CIEC_TIME &paTimeInterval);

    /*!\brief Register an Event source Function Block for receiving a periodic events.
     *
     * \param paTimedFB         the event source FB to be registered
     * \param paTimeInterval    delta time between two events
     */
    void registerPeriodicTimedFB(CEventSourceFB *const paTimedFB, const CIEC_TIME &paTimeInterval);

    /*!\brief  Unregister an FB from an the timmer
     *
     * \param paTimedFB FB to unregister from this external event handler
     */
    void unregisterTimedFB(CEventSourceFB *paTimedFB);

    //! one tick of time elapsed. Implementations should call this function on each tick.
    void nextTick();

    //! returns the time since startup of FORTE
    uint_fast64_t getForteTime() const{
      return mForteTime;
    }

  private:
    //! Data stored for each FB that is registered to the timer handler
    struct STimedFBListEntry{
        CEventSourceFB *mTimedFB; //!< Function block to be triggered
        uint_fast64_t mTimeOut; //!< Absolute time when the next trigger of the FB should occure
        TForteUInt32 mInterval; //!< relative time between FB trigger points (mainly needed for the different periodic timed FBs), 0 means one shot activation

        STimedFBListEntry(CEventSourceFB *paTimedFB, uint_fast64_t paTimeOut, TForteUInt32 paInterval) :
            mTimedFB(paTimedFB), mTimeOut(paTimeOut), mInterval(paInterval){
        }

        STimedFBListEntry() = default;

        bool operator<(const STimedFBListEntry& paRHS) const {
          return mTimeOut < paRHS.mTimeOut;
        }

        bool operator<(uint_fast64_t paRHS)  const {
          return mTimeOut < paRHS;
        }

        friend bool operator<(uint_fast64_t paLHS, const STimedFBListEntry& paRHS) {
          return paLHS < paRHS.mTimeOut;
        }

    };

    static constexpr TForteUInt32 scmOneShotIndicator = 0;

    static_assert(CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond > cgForteTicksPerSecond);
    static constexpr CIEC_TIME::TValueType scmTimeToTimerUnit = CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / cgForteTicksPerSecond;

    void addToAddFBList(const STimedFBListEntry &paTimerListEntry);
    TForteUInt32 convertIntervalToTimerHandlerUnits(const CIEC_TIME &paTimeInterval);

    //!Add an entry to the timed list.
    void addTimedFBEntry(const STimedFBListEntry & paTimerListEntry);

    void processTimedFBList();
    void processAddList();
    void processRemoveList();

    //!Remove an entry from the timed list.
    void removeTimedFB(CEventSourceFB *paTimedFB);

    //! process one timed FB entry, trigger the external event and if needed readd into the list.
    void triggerTimedFB(STimedFBListEntry paTimerListEntry);

    //!The runtime time in ticks till the start of FORTE.
    uint_fast64_t mForteTime;

    //! List of function blocks currently registered to the timer handler
    std::vector<STimedFBListEntry> mTimedFBList;

    //! List of function blocks to be added to the timer handler
    std::vector<STimedFBListEntry> mAddFBList;
    CSyncObject mAddListSync;

    //! List of function blocks to be removed from the timer handler
    std::vector<CEventSourceFB *> mRemoveFBList;
    CSyncObject mRemoveListSync;

};

#endif /*TIMERHA_H_*/
