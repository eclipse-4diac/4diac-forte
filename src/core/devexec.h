/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _DEVEXEC_H
#define _DEVEXEC_H

class CFunctionBlock;
class CEventSourceFB;
class CExternalEventHandler;
struct SForteTime;
class CTimerHandler;

#include <forte_config.h>

/**\ingroup CORE
 Handles all the IEC 61499 execution requests and aspects within one device

 @author az
 */
class CDeviceExecution {
  public:
    CDeviceExecution();

    ~CDeviceExecution();

    /*!\brief Notifies the CExternalEventHandlerManager that one tick in the time has passed by.
     *
     * \param paCurrentTime the current time in ticks since the last start of the runtime.
     */
    void notifyTime(uint_fast64_t) {
    }

    /*!\brief an external event occurred at an ES and a new event source is to start.
     *
     * In this function the CDeviceExecution does all means necessary that the new event chain will be executed correctly.
     * \param paECStartFB The start FB of the event chain
     */
    void startNewEventChain(CEventSourceFB* paECStartFB) const;
    /*!\brief Check if an occurrence of the given event handler is currently allowed.
     *
     * With this function the device execution can disable or enable the notification on external events.
     * Necessary to hold timing constraints. The notifyTime()-function will be used to determine if different external event
     * handlers are to be enabled again.
     * \param pa_nExtEvHandler ID of the external event handler given by the CDeviceExecution on registerExternalEvnetHandler()
     * \return true if the external event handler is allowed to start new event chains.
     */
    bool extEvHandlerIsAllowed(size_t) const {
      return true;
    }

    CTimerHandler& getTimer() const;

    template<typename T>
    T& getExtEvHandler() {
      return static_cast<T&>(*getExtEvHandler(T::mHandlerIdentifier));
    }

    bool isExtEvHandlerValid(size_t paIdentifier) const {
      return (0 != getExtEvHandler(paIdentifier));
    }

    void disableHandlers();

  private:
    /*!\brief Structure for holding the information belonging to one external event.
     */
    struct SEventHandlerElement {
        bool mOccured; //!<flag indicating that the external event has occurred between the last invocation.
        CExternalEventHandler *mHandler; //!< pointer to the external event handler instance.
    };

    CExternalEventHandler* getExtEvHandler(size_t paIdentifer) const;

    static void createHandlers(CDeviceExecution& paDeviceExecution);

    /*!\brief List of currently available external event sources.
     *
     * The element 0 is always the timer event source.
     */
    SEventHandlerElement mRegisteredEventHandlers[cg_unNumberOfHandlers];

};

#endif
