/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _TXTIMEHA_H_
#define _TXTIMEHA_H_

#include <threadx/tx_api.h>
#include "../timerha.h"
#include "../devlog.h"

/*! \ingroup netos_hal 
 * \ingroup EXTEVHAND
 * \brief Timerhandler for NET+OS. 
 *  
 */
class CTXTimerHandler : public CTimerHandler{
  public:
    virtual ~CTXTimerHandler();

    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler(void);
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void);
    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int pa_nPriority);
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const;
  private:
    /*!\brief callback function for the system timer
     */
    static void timerHandlerFunc(ULONG data){
      if(data){
        static_cast<CTXTimerHandler*>(data)->nextTick();
      }
    }
    ;

    explicit CTXTimerHandler(CDeviceExecution& pa_poDeviceExecution);

    TX_TIMER m_stTimer;

    friend class CTimerHandler;
};

#endif /*PCTIMEHA_H_*/
