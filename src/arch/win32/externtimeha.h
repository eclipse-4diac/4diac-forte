/*******************************************************************************
 * Copyright (c) 2011 Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _PCTIMEHA_H_
#define _PCTIMEHA_H_

#include "timerha.h"

/*! \ingroup pc_hal 
 *\ingroup EXTEVHAND
 *\brief the timer handler for the pc architecture. 
*/
class CExternTimerHandler : public CTimerHandler{
  public:
    static void externNextTick();
    static TForteUInt32 getExternTicksPerSecond(){ return csmTicksPerSecond; };
    explicit CExternTimerHandler(CDeviceExecution& paDeviceExecution);
      
    ~CExternTimerHandler() override;

    /*!\brief Pointer to a general timer used for code outside FBs, like devlog, TIME() and so on
     */
    static CExternTimerHandler *smFORTEExtTimer;
    
  
  /*!\brief Enables this event source
   * 
   */
    void enableHandler() override;
  /*!\brief Disable this event source
   */  
    void disableHandler() override;
  /*!\brief Sets the priority of the event source  
   * 
   * \param paPriority new priority of the event source
   */
    void setPriority(int paPriority) override;
  /*!\brief Get the current priority of the event source
   * 
   * \return current priority
   */  
    int getPriority() const override;
    
  /*! \brief Get the time base of the runtime
   * 
   * \return internal runtime ticks per millisecond
   */ 
    virtual TForteUInt32 getTicksPerSecond(){ return csmTicksPerSecond; };

  private:
  /*!\brief default time base of the used timer. in useconds.
   */
    static const TForteInt32 csmTicksPerSecond;
  /*!\brief callback function for the system timer
   */
    
};

#endif /*PCTIMEHA_H_*/
