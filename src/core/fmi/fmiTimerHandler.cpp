/*******************************************************************************
 * Copyright (c) 2016 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "fmiTimerHandler.h"
#include "devexec.h"
#include "fmuInstance.h"
#include "../../arch/utils/timespec_utils.h"
#include <time.h>

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

/*
 * This function is the core of the co-simulation, so this will help understand what's going on here
 * The Ecets can be suspended because of lacking of event, or because it's not currently in a step of the co-simulation
 * Each instance has a boolean for allow the Ecets to run, and a semaphore to suspend them when not in Step mode (when the simulation runs)
 * These two variables are passed as parameters because the timerHandler has no access to its fmuInstance.
 * This method does the following:
 * 1- Calculate the ticks from the time to be simulated
 * 2- Put all Ecets in StepSuspend state, meaning, waiting for a new step, not a new event.
 * 3- Allow the Ecets to run, and release the semaphore for all Ecets
 * 4- Run the ticks
 * 5- Since this loop can be fast enough that the Ecets didn't processed all internal events, but they would have processed if the time was real, we calculate the time that passed from before to after the ticks
 * 6- We calculate the difference with the request time and see how much time is left
 * 7- Then, we go in a loop and unsupend the Ecets until no internal event is there anymore or the actual time passed
 */

bool fmiTimerHandler::advanceTicks(CIEC_LREAL& paTime, bool* paAllowToRun, forte::arch::CSemaphore* paEcetSemaphore)
{
  unsigned int noOfTicks = static_cast<unsigned int>(getTicksPerSecond() * static_cast<TForteDFloat>(paTime));

  if(0 != noOfTicks){ //you can start at any time, so normally, it starts at zero

    putAllEcetInWaitingStepState(paAllowToRun, paEcetSemaphore);

    struct timespec startTIme;

    *paAllowToRun = true;

    timespec_get(&startTIme, TIME_UTC);

    for(unsigned int i = 0; i < eventChainExecutions.size(); i++){
      paEcetSemaphore->inc();
    }

    while(noOfTicks--){
      nextTick();
      CThread::sleepThread(0); //to let others threads run
    }

    *paAllowToRun = false;

    struct timespec endTime;
    timespec_get(&endTime, TIME_UTC);

    struct timespec actualPassedTime;
    timespecSub(&endTime, &startTIme, &actualPassedTime);

    CIEC_LINT nanosecondsRequested = static_cast<TForteInt64>(paTime * 1000000000);
    struct timespec requestedTime = { nanosecondsRequested / 1000000000, static_cast<long>(nanosecondsRequested % 1000000000)};

    if(timespecLessThan(&actualPassedTime, &requestedTime) && stillSomeEvents()){

      struct timespec remainingTime;
      timespecSub(&requestedTime, &actualPassedTime, &remainingTime);

      do{
        putAllEcetInWaitingStepState(paAllowToRun, paEcetSemaphore);

        *paAllowToRun = true;

        timespec_get(&startTIme, TIME_UTC);

        for(unsigned int i = 0; i < eventChainExecutions.size(); i++){
          paEcetSemaphore->inc();
        }

        CThread::sleepThread(0); //let others threads run
        *paAllowToRun = false;

        timespec_get(&endTime, TIME_UTC);

        timespecSub(&endTime, &startTIme, &actualPassedTime);

        timespecSub(&remainingTime, &actualPassedTime, &remainingTime);

      }while(remainingTime.tv_nsec >= 0 && stillSomeEvents());
    }
  }

  return true;//(toTest == time);
}

bool fmiTimerHandler::stillSomeEvents(){
  bool stillSomeEvent = false;
  for(std::vector<CFMUEventChainExecutionThread*>::iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){
    if((*it)->hasMoreEvents()){
      stillSomeEvent = true;
      break;
    }
  }

  return stillSomeEvent;
}

void fmiTimerHandler::putAllEcetInWaitingStepState(bool* paAllowToRun, forte::arch::CSemaphore* paEcetSemaphore){

  *paAllowToRun = false;

  //empty the semaphore first
  while(paEcetSemaphore->tryNoWait());

  //In case some Ecet is suspended because of lacking of events, take them out of that state, and make them wait for the next step
  for(std::vector<CFMUEventChainExecutionThread*>::iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){
    (*it)->resumeSelfSuspend();
  }

  //wait for the stepWaitingstep of all of them
  bool areAllEcetReady = true;
  do{
    areAllEcetReady = true;
    for(std::vector<CFMUEventChainExecutionThread*>::iterator it = eventChainExecutions.begin(); it != eventChainExecutions.end(); ++it){
      if(!(*it)->isInWaitingStepState()){
        areAllEcetReady = false;
        CThread::sleepThread(0);
        break;
      }
    }
  }while(!areAllEcetReady);
}
