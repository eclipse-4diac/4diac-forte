/*******************************************************************************
  * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Thomas Strasser, Ingo Hegny
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _DEVEXEC_H
#define _DEVEXEC_H

const int cg_MaxRegisteredEventHandlers = 10;

class CEventSourceFB;
class CExternalEventHandler;
struct SForteTime;
class CTimerHandler;

/**\ingroup CORE
  Handles all the IEC 61499 execution requests and aspects within one device

  @author az
*/
class CDeviceExecution{
public:
    CDeviceExecution();

    ~CDeviceExecution();

/*!\brief Register a new external event handler
 *
 * Prepares internal structures for the new event handler.
 * \param pa_poHandler pointer to the external event handler instance
 * \return ID of the new external event handler.
 */
    int registerExternalEventHandler(CExternalEventHandler *pa_poHandler);

/*!\brief Notifies the CExternalEventHandlerManager that one tick in the time has passed by.
 *
 * \param pa_nCurrentTime the current time in ticks since the last start of the runtime.
 */
    void notifyTime(const SForteTime& ){
    };

/*!\brief an external event occurred at an ES and a new event source is to start.
 *
 * In this function the CDeviceExecution does all means necessary that the new event chain will be executed correctly.
 * \param pa_poECStartFB The start FB of the event chain
 */
    void startNewEventChain(CEventSourceFB *pa_poECStartFB);
/*!\brief Check if an occurrence of the given event handler is currently allowed.
 *
 * With this function the device execution can disable or enable the notification on external events.
 * Necessary to hold timing constraints. The notifyTime()-function will be used to determine if different external event
 * handlers are to be enabled again.
 * \param pa_nExtEvHandler ID of the external event handler given by the CDeviceExecution on registerExternalEvnetHandler()
 * \return true if the external event handler is allowed to start new event chains.
 */
    bool extEvHandlerIsAllowed(int ){
      return true;
    };

    CTimerHandler& getTimer() const{
      return *mFORTETimer;
    }

protected:
private:
/*!\brief Structure for holding the information belonging to one external event.
*
*/
  struct SEventHandlerElement{
    bool m_bOccured; //!<flag indicating that the external event has occurred between the last invocation.
    CExternalEventHandler *m_poHandler; //!< pointer to the external event handler instance.
  };
/*!\brief List of currently available external event sources.
*
* The element 0 is always the timer event source.
*/
  SEventHandlerElement m_astRegisteredEventHandlers [ cg_MaxRegisteredEventHandlers ];
  int m_nNumberofExternalEventHandler; //!< number of currently used external event handlers in the m_asRegisteredEventHandlers list.

  CTimerHandler *mFORTETimer;


};

#endif
