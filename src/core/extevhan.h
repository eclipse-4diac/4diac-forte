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

#include "devexec.h"
class CEventSourceFB;
class CFunctionBlock;

#define DECLARE_HANDLER(TypeName)                             \
  public:                                                     \
    static const size_t mHandlerIdentifier;              \
    virtual size_t getIdentifier() const;               \
    explicit TypeName(CDeviceExecution& paDeviceExecution);\
    ~TypeName();

#define DEFINE_HANDLER(TypeName)                            \
    size_t TypeName::getIdentifier() const { return TypeName::mHandlerIdentifier;}

#define GET_HANDLER_FROM_FB(fb, TypeName)                 \
  static_cast<TypeName*>(CExternalEventHandler::getHandlerFromFB(fb, TypeName::mHandlerIdentifier))

#define GET_HANDLER_FROM_THIS(TypeName)                 \
    GET_HANDLER_FROM_FB(*this, TypeName)

#define GET_HANDLER_FROM_COMM_LAYER(TypeName)                 \
    GET_HANDLER_FROM_FB(*m_poFb, TypeName)

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
    explicit CExternalEventHandler(CDeviceExecution& paDeviceExecution);

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
     * \param paPriority new priority of the event source
     */
    virtual void setPriority(int paPriority) = 0;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const = 0;

    virtual size_t getIdentifier() const = 0;

    static CExternalEventHandler* getHandlerFromFB(CFunctionBlock& paFB, size_t paIdentifier);

  protected:

    /*! \brief Check if the external event handler is allowed to start event chains
     *
     */
    bool isAllowed();

    /*!\brief register event source at device execution for starting a new event chain
     *
     * this function checks if the external event handler is allowed to start new event chains and if yes performs the necessary actions.
     *
     * @param paECStartFB the event source function block which starts the new event chain
     */
    void startNewEventChain(CEventSourceFB *paECStartFB);


    template<typename T>
    T* getHandlerFromHandler(){
      return static_cast<T*>(m_poDeviceExecution.getHandler(T::mHandlerIdentifier));
    }

    bool isHandlerValid(size_t paIdentifier){
      return (0 != m_poDeviceExecution.getHandler(paIdentifier));
    }

    CDeviceExecution& m_poDeviceExecution;

  private:

};

/*@}*/
/*@}*/
#endif /*EXTEVHAN_H_*/
