/*******************************************************************************
  * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _EXTEVHAN_H_
#define _EXTEVHAN_H_

class CDeviceExecution;
class CEventSourceFB;

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
 *   - registerFB(CFunctionBlock *pa_poESFB, ...) allows the ES-FB to register to this ExternalEventHandler for recieving external 
 *      events (e.g. INIT+). the specific parameters depend on the ExternalEventHandler.
 *   - unregisterFB(CFunctionBlock *pa_poESFB) the ES-FB doesn't want to receive any external events any more (e.g. INIT-).
 */
class CExternalEventHandler{
  public:
    CExternalEventHandler();

    virtual ~CExternalEventHandler(){
    }
    ;
    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler(void) = 0;
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void) = 0;
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

    static void setDeviceExecution(CDeviceExecution *pa_poDevExec){
      sm_poDeviceExecution = pa_poDevExec;
    }
    ;
    static CDeviceExecution *getDeviceExecution(void){
      return sm_poDeviceExecution;
    }
    ;
  protected:

    int getExternalEventHandlerID(){
      return m_nExtEvHandID;
    }

    /*! \brief Check if the external event handler is allowed to start event chains
     *
     */
    bool isAllowed();

    /*!\brief register event source at device execution for starting a new event chain
     *
     * this function checks if the external event handler is allowed to start new event chains and if yes performs the necessary actions.
     *
     * @param pa_poECStartFB the event source function block which starts the new event chain
     */
    void startNewEventChain(CEventSourceFB *pa_poECStartFB);

  private:
    static CDeviceExecution *sm_poDeviceExecution; //!< Reference to the device execution needed for all external event handlers to ask for execution and start new event chains.

    //!ID for this timer callback as given by the CDeviceExecution
    int m_nExtEvHandID;

  private:

};

/*@}*/
/*@}*/
#endif /*EXTEVHAN_H_*/
