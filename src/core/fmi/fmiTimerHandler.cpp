/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "fmiTimerHandler.h"
#include "devexec.h"
#include "fmuInstance.h"

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& pa_poDeviceExecution){
  return new fmiTimerHandler(pa_poDeviceExecution);
}

fmiTimerHandler::fmiTimerHandler(CDeviceExecution& pa_poDeviceExecution) : CTimerHandler(pa_poDeviceExecution)  {
}

fmiTimerHandler::~fmiTimerHandler(){
}

void fmiTimerHandler::enableHandler(void){
}

void fmiTimerHandler::disableHandler(void){
}

void fmiTimerHandler::setPriority(int pa_nPriority){
  NOT_USED(pa_nPriority)
}

int fmiTimerHandler::getPriority(void) const {
  return 0;
}

bool fmiTimerHandler::advanceTicks(CIEC_LREAL& time)
{
  unsigned int noOfTicks = static_cast<unsigned int>(getTicksPerSecond() * static_cast<TForteDFloat>(time));
  //fmi2Real toTest = time;
  fmi2Real toTest = static_cast<fmi2Real>(static_cast<fmi2Real>(noOfTicks) / getTicksPerSecond()); //TODO: test if this problem arise again in 20-Sim

#ifdef FMU_DEBUG
  FMU_DEBUG_LOG("AdvanceTicks: time = " <<  static_cast<TForteDFloat>(time) << ", and toTest = " << toTest << ", noOfTicks = " << noOfTicks << "\n")
#endif

  if(0 != noOfTicks){ //you can start at any time, so normally, it starts at zero

#ifdef FMU_DEBUG
  ++m_stForteTimeFMI;
  FMU_DEBUG_LOG("Tick increased to " << m_stForteTimeFMI.m_nLowerValue << "\n")
#endif
    nextTick();
    noOfTicks--;
    for(std::vector<CEventChainExecutionThread*>::const_iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){ //this waits for all EventChain to finish be clean of events, meaning the they're not processing events.
      (*it)->setAllowedToRun(true);
      (*it)->resumeSelfSuspend();
    }

    while(noOfTicks){
#ifdef FMU_DEBUG //for some reason, if this block is not here, the co-simulation doesn't work. It goes just until 0.4 seconds
  ++m_stForteTimeFMI;
  FMU_DEBUG_LOG("Tick increased to " << m_stForteTimeFMI.m_nLowerValue << "\n")
#endif
      nextTick();
      noOfTicks--;
    }

    for(std::vector<CEventChainExecutionThread*>::const_iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){ //this waits for all EventChain to finish be clean of events, meaning the they're not processing events.
      (*it)->setAllowedToRun(false);
    }
  }

  return true;//(toTest == time);
}
