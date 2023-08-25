/*******************************************************************************
 * Copyright (c) 2016 -2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FMITIMERHANDLER_H_
#define _FMITIMERHANDLER_H_

#include "../../arch/timerha.h"
#include "../fmuEcet.h"
#include "../datatypes/forte_lreal.h"
#include <vector>

/*!\brief Timer handler for FMI/FMU
 *
 */
class fmiTimerHandler : public CTimerHandler{
  public:

    explicit fmiTimerHandler(CDeviceExecution& paDeviceExecution);

    ~fmiTimerHandler() override;

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

    /*!\brief Advance the ticks according of the time
     */
    bool advanceTicks(CIEC_LREAL& time, bool* paAllowToRun, forte::arch::CSemaphore* paEcetSemaphore);

    void addExecutionThread(CFMUEventChainExecutionThread* paExecutionThread){
      eventChainExecutions.push_back(paExecutionThread);
    }

    void removeExecutionThread(CEventChainExecutionThread* paExecutionThread){
      std::vector<CFMUEventChainExecutionThread*>::iterator toDelete = eventChainExecutions.end();
      for(std::vector<CFMUEventChainExecutionThread*>::iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){
        if(paExecutionThread == (*it)){
          toDelete = it;
          break;
        }
       }
      if(eventChainExecutions.end() != toDelete){
        eventChainExecutions.erase(toDelete);
      }
    }

  private:
    std::vector<CFMUEventChainExecutionThread*> eventChainExecutions;

    bool stillSomeEvents();

    void putAllEcetInWaitingStepState(bool* paAllowToRun, forte::arch::CSemaphore* paEcetSemaphore);

};

#endif /* _FMITIMERHANDLER_H_ */
