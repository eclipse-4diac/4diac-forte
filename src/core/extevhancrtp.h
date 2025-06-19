/*******************************************************************************
 * Copyright (c) 2025 Monika Wenger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * uses CRTP (Curiously Recurring Template Pattern)
 *******************************************************************************/
#ifndef _EXTEVHANCRTP_H_
#define _EXTEVHANCRTP_H_

#include <cstddef>
#include <cstdint>
#include "devlog.h"
#include "devexec.h"
#include "iextevhan.h"

class CEventSourceFB;
class CFunctionBlock;
class CDeviceExecution;

/**  \defgroup FORTE_HAL FORTE Hardware Abstraction Layer - FORTE-HAL
 * \brief The FORTE-HAL is the abstraction of HW dependent features important
 * and needed in each port of FORTE.
 */
/*@{*/

/**\defgroup EXTEVHAND External Event Handling
 */
/*@{*/

/*! \brief Baseclass for handling incoming interrupts and similar external events.
 *
 * Implementations of such classes should provide the following functions for ES-FBs:
 *   - registerFB(CFunctionBlock *paESFB, ...) allows the ES-FB to register to this ExternalEventHandler for recieving
 * external events (e.g. INIT+). the specific parameters depend on the ExternalEventHandler.
 *   - unregisterFB(CFunctionBlock *paESFB) the ES-FB doesn't want to receive any external events any more (e.g. INIT-).
 */

template<typename Derived>
class CExternalEventHandlerCRTP : public IExternalEventHandler {
  public:
    explicit CExternalEventHandlerCRTP(CDeviceExecution &paDeviceExecution) :
        mDeviceExecution(paDeviceExecution) {
    }

    virtual ~CExternalEventHandlerCRTP() = default;
    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler() = 0;
    /*!\brief Disable this event source
     */
    virtual void disableHandler() = 0;
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    virtual void setPriority(int paPriority) = 0;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority() const = 0;

    size_t getIdentifier() const {
      return Derived::mHandlerIdentifier;
    }

  protected:
    CDeviceExecution &mDeviceExecution;

    /*! \brief Check if the external event handler is allowed to start event chains
     *
     */
    bool isAllowed() {
      return mDeviceExecution.extEvHandlerIsAllowed(getIdentifier());
    }

    /*!\brief register event source at device execution for starting a new event chain
     *
     * this function checks if the external event handler is allowed to start new event chains and if yes performs the
     * necessary actions.
     *
     * @param paECStartFB the event source function block which starts the new event chain
     */
    void startNewEventChain(CEventSourceFB *paECStartFB) {
      if (isAllowed()) {
        FORTE_TRACE("Starting EC\n");
        mDeviceExecution.startNewEventChain(paECStartFB);
      } else {
        // TODO: handle this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        DEVLOG_DEBUG("Starting EC NOT ALLOWED !!!!!!!!!!!!!!!1\n");
      }
    }

    template<typename T>
    T& getExtEvHandler() {
      return mDeviceExecution.getExtEvHandler<T>();
    }

  private:
};

/*@}*/
/*@}*/
#endif /*EXTEVHANCRTP_H_*/
