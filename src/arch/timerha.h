/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Monika Wenger, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _TIMERHA_H_
#define _TIMERHA_H_

#include <forte_config.h>
#include "../core/extevhan.h"
#include <forte_sync.h>

class CEventSourceFB;
class CIEC_TIME;

enum ETimerActivationType{
  e_SingleShot, e_Periodic
};

struct SForteTime{
    TForteUInt32 m_nLowerValue;
    TForteUInt32 m_nUpperValue;

    SForteTime(){
      m_nLowerValue = m_nUpperValue = 0;
    }

    SForteTime &operator++(){ //++i;
      ++m_nLowerValue;
      if(0 == m_nLowerValue)
        ++m_nUpperValue;
      return *this;
    }
    SForteTime& operator=(const SForteTime &pa_stRightVal){
      m_nLowerValue = pa_stRightVal.m_nLowerValue;
      m_nUpperValue = pa_stRightVal.m_nUpperValue;
      return *this;
    }

    SForteTime& operator+=(TForteUInt32 pa_nRightVal){
      TForteUInt32 nBuf = m_nLowerValue;
      m_nLowerValue += pa_nRightVal;
      if(nBuf > m_nLowerValue)
        ++m_nUpperValue;
      return *this;
    }

    SForteTime& operator-=(TForteUInt32 pa_nRightVal){
      TForteUInt32 nBuf = m_nLowerValue;
      m_nLowerValue -= pa_nRightVal;
      if(nBuf < m_nLowerValue)
        --m_nUpperValue;
      return *this;
    }

    SForteTime& operator-=(const SForteTime& pa_nRightVal){
      m_nUpperValue -= pa_nRightVal.m_nUpperValue;
      if(m_nLowerValue < pa_nRightVal.m_nLowerValue){
        --m_nUpperValue;
        --m_nLowerValue;
      }
      m_nLowerValue -= pa_nRightVal.m_nLowerValue;
      return *this;
    }

    bool operator>(const SForteTime &pa_stRightVal) const{
      bool bRetVal = false;
      if((m_nUpperValue > pa_stRightVal.m_nUpperValue) || ((m_nUpperValue == pa_stRightVal.m_nUpperValue) && (m_nLowerValue > pa_stRightVal.m_nLowerValue))){
        bRetVal = true;
      }
      return bRetVal;
    }

    bool operator==(const SForteTime &pa_stRightVal) const{
      return m_nUpperValue == pa_stRightVal.m_nUpperValue && m_nLowerValue == pa_stRightVal.m_nLowerValue;
    }
};

inline const SForteTime operator-(const SForteTime &pa_nLeftVal, const SForteTime &pa_nRightVal){
  SForteTime oRetVal(pa_nLeftVal);
  oRetVal -= pa_nRightVal;
  return oRetVal;
}

//! Data stored for each FB that is registered to the timer handler
struct STimedFBListEntry{
    SForteTime m_stTimeOut; //!< absolut time when the next trigger of the FB should occure
    CEventSourceFB *m_poTimedFB; //!< Functionblock to be triggered
    TForteUInt32 m_nInterval; //!< relative time between FB trigger points (mainly needed for the different periodic timed FBs)
    ETimerActivationType m_eType; //!< type of activation. e.g. singleshot, periodic, ...
    STimedFBListEntry *m_pstNext; //!< pointer to the next entry in the list
};

/*! \brief External event handler for the Timer.
 *  \ingroup EXTEVHAND
 */
class CTimerHandler : public CExternalEventHandler{
  DECLARE_HANDLER(CTimerHandler);
  public:

    /*!\brief Pointer to a general timer used for code outside FBs, like devlog, TIME() and so on
     */
    static CTimerHandler *sm_poFORTETimer;
    /*!\brief create the timer handler and set the parameter pointer with the the new timer handler.
     *
     * This function is not implemented in the standardtimerhandler and has to be implemented in the specific implementation.
     * implementations should check that not two timerhanlders can be created.
     */
    static CTimerHandler* createTimerHandler(CDeviceExecution& pa_poDeviceExecution);

    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int pa_nPriority) = 0;
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
    ;

    /*!\brief Register an Event source Functionblock to this external event source.
     *
     * \param pa_pstTimerListEntry   TimerListEntry data
     * \param pa_roTimeInterval      time interval to next event
     */
    void registerTimedFB(STimedFBListEntry *pa_pstTimerListEntry, const CIEC_TIME &pa_roTimeInterval);
    /*!\brief  Unregister an FB from an the timmer
     *
     * \param pa_poTimedFB FB to unregister from this external event handler
     */
    void unregisterTimedFB(CEventSourceFB *pa_poTimedFB);
    //! one tick of time elapsed. Implementations should call this function on each tick.
    void nextTick(void);
    //! returns the time since startup of FORTE
    const SForteTime &getForteTime() const{
      return m_stForteTime;
    }
  protected:
    CSyncObject m_oSync;

  private:
    /*!\brief The runtime time in tics till last reboot.
     */
    SForteTime m_stForteTime;
    //! List of function blocks currently registered to the timer handler
    STimedFBListEntry *m_pstTimedFBList;
    //!Add an entry to the timed list.
    void addTimedFBEntry(STimedFBListEntry *pa_pstTimerListEntry);

};

#endif /*TIMERHA_H_*/
